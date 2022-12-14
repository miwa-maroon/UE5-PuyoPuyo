// Fill out your copyright notice in the Description page of Project Settings.


#include "StagePawn.h"
#include "Kismet/GameplayStatics.h"


// Set default values
AStagePawn::AStagePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	//Get stage mesh and spawn it
	//UStaticMesh* SMStageMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/Mesh/stage/stage"));
	//StageMesh = CreateDefaultSubobject<UStaticMeshComponent>("stage");
	//SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	//Camera = CreateDefaultSubobject<UCameraComponent>("Camera");

	//StageMesh->SetStaticMesh(SMStageMesh);
	//RootComponent = StageMesh;
	// SpringArm->SetupAttachment(RootComponent);
	//Camera->SetupAttachment(RootComponent);

	//Camera->SetWorldLocation(TitleCameraPos);
	//Camera->SetWorldRotation(TitleCameraRot);
}


// Called to bind functionality to input
void AStagePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AStagePawn::Initialize(APuyoConfigActor* Config)
{
	PuyoConfig = Config;

	SpawnPuyoMesh();

	//RootComponent->SetWorldLocation(FVector(PuyoConfig->PosX, PuyoConfig->InitY, PuyoConfig->InitZ));
	//RootComponent->SetWorldRotation(FRotator(0, 0, 0));
	//RootComponent->SetWorldScale3D(FVector(1.0, PuyoConfig-> StageCols, PuyoConfig->StageRows));

	//Get PuyoMesh Actor
	MorphPuyoMesh = Cast<AMorphPuyoMesh>(UGameplayStatics::GetActorOfClass(GetWorld(), AMorphPuyoMesh::StaticClass()));

	//Prepare memory
	ZeroArray.Init({0, nullptr}, PuyoConfig->StageCols);
	for(int32 i = 0 ; i<PuyoConfig->StageRows;++i)
	{
		Board.Add(ZeroArray);
	}
	
	PuyoCount = 0;
	for(int32 z=0;z<PuyoConfig->StageRows;++z)
	{
		Line = Board[z];
		for(int32 y=0;y<PuyoConfig->StageCols;++y)
		{
			Puyo = Line[y];
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
void AStagePawn::SetPuyo(int PuyoCol, int32 y, int32 z)
{
	if(MorphPuyoMesh)
	{
		//Get PuyoMeshActor
		AMorphPuyoActor* FixedPuyoMeshActor = MorphPuyoMesh->GetMorphPuyo(PuyoCol);
		FixedPuyoMeshActor->SetActorLocation(FVector(PuyoConfig->PosX, PuyoConfig->PuyoMeshWidth * y, PuyoConfig->PuyoMeshHeight * -z));
		
		Board[z][y] = {PuyoCol, FixedPuyoMeshActor};
	}
	// ShowArray();
}

void AStagePawn::ShowArray()
{
	TArray<FString> Array;
	for(int32 i = 0; i < Board.Num(); ++i)
	{
		FString LineName;
		for(int32 j = 0; j < Board[i].Num(); ++j)
		{
			LineName += FString::FromInt(Board[i][j].Puyo);
		}
		Array.Add(LineName);
	}
	UE_LOG(LogTemp, Log, TEXT("Array is: %s"), *FString::Join(Array, TEXT("\n")));
	
}



void AStagePawn::SpawnPuyoMesh()
{
	PuyoMeshClass = GetWorld()->SpawnActor<AMorphPuyoMesh>(AMorphPuyoMesh::StaticClass());
}



//Check freefall
bool AStagePawn::CheckFall()
{
	FallingPuyoArray.Empty();
	bIsFalling = false;

	//check from bottom to top
	for(int32 z = PuyoConfig->StageRows - 2; z >= 0; z--)
	{
		Line = Board[z];
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
				cell = Board[z][y];
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

void AStagePawn::CheckAndMorphPuyo(int32 y, int32 z)
{
	if(y < 0 || y > PuyoConfig->StageCols || z < 0 || z > PuyoConfig->StageRows)
	{
		return;
	}
	AMorphPuyoActor* FixedPuyoMeshActor = Board[z][y].PuyoMeshActor;
	//initialize morph
	FixedPuyoMeshActor->BackToNormal();
	
	//check 4 directions
	TArray<TArray<int32>> NeighborPuyosPos = {{1,0},{-1,0},{0,1},{0,-1}};
	TArray<EMorphState> MorphEventArray = {right, left, up, down};
	TArray<EMorphState> MorphEventOpoArray = {left, right, down, up};
	int32 PuyoCol = Board[z][y].Puyo;
	for(int32 i = 0; i < NeighborPuyosPos.Num(); ++i)
	{
		int32 NeighborY = y + NeighborPuyosPos[i][0];
		int32 NeighborZ = z + NeighborPuyosPos[i][1];
		if(NeighborY >= 0 && NeighborY < PuyoConfig->StageCols && NeighborZ >= 0 && NeighborZ < PuyoConfig->StageRows)
		{
			if(Board[NeighborZ][NeighborY].Puyo == PuyoCol)
			{
				//if neighbor puyo is same color, morph it
				FixedPuyoMeshActor->MorphPuyo(MorphEventArray[i]);
				Board[NeighborZ][NeighborY].PuyoMeshActor->MorphPuyo(MorphEventOpoArray[i]);
			}
		}
	}
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
		//CheckAndMorphPuyo(int32(PuyoMeshActor->GetActorLocation().Y / PuyoConfig->PuyoMeshWidth), int32(PosZ / PuyoConfig->PuyoMeshHeight));
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
		Puyo = this->Board[z][y];
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
			cell = this->Board[dz][dy];
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
		CheckAndMorphPuyo(info.Y, info.Z);
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
		MorphPuyoMesh->DestroyMorphPuyo(info.Cell.PuyoMeshActor);
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
				MorphPuyoMesh->DestroyMorphPuyo(Board[z][y].PuyoMeshActor);
				Board[z][y] = {0, nullptr};
			}
		}
	}
}



