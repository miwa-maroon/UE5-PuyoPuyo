// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Engine/StaticMeshActor.h"
#include "PuyoConfigActor.h"
#include "PuyoHUD.h"
#include "StagePawn.h"
#include "PuyoMesh.h"
#include "PuyoPlayState.h"
#include "CameraSwitchingActor.h"
#include "ScoreTextThreeDActor.h"
#include "PuyoPlayerController.generated.h"

/**
 * 
 */
USTRUCT()
struct FPuyoStatus
{
	GENERATED_BODY()
	int32 y;
	int32 z;
	int32 centerY;
	int32 centerZ;
	int32 dy;
	int32 dz;
	float rotation;
};

USTRUCT()
struct FKeyStatus
{
	GENERATED_BODY()
	bool Left;
	bool Right;
	bool Down;
	bool Up;
};

UCLASS(Config=Game)
class RXSAMPLE_API APuyoPlayerController : public APlayerController
{
	GENERATED_BODY()
protected:
	APuyoPlayerController();
public:
	virtual void BeginPlay() override;
	virtual void PlayerTick(float DeltaTime) override;

private:
	
	FPuyoStatus PuyoStatus;
	FKeyStatus KeyStatus;
	
	APuyoConfigActor* PuyoConfig;
	AStagePawn* StagePawn;
	APuyoPlayState* PlayerState;
	APuyoHUD* PuyoHUD;
	ACameraSwitchingActor* CameraSwitchingActor;
	AScoreTextThreeDActor* ScoreTextThreeDActor;

	int32 frame;
	EStateEnum action;
	TArray<int32> EraseInfo;
	
	int32 CenterPuyoColor;
	int32 MovablePuyoColor;
	int32 GroundFrame;

	APuyoMesh* PuyoMeshActor;
	AMorphPuyoActor* CenterPuyoActor;
	AMorphPuyoActor* MovablePuyoActor;
	UStaticMesh* PuyoMesh;

	int32 ActionStartFrame;
	float MoveSource;
	float MoveDestination;
	float RotateBeforeCenterY;
	float RotateAfterCenterY;
	float RotateFromRotation;

	template <typename TYPE>
	bool IsValidIndex(TArray<TYPE> Array, int32 y, int32 z);
	
	void PressLeft();
	void PressRight();
	void PressDown();
	void PressUp();

	void ReleaseLeft();
	void ReleaseRight();
	void ReleaseDown();
	void ReleaseUp();

public:
	bool CreateNewPuyo();
	void SetPuyoPosition();
    bool Falling(bool bDownPressed);
    EStateEnum Playing(int32 InFrame);
    bool Moving(int32 InFrame);
    bool Rotating(int32 InFrame);
    void Fix();
    void Batankyu();
};



