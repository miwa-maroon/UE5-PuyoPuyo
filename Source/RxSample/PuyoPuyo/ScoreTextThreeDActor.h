// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ScoreTextThreeDActor.generated.h"

UCLASS()
class RXSAMPLE_API AScoreTextThreeDActor : public AActor
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent)
	void ShowScoreText(int32 Score);
};
