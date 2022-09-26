// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Engine/StaticMeshActor.h"
#include "PuyoConfigActor.h"
#include "StagePawn.h"
#include "PuyoMesh.h"
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
	

private:
	
	FPuyoStatus PuyoStatus;
	FKeyStatus KeyStatus;
	APuyoConfigActor* PuyoConfig;
	AStagePawn* StagePawn;
	
	//int32 PuyoColors;
	int32 CenterPuyoColor;
	int32 MovablePuyoColor;
	int32 GroundFrame;

	APuyoMesh* PuyoMeshActor;
	AStaticMeshActor* CenterPuyoActor;
	AStaticMeshActor* MovablePuyoActor;
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
	void Initialize(APuyoConfigActor* PuyoConfig);
    
	UFUNCTION()
	bool CreateNewPuyo();
	void SetPuyoPosition();
    bool Falling(bool bDownPressed);
    FString Playing(int32 frame);
    bool Moving(int32 frame);
    bool Rotating(int32 frame);
    void Fix();
    void Batankyu();
};



