// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TitleWidget.h"
#include "ScoreWidget.h"
#include "GameFramework/HUD.h"
#include "PuyoHUD.generated.h"


/**
 * 
 */
UCLASS()
class RXSAMPLE_API APuyoHUD : public AHUD
{
	GENERATED_BODY()
public:
	TSubclassOf<class UUserWidget> TitleWidgetClass;
	TSubclassOf<class UUserWidget> ScoreWidgetClass;

	UPROPERTY(EditAnywhere)
	FString PathTitleWBP = "/Game/Widget/WBP_TitleWidget.WBP_TitleWidget_C";

	UPROPERTY(EditAnywhere)
	FString PathScoreWBP = "/Game/Widget/WBP_ScoreWidget.WBP_ScoreWidget_C";

	UPROPERTY(VisibleInstanceOnly, Category="Runtime")
	UTitleWidget* TitleWidget;
	
	UPROPERTY(VisibleInstanceOnly, Category="Runtime")
	UScoreWidget* ScoreWidget;

	
	void ShowTitleWidget();
	void ShowScoreWidget();
	void HideTitleWidget();
	void HideScoreWidget();

	void ShowScoreText(int32 InScore);
	void ShowGameOverText();
	void HideGameOverText();

	bool IsTitleWidgetViewport();
	bool IsScoreWidgetViewport();

	bool GetIsLobby();
};
