// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PuyoConfigActor.generated.h"

UCLASS()
class RXSAMPLE_API APuyoConfigActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APuyoConfigActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Puyo")
	int32 PuyoMeshWidth = 100; // = 1meter
	int32 PuyoMeshHeight = 100; // = 1meter

	int32 StageCols = 6;
	int32 StageRows = 12;

	float FreeFallingSpeed = 100.0f;
	float ErasePuyoNum = 4;

	
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Puyo")
	int32 PuyoColors = 4;
	float PlayerFallingSpeed = 10.0f;
	float PlayerDownSpeed = 10.0f;
	int32 PlayerGroundFrame = 20;
	int32 PlayerMoveFrame = 10;
	int32 PlayerRotateFrame = 10;

	float PosX = 0.0f;
	float InitY = 0.0f;
	float InitZ = 100.0f; //PuyoMeshHeight * StageRows * 0.5f + 100.0f

};
