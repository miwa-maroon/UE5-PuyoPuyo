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

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Puyo")
	float FreeFallingSpeed = 1.0f;
	float ErasePuyoNum = 4;

	
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Puyo")
	int32 PuyoColors = 4;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Puyo")
	float PlayerFallingSpeed = 10.0f;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Puyo")
	float PlayerDownSpeed = 10.0f;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Puyo")
	int32 PlayerGroundFrame = 5;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Puyo")
	int32 PlayerMoveFrame = 20;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Puyo")
	int32 PlayerRotateFrame = 20;

	float PosX = 0.0f;
	float InitY = 248.0f;
	float InitZ = -480.0f;

};
