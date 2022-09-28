// Fill out your copyright notice in the Description page of Project Settings.


#include "StagePawn.h"

#include "Kismet/GameplayStatics.h"


// Sets default values
AStagePawn::AStagePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	//Get stage mesh and spawn it
	UStaticMesh* SMStageMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/Mesh/stage/stage"));
	StageMesh = CreateDefaultSubobject<UStaticMeshComponent>("stage");
	//SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");

	StageMesh->SetStaticMesh(SMStageMesh);
	RootComponent = StageMesh;
	// SpringArm->SetupAttachment(RootComponent);
	Camera->SetupAttachment(RootComponent);

	Camera->SetRelativeLocation(FVector(-1200.0f, 0.0f, 0.0f));

}

// Called when the game starts or when spawned
void AStagePawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AStagePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AStagePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AStagePawn::Initialize(APuyoConfigActor* Config)
{
	PuyoConfig = Config;

	RootComponent->SetWorldLocation(FVector(PuyoConfig->PosX, PuyoConfig->InitY, PuyoConfig->InitZ));
	RootComponent->SetWorldRotation(FRotator(0, 0, 0));
	RootComponent->SetWorldScale3D(FVector(1.0, PuyoConfig-> StageCols, PuyoConfig->StageRows));

	//Get PuyoMesh Actor
	PuyoMesh = Cast<APuyoMesh>(UGameplayStatics::GetActorOfClass(GetWorld(), APuyoMesh::StaticClass()));

	//Prepare memory
	TArray<FPuyoMemoryData> ZeroArray;
	ZeroArray.Init({0, nullptr}, PuyoConfig->StageCols);
	for(int32 i = 0 ; i<PuyoConfig->StageRows;++i)
	{
		Board.Add(ZeroArray);
	}
	
	PuyoCount = 0;
	for(int32 z=0;z<PuyoConfig->StageRows;++z)
	{
		TArray<FPuyoMemoryData> Line = Board[z];
		for(int32 y=0;y<PuyoConfig->StageCols;++y)
		{
			FPuyoMemoryData Puyo = Line[y];
			if(Puyo.Puyo >= 1 && Puyo.Puyo <= 5)
			{
				SetPuyo(Puyo.Puyo, y, z);
				++PuyoCount;
			}
			else
			{
				Line[y] = {0, nullptr};
			}
		}
	}
	this->PuyoCount = PuyoCount;
	// ShowArray();
	
}

//Set puyo color index and StaticMeshActor
void AStagePawn::SetPuyo(int Puyo, int32 y, int32 z)
{
	if(PuyoMesh)
	{
		//Get PuyoMeshActor
		AStaticMeshActor* FixedPuyoMeshActor = PuyoMesh->GetPuyo(Puyo);
		FixedPuyoMeshActor->SetActorLocation(FVector(PuyoConfig->PosX, PuyoConfig->PuyoMeshWidth * y, PuyoConfig->PuyoMeshHeight * -z));

		Board[z][y] = {Puyo, FixedPuyoMeshActor};

	}
	// ShowArray();
}

void AStagePawn::ShowArray()
{
	TArray<FString> Array;
	for(int32 i = 0; i < Board.Num(); ++i)
	{
		FString Line;
		for(int32 j = 0; j < Board[i].Num(); ++j)
		{
			Line += FString::FromInt(Board[i][j].Puyo);
		}
		Array.Add(Line);
	}
	UE_LOG(LogTemp, Log, TEXT("Array is: %s"), *FString::Join(Array, TEXT("\n")));
	
}

//Check freefall
bool AStagePawn::CheckFall()
{
	FallingPuyoArray.Empty();
	bIsFalling = false;

	//check from bottom to top
	for(int32 z = PuyoConfig->StageRows - 2; z >= 0; z--)
	{
		TArray<FPuyoMemoryData> Line = Board[z];
		for(int32 y = 0; y < Line.Num(); y++)
		{
			if(Board[z][y].Puyo == 0)
			{
				//if puyo isnt in this space, go next
				continue;
			}
			if(Board[z+1][y].Puyo == 0)
			{
				//this puyo is gonna fall, so remove it
				FPuyoMemoryData cell = Board[z][y];
				Board[z][y] = {0, nullptr};
				int32 Dst = z;
				while(Dst + 1 < PuyoConfig->StageRows && Board[Dst+1][y].Puyo == false)
				{
					Dst++;
				}
				//arrive destination
				Board[Dst][y] = cell;
				int32 PosY = y;
				//append puyo to falling array
				FallingPuyoArray.Push({cell, z * PuyoConfig->PuyoMeshHeight, Dst * PuyoConfig->PuyoMeshHeight, true});
				//record that puyo falled
				bIsFalling = true;
			}
		}
	}
	return bIsFalling;
}

//Free fall
bool AStagePawn::Fall()
{
	bIsFalling = false;
	for(int32 i = 0; i < FallingPuyoArray.Num(); i++)
	{
		if(!FallingPuyoArray[i].Falling)
		{
			//alreday arrived
			continue;
		}
		int32 PosZ = FallingPuyoArray[i].PosZ;
		PosZ += int32(PuyoConfig->FreeFallingSpeed) * 10;
		if(PosZ >= FallingPuyoArray[i].DstZ)
		{
			//finish falling
			PosZ = FallingPuyoArray[i].DstZ;
			FallingPuyoArray[i].Falling = false;
		}
		else
		{
			//record that puyo is still falling
			bIsFalling = true;
		}
		//save the new position of z
		FallingPuyoArray[i].PosZ = PosZ;
		//move puyo
		PuyoMeshActor = FallingPuyoArray[i].PuyoMemoryData.PuyoMeshActor;
		PuyoMeshActor->SetActorLocation(FVector(PuyoConfig->PosX, PuyoMeshActor->GetActorLocation().Y, -PosZ));
	}
	return  bIsFalling;
}

//check if puyo can be erased
TArray<int32> AStagePawn::CheckErase(int32 InStartFrame)
{
	EraseStartFrame = InStartFrame;
	ErasingPuyoArray.Empty();

	//record what color puyo is erased
	TMap<int32, bool> ErasedPuyoColorMap;

	//create a function to check neighbor puyo
	TArray<FPuyoEraseData> SequencePuyoArray;
	TArray<FPuyoEraseData> ExistingPuyoArray;

	TFunction<void(const int32, const int32)> CheckSequentialPuyo = [&](int32 y, int32 z)
	{
		//confirm existing puyo
		int32 orig = this->Board[z][y].Puyo;
		if(orig == 0)
		{
			//if not exist, do nothing
			return;
		}
		//if exist, it avoid temporary and erase from memory
		FPuyoMemoryData Puyo = this->Board[z][y];
		SequencePuyoArray.Push({y, z, this->Board[z][y]});
		this->Board[z][y] = {0, nullptr};

		TArray<TArray<int32>> Direction = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
		for(int32 i = 0; i < Direction.Num(); i++)
		{
			int32 dy = y + Direction[i][0];
			int32 dz = z + Direction[i][1];
			if(dy < 0 || dy >= PuyoConfig->StageCols || dz < 0 || dz >= PuyoConfig->StageRows)
			{
				//out of stage
				continue;
			}
			FPuyoMemoryData cell = this->Board[dz][dy];
			if(!cell.Puyo || cell.Puyo != Puyo.Puyo)
			{
				//if not exist or not same color, do nothing
				continue;
			}
			//check if i can erase puyos around the puyo
			CheckSequentialPuyo(dy, dz);
		}
	};

	//do confirm erase puyo
	for(int32 z = 0; z < PuyoConfig->StageRows; z++)
	{
		for(int32 y = 0; y < PuyoConfig->StageCols; y++)
		{
			SequencePuyoArray.Empty();
			int32 PuyoColor = Board[z][y].Puyo && Board[z][y].Puyo != 0;
			CheckSequentialPuyo(y, z);
			if(SequencePuyoArray.Num() == 0 || SequencePuyoArray.Num() < PuyoConfig->ErasePuyoNum)
			{
				//if not exist or not enough erase puyo, not erase
				if(SequencePuyoArray.Num() > 0)
				{
					//push avoided puyo to list
					for(int32 i = 0; i < SequencePuyoArray.Num(); i++)
					{
						ExistingPuyoArray.Push({SequencePuyoArray[i].Y, SequencePuyoArray[i].Z, SequencePuyoArray[i].Cell});
					}
				}
			}else
			{
				//these are erasable puyo, so add to the erase list
				for(int32 i = 0; i < SequencePuyoArray.Num(); i++)
				{
					ErasingPuyoArray.Push({SequencePuyoArray[i].Y, SequencePuyoArray[i].Z, SequencePuyoArray[i].Cell});
				}
				ErasedPuyoColorMap.Add(PuyoColor, true);
			}
		}
	}
	PuyoCount -= ErasingPuyoArray.Num();

	//restore the puyo is in existing list to memory
	for(FPuyoEraseData info : ExistingPuyoArray)
	{
		Board[info.Z][info.Y] = info.Cell;
		
	}

	if(ErasingPuyoArray.Num())
	{
		//if i can erase, return the number of erasing puyo and color
		TArray<int32> ErasedPuyoData = {ErasingPuyoArray.Num(), ErasedPuyoColorMap.Num()};
		return ErasedPuyoData;
	}
	return {0};
}

//erasing animation
bool AStagePawn::Erasing(int32 frame)
{
	for(FPuyoEraseData info: ErasingPuyoArray)
	{
		PuyoMesh->DestroyPuyo(info.Cell.PuyoMeshActor);
	}
	return false;
}

void AStagePawn::DestroyAllPuyo()
{
	for(int32 z = 0; z < PuyoConfig->StageRows; z++)
	{
		for(int32 y = 0; y < PuyoConfig->StageCols; y++)
		{
			if(Board[z][y].Puyo)
			{
				PuyoMesh->DestroyPuyo(Board[z][y].PuyoMeshActor);
				Board[z][y] = {0, nullptr};
			}
		}
	}
}



