// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MorphPuyoActor.generated.h"

UENUM()
enum EMorphState
{
	right,
	left,
	up,
	down,
};

UCLASS()
class RXSAMPLE_API AMorphPuyoActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMorphPuyoActor();

	void MorphPuyo(EMorphState state);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Puyo")
	USkeletalMeshComponent* MorphPuyoMesh;

	UFUNCTION(BlueprintImplementableEvent, Category = "Puyo")
	void MorphUp();

	UFUNCTION(BlueprintImplementableEvent, Category = "Puyo")
	void MorphDown();

	UFUNCTION(BlueprintImplementableEvent, Category = "Puyo")
	void MorphRight();

	UFUNCTION(BlueprintImplementableEvent, Category = "Puyo")
	void MorphLeft();

	UFUNCTION(BlueprintImplementableEvent, Category = "Puyo")
	void BackToNormal();
};
