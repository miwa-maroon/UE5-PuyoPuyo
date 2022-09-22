// Fill out your copyright notice in the Description page of Project Settings.


#include "PuyoGameModeBase.h"
#include "Kismet/GameplayStatics.h"


APuyoGameModeBase::APuyoGameModeBase()
{
	PlayerControllerClass = APuyoPlayerController::StaticClass();
	DefaultPawnClass = AStagePawn::StaticClass();
	PrimaryActorTick.bCanEverTick = true;
}

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

	PuyoMesh = GetWorld()->SpawnActor<APuyoMesh>(APuyoMesh::StaticClass());

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	PuyoPlayerController = Cast<APuyoPlayerController>(PlayerController);
	
	StagePawn = Cast<AStagePawn>(PuyoPlayerController->GetPawn());

	APuyoConfigActor* PuyoConfigActor = Cast<APuyoConfigActor>(UGameplayStatics::GetActorOfClass(GetWorld(), APuyoConfigActor::StaticClass()));
	//FString MsgInt = FString::FromInt(PuyoConfigActor->StageRows);
	//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Cyan, MsgInt);
	
	InitializeGame(PuyoConfigActor);
	
}

void APuyoGameModeBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	time += DeltaSeconds;
	if(time > 0.5f)
	{
		Loop();
		time = 0.0f;
	}
	
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
	//ScoreInitalize();
	mode = "start";
	frame = 0;
	time = 0.0f;
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
        	//ScoreCalculate(combinationCount, eraseInfo.piece, eraseInfo.color);
        	//StagePawn.hideZenkeshi();
        }
        else
        {
        	if(StagePawn->PuyoCount == 0 && combinationCount > 0)
        	{
        		//StagePawn.ShowZenkeshi();
        		//ScoreAddScore(3600);
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
		// PuyoMesh.PrePareBatankyu(frame);
  //       mode = "batankyu";
	}else if(mode == "batankyu")
	{
		// PuyoMesh.Batankyu(frame);
  //       PlayerController.Batankyu();
	}
	UE_LOG(LogTemp, Log, TEXT("mode: %s"), *mode);
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, mode);
	frame++;
	
}



