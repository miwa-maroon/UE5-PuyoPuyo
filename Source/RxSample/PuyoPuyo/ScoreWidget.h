// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ScoreWidget.generated.h"

/**
 * 
 */


UCLASS()
class RXSAMPLE_API UScoreWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent)
	void SetScoreText(int32 Score);

	UFUNCTION(BlueprintImplementableEvent)
	void SetGameOverText();

	UFUNCTION(BlueprintImplementableEvent)
	void HideGameOverText();

	UFUNCTION(BlueprintImplementableEvent)
	void TurnOnLeftColor();

	UFUNCTION(BlueprintImplementableEvent)
	void TurnOnRightColor();

	UFUNCTION(BlueprintImplementableEvent)
	void TurnOnUpColor();

	UFUNCTION(BlueprintImplementableEvent)
	void TurnOnDownColor();

	UFUNCTION(BlueprintImplementableEvent)
	void TurnOffLeftColor();

	UFUNCTION(BlueprintImplementableEvent)
	void TurnOffRightColor();

	UFUNCTION(BlueprintImplementableEvent)
	void TurnOffUpColor();

	UFUNCTION(BlueprintImplementableEvent)
	void TurnOffDownColor();
};
