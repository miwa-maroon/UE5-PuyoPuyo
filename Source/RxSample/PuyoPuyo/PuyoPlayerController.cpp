// Fill out your copyright notice in the Description page of Project Settings.


#include "PuyoPlayerController.h"

#include "Kismet/GameplayStatics.h"

APuyoPlayerController::APuyoPlayerController()
{
}

void APuyoPlayerController::BeginPlay()
{
	Super::BeginPlay();

	//Get Actors
	StagePawn = Cast<AStagePawn>(GetPawn());
	PuyoConfig = Cast<APuyoConfigActor>(UGameplayStatics::GetActorOfClass(GetWorld(), APuyoConfigActor::StaticClass()));
	PuyoMeshActor = Cast<APuyoMesh>(UGameplayStatics::GetActorOfClass(GetWorld(), APuyoMesh::StaticClass()));
	PlayerState = Cast<APuyoPlayState>(StagePawn->GetPlayerState());
	PuyoHUD = Cast<APuyoHUD>(GetHUD());
	CameraSwitchingActor = Cast<ACameraSwitchingActor>(UGameplayStatics::GetActorOfClass(GetWorld(), ACameraSwitchingActor::StaticClass()));
	
	//initialize this class
	KeyStatus = { false, false, false, false };
	
	EnableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	InputComponent->BindAction("Left", IE_Pressed, this, &APuyoPlayerController::PressLeft);
	InputComponent->BindAction("Left", IE_Released, this, &APuyoPlayerController::ReleaseLeft);
	InputComponent->BindAction("Right", IE_Pressed, this, &APuyoPlayerController::PressRight);
	InputComponent->BindAction("Right", IE_Released, this, &APuyoPlayerController::ReleaseRight);
	InputComponent->BindAction("Down", IE_Pressed, this, &APuyoPlayerController::PressDown);
	InputComponent->BindAction("Down", IE_Released, this, &APuyoPlayerController::ReleaseDown);
	InputComponent->BindAction("Up", IE_Pressed, this, &APuyoPlayerController::PressUp);
	InputComponent->BindAction("Up", IE_Released, this, &APuyoPlayerController::ReleaseUp);

	frame = 0;

	
	//initialize StagePawn
	StagePawn->Initialize(PuyoConfig);

	
	//initialize PlayerState
	PlayerState->SetState(lobby);
	PlayerState->SetCombo(0);
	PlayerState->SetScore(0);

	
	//initialize PuyoHUD
	PuyoHUD->ShowTitleWidget();

	//initialize CameraSwitchingActor
	CameraSwitchingActor->SwitchTitleCamera();
}

//main function
void APuyoPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	
	switch (PlayerState->GetState())
	{
	case lobby:
		if(!PuyoHUD->IsTitleWidgetViewport())
		{
			PuyoHUD->ShowTitleWidget();
		}
		
		//OnPress Start Button
		//bIsLobby = true
		if(!PuyoHUD->GetIsLobby())
		{
			PlayerState->SetState(start);
			CameraSwitchingActor->SwitchPlayCamera();
			if(PuyoHUD->IsTitleWidgetViewport())
			{
				PuyoHUD->HideTitleWidget();
				PuyoHUD->ShowScoreWidget();
				
				PuyoHUD->HideGameOverText();
				PlayerState->SetScore(0);
				PuyoHUD->ShowScoreText(PlayerState->GetScore());
			}
		}
		break;
		
	case start:
		PlayerState->SetState(checkFall);
		break;
		
	case checkFall:
		if(StagePawn->CheckFall())
		{
			PlayerState->SetState(fall);
		}else
		{
			PlayerState->SetState(checkErase);
		}
		break;
		
	case fall:
		if(!StagePawn->Fall())
		{
			PlayerState->SetState(checkErase);
		}
		break;
		
	case checkErase:
		//EraseInfo = {ErasingPuyoArray.Num(), ErasedPuyoColorMap.Num()}
		//if there is no puyo to erase, EraseInfo = {0}
		EraseInfo = StagePawn->CheckErase(frame);
		if(EraseInfo[0])
		{
			PlayerState->SetState(erase);
			PlayerState->IncrementCombo();
			PlayerState->CalcScore(PlayerState->GetCombo(), EraseInfo[0], EraseInfo[1]);
			PuyoHUD->ShowScoreText(PlayerState->GetScore());
		}else
		{
			if(StagePawn->PuyoCount == 0 && PlayerState->GetCombo() > 0)
			{
				PlayerState->UpdateScore(3600);
				PuyoHUD->ShowScoreText(PlayerState->GetScore());
			}
			PlayerState->SetCombo(0);
			PlayerState->SetState(newPuyo);
		}
		break;
		
	case erase:
		if(!StagePawn->Erasing(frame))
		{
			PlayerState->SetState(checkFall);
		}
		break;
		
	case newPuyo:
		if(!CreateNewPuyo())
		{
			PlayerState->SetState(gameOver);
		}else
		{
			PlayerState->SetState(playing);
		}
		break;
		
	case playing:
		action = Playing(frame);
		PlayerState->SetState(action);
		break;
		
	case moving:
		if(!Moving(frame))
		{
			PlayerState->SetState(playing);
		}
		break;

	case rotating:
		if(!Rotating(frame))
		{
			PlayerState->SetState(playing);
		}
		break;
		
	case fix:
		Fix();
		PlayerState->SetState(checkFall);
		break;
		
	case gameOver:
		PuyoHUD->ShowGameOverText();
		StagePawn->DestroyAllPuyo();
		if(PuyoHUD->IsScoreWidgetViewport())
		{
			PuyoHUD->HideScoreWidget();
			PuyoHUD->ShowTitleWidget();
		}
		PlayerState->SetState(lobby);
		CameraSwitchingActor->SwitchTitleCamera();
		break;
	}
	frame++;
}

bool APuyoPlayerController::CreateNewPuyo()
{
	//Check if you can put new puyo
	if(StagePawn->Board[0][2].Puyo != 0)
	{
		return false;
	}

	//set the puyo color
	int32 PuyoColors = FMath::Max(1, FMath::Min(5, PuyoConfig->PuyoColors));
	CenterPuyoColor = FMath::Floor(FMath::FRand() * PuyoColors) + 1;
	MovablePuyoColor = FMath::Floor(FMath::FRand() * PuyoColors) + 1;

	//generate new puyo
	CenterPuyoActor = PuyoMeshActor->GetPuyo(CenterPuyoColor);
	MovablePuyoActor = PuyoMeshActor->GetPuyo(MovablePuyoColor);

	//Be child to Stage
	//set initial location of puyo
	PuyoStatus.y = 2; //location of center puyo second from left
	PuyoStatus.z = -1; //Comes out just above the top of the screen.
	PuyoStatus.centerY = 2 * PuyoConfig->PuyoMeshWidth;
	PuyoStatus.centerZ = -1 * PuyoConfig->PuyoMeshHeight;
	PuyoStatus.dy = 0; //Relative position of the moving Puyo: the moving puyo is in the upward direction
	PuyoStatus.dz = -1;
	PuyoStatus.rotation = 90.0f; //Angle of moving Puyo is 90 degrees (upward)
	//ground time is 0
	GroundFrame = 0;
	//draw puyo
	SetPuyoPosition();
	return true;
	
}

void APuyoPlayerController::SetPuyoPosition()
{
	CenterPuyoActor->SetActorLocation(FVector(PuyoConfig->PosX, PuyoStatus.centerY, -PuyoStatus.centerZ));
	int32 y = PuyoStatus.centerY + FMath::Cos(PuyoStatus.rotation * (float)PI / 180.0f) * PuyoConfig->PuyoMeshWidth;
	int32 z = PuyoStatus.centerZ - FMath::Sin(PuyoStatus.rotation * (float)PI / 180.0f) * PuyoConfig->PuyoMeshHeight;
	MovablePuyoActor->SetActorLocation(FVector(PuyoConfig->PosX, y, -z));
}

bool APuyoPlayerController::Falling(bool bDownPressed)
{
	
	//check whether there is a block under current location or not
	bool bIsBlocked = false;
	int32 y = PuyoStatus.y;
	int32 z = PuyoStatus.z;
	int32 dy = PuyoStatus.dy;
	int32 dz = PuyoStatus.dz;
	
	if(z + 1 >=PuyoConfig->StageRows || IsValidIndex(StagePawn->Board, y, z + 1) || (z + dz + 1 >= 0 && (z + dz + 1 >= PuyoConfig->StageRows || IsValidIndex(StagePawn->Board, y + dy, z + dz + 1))))
    {
        bIsBlocked = true;
    }
	
	if(!bIsBlocked)
	{
		
		//if there is no block under, fall puyo. processing of free fall
		PuyoStatus.centerZ += PuyoConfig->FreeFallingSpeed;
		
		if(bDownPressed)
		{
			//if press down key, fall faster
			PuyoStatus.centerZ += PuyoConfig->PlayerDownSpeed;
		}
		if(FMath::Floor(PuyoStatus.centerZ / PuyoConfig->PuyoMeshHeight) != z)
		{
			//puyo goes over one grid, check again
			//if press down key, add acore
			if(bDownPressed)
			{
				//Score.addScore(1);
			}
			z += 1;
			PuyoStatus.z = z;
			
			if(z + 1 >= PuyoConfig->StageRows || IsValidIndex(StagePawn->Board, y, z + 1) || (z + dz + 1 >= 0 && (z + dz + 1 >= PuyoConfig->StageRows || IsValidIndex(StagePawn->Board, y + dy, z + dz + 1))))
            {
                bIsBlocked = true;
            }
			
			
			if(!bIsBlocked)
			{
				//go over the border, but there is no block under. so keep falling
				GroundFrame = 0;
				return false;
			}else
			{
				//go over ther border, hit the block. so stop falling and adjust the position
				PuyoStatus.centerZ = z * PuyoConfig->PuyoMeshHeight;
				GroundFrame = 1;
				return false;
			}
		}else
		{
			//no problem during free fall, so keep falling
			GroundFrame = 0;
			return false;
		}
	}
	if(GroundFrame == 0)
	{
		GroundFrame = 1;
		return false;
	}else
	{
		GroundFrame++;
        if(GroundFrame > PuyoConfig->PlayerGroundFrame)
        {
        	return true;
        }
	}
	
	return false;
}

EStateEnum APuyoPlayerController::Playing(int32 InFrame)
{
	//check free fall
	//if pressed down key, fall faster
	if(Falling(KeyStatus.Down))
	{
		//if finished free fall, fix puyo
		SetPuyoPosition();
		return fix;
	}
	SetPuyoPosition();
	if(KeyStatus.Right || KeyStatus.Left)
	{
		//check right and left
		int32 cy = KeyStatus.Right ? 1 : -1;
		int32 y = PuyoStatus.y;
		int32 z = PuyoStatus.z;
		int32 my = y + PuyoStatus.dy;
		int32 mz = z + PuyoStatus.dz;
		//check if the block doesn't exist for the direction
		//check right and left from own position
		bool bCanMove = true;
		if(z < 0 || y + cy < 0 || y + cy >= PuyoConfig->StageCols || IsValidIndex(StagePawn->Board, y + cy, z))
        {
            if(z >= 0)
            {
            	bCanMove = false;
            }
        }

		
		if(mz < 0 || my + cy < 0 || my + cy >= PuyoConfig->StageCols || IsValidIndex(StagePawn->Board, my + cy, mz))
		{
			if(mz >= 0)
			{
				bCanMove = false;
			}
		}
		
		//if puyo doesn't ground, check the block under the puyo
		if(GroundFrame == 0)
		{
			if(z + 1 < 0 || y + cy < 0 || y + cy >= PuyoConfig->StageCols || IsValidIndex(StagePawn->Board, y + cy, z + 1))
            {
                if(z + 1 >= 0)
                {
                	bCanMove = false;
                }
            }
			
			if(mz + 1 < 0 || my + cy < 0 || my + cy >= PuyoConfig->StageCols || IsValidIndex(StagePawn->Board, my + cy, mz + 1))
            {
                if(mz + 1 >= 0)
                {
                	bCanMove = false;
                }
            }
			

		}

		if(bCanMove)
		{
			//set moving info and turn to moving state
			ActionStartFrame = InFrame;
			MoveSource = y * PuyoConfig->PuyoMeshWidth;
			MoveDestination = (y + cy) * PuyoConfig->PuyoMeshWidth;
			PuyoStatus.y += cy;
			return moving;
		}
	}else if(KeyStatus.Up)
	{
		//check rotation
		//check later if can rotate
		int32 y = PuyoStatus.y;
		int32 z = PuyoStatus.z;
		int32 my = y + PuyoStatus.dy;
		int32 mz = z + PuyoStatus.dz;
		int32 rotation = int32(PuyoStatus.rotation);
		bool bCanRotate = true;

		int32 cy = 0;
		int32 cz = 0;
		if(rotation == 0)
		{
			//can rotate 100% from right to top, do nothing
		}else if(rotation == 90)
		{
		
			//check first if the block is on the left when rotate from up to left cuz need to move right
            if(z + 1 < 0 || y - 1 < 0 || y - 1 >= PuyoConfig->StageCols || IsValidIndex(StagePawn->Board, y - 1, z + 1))
			{
				if(z + 1 >= 0)
				{
					cy = 1;
				}
			}
                
			
			
			//check if the block is on the right when move to right cuz can't move
			if(cy == 1)
			{
				
				if(z + 1 < 0 || y + 1 < 0 || y + 1 >= PuyoConfig->StageCols || IsValidIndex(StagePawn->Board, y + 1, z + 1))
                {
                    if(z + 1 >= 0)
                    {
                    	//the block exists, can't rotate
                    	bCanRotate = false;
                    }
                }
				

			}
		}else if(rotation == 180)
		{
			
			//When turning from left to bottom, if there is a block under you or to your lower left, pull one up. Check down first.
            if(z + 2 < 0 || z + 2 >= PuyoConfig->StageRows || IsValidIndex(StagePawn->Board, y, z + 2))
            {
                if(z + 2 >= 0)
                {
                	//block exists, pull one up
                	cz = -1;
                }
            }
			
			
			//check left and right
            if(z + 2 < 0 || z + 2 >= PuyoConfig->StageRows || y - 1 < 0 || IsValidIndex(StagePawn->Board, y - 1, z + 2))
			{
				if(z + 2 >= 0)
				{
					//block exists, pull one up
					cz = -1;
				}
			}
                
			
			
		}else if(rotation == 270)
		{
			
			//check if the block is on the right when rotate from bottom to right cuz need to move left
            if(z + 1 < 0 || y + 1 < 0 || y + 1 >= PuyoConfig->StageCols || IsValidIndex(StagePawn->Board, y + 1, z + 1))
            {
                if(z + 1 >= 0)
                {
                	//block exists, move to left one block
                	cy = -1;
                }
            }
			
			
			//check if the block is on the left when move to left cuz can't move
			if(cy == -1)
			{
				
				if(z + 1 < 0 || y - 1 < 0 || y - 1 >= PuyoConfig->StageCols || IsValidIndex(StagePawn->Board, y - 1, z + 1))
                {
                    if(z + 1 >= 0)
                    {
                    	//the block exists, can't rotate
                    	bCanRotate = false;
                    }
                }
				
			}
		}

		if(bCanRotate)
		{
			//When you need to move up, you raise it all at once.
			if(cz == -1)
			{
				if(GroundFrame > 0)
				{
					//if grounded, pull one up
					PuyoStatus.z -= 1;
					GroundFrame = 0;
				}
				PuyoStatus.centerZ = PuyoStatus.z * PuyoConfig->PuyoMeshHeight;
			}
			//set moving info and turn to moving state
			ActionStartFrame = InFrame;
			RotateBeforeCenterY = y * PuyoConfig->PuyoMeshHeight;
			RotateAfterCenterY = (y + cy) * PuyoConfig->PuyoMeshHeight;
			RotateFromRotation = PuyoStatus.rotation;

			//set the next state
			PuyoStatus.y += cy;
			int32 DistRotation = (int32(PuyoStatus.rotation) + 90) % 360;
			TArray<TArray<int32>> DRotateArray = { {1, 0}, {0, -1}, {-1, 0}, {0, 1} };
			TArray<int32> DCombi = DRotateArray[DistRotation / 90];
			PuyoStatus.dy = DCombi[0];
			PuyoStatus.dz = DCombi[1];
			return rotating;
		}
	}
	return playing;
}


bool APuyoPlayerController::Moving(int32 InFrame)
{
	//make puyo fall during moving
	bool bFalling = Falling(KeyStatus.Down);
	float ratio = FMath::Min(1, (InFrame - ActionStartFrame) / float(PuyoConfig->PlayerMoveFrame));
	PuyoStatus.centerY = FMath::Lerp(MoveSource, MoveDestination, ratio);
	SetPuyoPosition();
	if(ratio == 1)
	{
		return false;
	}
	return true;
}

bool APuyoPlayerController::Rotating(int32 InFrame)
{
	//make puyo fall during rotating
	bool bFalling = Falling(KeyStatus.Down);
	float ratio = FMath::Min(1, (InFrame - ActionStartFrame) / float(PuyoConfig->PlayerRotateFrame));
	PuyoStatus.centerY = FMath::Lerp(RotateBeforeCenterY, RotateAfterCenterY, ratio);
	PuyoStatus.rotation = FMath::Lerp(RotateFromRotation, RotateFromRotation + 90.0f, ratio);
	SetPuyoPosition();
	if(ratio == 1)
	{
		PuyoStatus.rotation = float((int32(PuyoStatus.rotation)) % 360);
		return false;
	}
	return true;
}

void APuyoPlayerController::Fix()
{
	//place current puyo on the stage
	int32 y = PuyoStatus.y;
	int32 z = PuyoStatus.z;
	int32 dy = PuyoStatus.dy;
	int32 dz = PuyoStatus.dz;
	if(z >= 0)
	{
		//erase the puyo out of stage
		StagePawn->SetPuyo(CenterPuyoColor, y, z);
		StagePawn->PuyoCount++;
	}
	if(z + dz >= 0)
	{
		//erase the puyo out of stage
		StagePawn->SetPuyo(MovablePuyoColor, y + dy, z + dz);
		StagePawn->PuyoCount++;
	}
	//destroy puyo that is prepared for operating
	CenterPuyoActor->Destroy();
	MovablePuyoActor->Destroy();
	CenterPuyoActor = nullptr;
	MovablePuyoActor = nullptr;
}

void APuyoPlayerController::Batankyu()
{
}

template <typename TYPE>
bool APuyoPlayerController::IsValidIndex(TArray<TYPE> Array, int32 y, int32 z)
{
	bool bFirstValid = Array.IsValidIndex(z);
	bool bIsPuyo = false;
	if(bFirstValid)
	{
		bool bSecondValid = Array[z].IsValidIndex(y);
		if(bSecondValid)
		{
			if(Array[z][y].Puyo)
			{
				bIsPuyo = true;
			}
		}
	}
	return bIsPuyo;
}



void APuyoPlayerController::PressLeft()
{
	KeyStatus.Left = true;
}

void APuyoPlayerController::PressRight()
{
	KeyStatus.Right = true;
}

void APuyoPlayerController::PressDown()
{
	KeyStatus.Down = true;
}

void APuyoPlayerController::PressUp()
{
	KeyStatus.Up = true;
}

void APuyoPlayerController::ReleaseLeft()
{
	KeyStatus.Left = false;
}

void APuyoPlayerController::ReleaseRight()
{
	KeyStatus.Right = false;
}

void APuyoPlayerController::ReleaseDown()
{
	KeyStatus.Down = false;
}

void APuyoPlayerController::ReleaseUp()
{
	KeyStatus.Up = false;
}
