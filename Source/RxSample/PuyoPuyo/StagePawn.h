// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PuyoConfigActor.h"
#include "Engine/StaticMeshActor.h"
#include "GameFramework/SpringArmComponent.h"
#include "PuyoMesh.h"
#include "Camera/CameraComponent.h"
#include "StagePawn.generated.h"

USTRUCT()
struct FPuyoMemoryData
{
	GENERATED_BODY()
	int32 Puyo;
	AStaticMeshActor* PuyoMeshActor;
};

USTRUCT()
struct FPuyoFallData
{
	GENERATED_BODY()

	FPuyoMemoryData PuyoMemoryData;
	float PosY;
	float PosZ;
	float DstZ;
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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	bool bIsFalling;
	int32 EraseStartFrame;

	UStaticMeshComponent* StageMesh;
	UCameraComponent* Camera;
	USpringArmComponent* SpringArm;
	
	APuyoConfigActor* PuyoConfig;

	AStaticMeshActor* PuyoMeshActor;
	APuyoMesh* PuyoMesh;

	
	TArray<FPuyoFallData> FallingPuyoArray;
	TArray<FPuyoEraseData> ErasingPuyoArray;
	
	
	void ShowArray();
	

public:
	int32 PuyoCount;

	TArray<TArray<FPuyoMemoryData>> Board;
	
	void Initialize(APuyoConfigActor* Config);
	bool CheckFall();
	bool Fall();
	TArray<int32> CheckErase(int32 InStartFrame);
	bool Erasing(int32 frame);

	void SetPuyo(int32 Puyo, int32 y, int32 z);
};

