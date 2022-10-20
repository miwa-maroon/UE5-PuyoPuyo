// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PuyoConfigActor.h"
#include "Engine/StaticMeshActor.h"
#include "GameFramework/SpringArmComponent.h"
#include "PuyoMesh.h"
#include "MorphPuyoMesh.h"
#include "MorphPuyoActor.h"
#include "Camera/CameraComponent.h"
#include "StagePawn.generated.h"

USTRUCT()
struct FPuyoMemoryData
{
	GENERATED_BODY()
	int32 Puyo;
	AMorphPuyoActor* PuyoMeshActor;
};

USTRUCT()
struct FPuyoFallData
{
	GENERATED_BODY()

	FPuyoMemoryData PuyoMemoryData;
	int32 PosZ;
	int32 DstZ;
	bool Falling;
};

USTRUCT()
struct FPuyoEraseData
{
	GENERATED_BODY()
	int32 Y;
	int32 Z;
	FPuyoMemoryData Cell;
	
};



UCLASS()
class RXSAMPLE_API AStagePawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AStagePawn();

public:	

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	
	bool bIsFalling;
	int32 EraseStartFrame;
	

	UStaticMeshComponent* StageMesh;
	//UCameraComponent* Camera;
	//USpringArmComponent* SpringArm;
	
	APuyoConfigActor* PuyoConfig;

	AMorphPuyoActor* PuyoMeshActor;
	AActor* PuyoMeshClass;


	
	TArray<FPuyoFallData> FallingPuyoArray;
	TArray<FPuyoEraseData> ErasingPuyoArray;
	
	TArray<FPuyoMemoryData> ZeroArray;
	TArray<FPuyoMemoryData> Line;
	FPuyoMemoryData Puyo;
	FPuyoMemoryData cell;
	
	

public:
	int32 PuyoCount;

	TArray<TArray<FPuyoMemoryData>> Board;
	AMorphPuyoMesh* MorphPuyoMesh;
	
	virtual void Initialize(APuyoConfigActor* Config);
	bool CheckFall();
	void CheckAndMorphPuyo(int32 y, int32 z);
	bool Fall();
	TArray<int32> CheckErase(int32 InStartFrame);
	bool Erasing(int32 frame);
	void DestroyAllPuyo();

	virtual void SetPuyo(int32 PuyoCol, int32 y, int32 z);
	virtual void ShowArray();

	UFUNCTION(BlueprintCallable)
	virtual void SpawnPuyoMesh();
	
};

