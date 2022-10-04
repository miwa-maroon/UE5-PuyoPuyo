// Fill out your copyright notice in the Description page of Project Settings.


#include "PuyoGameModeBase.h"


#include "Kismet/GameplayStatics.h"


APuyoGameModeBase::APuyoGameModeBase()
{
	PlayerControllerClass = APuyoPlayerController::StaticClass();
	DefaultPawnClass = AStagePawn::StaticClass();
	PlayerStateClass = APuyoPlayState::StaticClass();
	HUDClass = APuyoHUD::StaticClass();
	PrimaryActorTick.bCanEverTick = true;
	
}

//dont use
void APuyoGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	const FString Msg("begin!!");
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Cyan, Msg);



	SubscribeTest();

	auto period = std::chrono::milliseconds(1);

	auto testObservable = rxcpp::observable<>::timer(period);
	testObservable.subscribe([](int v)
	{
		const FString Msg("Test!!");
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, Msg);
	});
	FString PathTitleWidget = "/Game/Widget/WBP_TitleWidget.WBP_TitleWidget_C";
	TitleWidgetClass = TSoftClassPtr<UUserWidget>(FSoftObjectPath(*PathTitleWidget)).LoadSynchronous();
	if(TitleWidgetClass != nullptr)
    {
    	TitleWidget = CreateWidget<UTitleWidget>(GetWorld(), TitleWidgetClass);
        if(TitleWidget)
        {
            TitleWidget->AddToViewport();
        }
    }

	FString PathScoreWidget = "/Game/Widget/WBP_ScoreWidget.WBP_ScoreWidget_C";
	ScoreWidgetClass = TSoftClassPtr<UUserWidget>(FSoftObjectPath(*PathScoreWidget)).LoadSynchronous();
	if(ScoreWidgetClass != nullptr)
	{
		ScoreWidget = CreateWidget<UScoreWidget>(GetWorld(), ScoreWidgetClass);
	}
	
	
	

	//PuyoMesh = GetWorld()->SpawnActor<APuyoMesh>(APuyoMesh::StaticClass());

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	PuyoPlayerController = Cast<APuyoPlayerController>(PlayerController);
	
	StagePawn = Cast<AStagePawn>(PuyoPlayerController->GetPawn());

	PuyoConfigActor = Cast<APuyoConfigActor>(UGameplayStatics::GetActorOfClass(GetWorld(), APuyoConfigActor::StaticClass()));
	//FString MsgInt = FString::FromInt(PuyoConfigActor->StageRows);
	//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Cyan, MsgInt);
	
	InitializeGame(PuyoConfigActor);
	
}

//dont use
void APuyoGameModeBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	Loop();
	
	
}

void APuyoGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
	PuyoMesh = GetWorld()->SpawnActor<APuyoMesh>(APuyoMesh::StaticClass());

}

void APuyoGameModeBase::SubscribeTest()
{
	auto TestStream = Test.get_observable();

	TestStream.subscribe([](bool b)
	{
		const FString Msg("Test!!");
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, Msg);
	});
}

void APuyoGameModeBase::InitializeGame(APuyoConfigActor* Config)
{
	StagePawn->Initialize(Config);
	PuyoPlayerController->Initialize(Config);
	mode = "lobby";
	frame = 0;
	time = 0.0f;
	TitleWidget->bIsLobby = true;
}

void APuyoGameModeBase::Loop()
{
	if (mode == "start")
	{
		mode = "checkfall";
	}else if (mode == "checkfall")
	{
		if(StagePawn->CheckFall()){
			mode = "fall";
		}else{
			mode = "checkErase";
		}
	}else if (mode == "fall")
	{
		if(!StagePawn->Fall()){
        	mode = "checkErase";
        }
	}else if (mode == "checkErase")
	{
		TArray<int32> EraseInfo = StagePawn->CheckErase(frame);
        if(EraseInfo[0])
        {
        	mode = "erasing";
        	combinationCount++;
        	CalcScore(combinationCount, EraseInfo[0], EraseInfo[1]);
        	//StagePawn.hideZenkeshi();
        }
        else
        {
        	if(StagePawn->PuyoCount == 0 && combinationCount > 0)
        	{
        		//StagePawn.ShowZenkeshi();
        		UpdateScore(3600);
        	}
        	combinationCount = 0;
        	mode = "newPuyo";
        }
	}else if (mode == "erasing")
	{
		if(!StagePawn->Erasing(frame))
        {
        	mode = "checkFall";
        }
	}else if (mode == "newPuyo")
	{
		if(!PuyoPlayerController->CreateNewPuyo())
        {
        	mode = "gameover";
        }else
        {
        	mode = "playing";
        }
	}else if(mode == "playing")
	{
		FString action = PuyoPlayerController->Playing(frame);
        mode = action;
	}else if(mode == "moving")
	{
		if(!PuyoPlayerController->Moving(frame))
        {
        	mode = "playing";
        }
	}else if(mode == "rotating")
	{
		if(!PuyoPlayerController->Rotating(frame))
        {
        	mode = "playing";
        }
	}else if(mode == "fix")
	{
		PuyoPlayerController->Fix();
        mode = "checkFall";
	}else if(mode == "gameover")
	{
		ShowGameOverText();
		mode = "batankyu";
	}else if(mode == "batankyu")
	{
		// PuyoMesh.Batankyu(frame);
  //       PlayerController.Batankyu();
		mode = "lobby";
		StagePawn->DestroyAllPuyo();
		if(ScoreWidget)
		{
			ScoreWidget->RemoveFromParent();
			TitleWidget->AddToViewport();
			TitleWidget->bIsLobby = true;
		}
	}else if(mode == "lobby")
	{
		if(!TitleWidget->bIsLobby)
		{
			mode = "start";
			if(ScoreWidget)
			{
				ScoreWidget->AddToViewport();
				InitializeScore();
				HideGameOverText();
			}
		}
		// PlayerController.Lobby();
	}
	UE_LOG(LogTemp, Log, TEXT("mode: %s"), *mode);
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, mode);
	frame++;
	
}

void APuyoGameModeBase::InitializeScore()
{
	Score = 0;
	ShowScoreText();
}

void APuyoGameModeBase::UpdateScore(int32 InScore)
{
	Score += InScore;
	ShowScoreText();
}

void APuyoGameModeBase::CalcScore(int32 InRensa, int32 InChain, int32 InColor)
{
	InRensa = FMath::Min(InRensa, RensaBonus.Num() - 1);
	InChain = FMath::Min(InChain, ChainBonus.Num() - 1);
	InColor = FMath::Min(InColor, ColorBonus.Num() - 1);
	int32 Scale = RensaBonus[InRensa] * ChainBonus[InChain] * ColorBonus[InColor];
	if(Scale == 0)
	{
		Scale = 1;
	}
	UpdateScore(Scale * InChain * 10);
}

void APuyoGameModeBase::ShowScoreText()
{
	ScoreWidget->SetScoreText(Score);
}

void APuyoGameModeBase::ShowGameOverText()
{
	ScoreWidget->SetGameOverText();
}

void APuyoGameModeBase::HideGameOverText()
{
	ScoreWidget->HideGameOverText();
}



