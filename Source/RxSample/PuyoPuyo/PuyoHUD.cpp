// Fill out your copyright notice in the Description page of Project Settings.


#include "PuyoHUD.h"

void APuyoHUD::ShowTitleWidget()
{
	TitleWidgetClass = TSoftClassPtr<UUserWidget>(FSoftObjectPath(*PathTitleWBP)).LoadSynchronous();
	if(TitleWidgetClass != nullptr)
	{
		TitleWidget = CreateWidget<UTitleWidget>(GetWorld(), TitleWidgetClass);
		if(TitleWidget)
		{
			TitleWidget->AddToViewport();
			TitleWidget->bIsLobby = true;
		}
	}
}

void APuyoHUD::ShowScoreWidget()
{
	ScoreWidgetClass = TSoftClassPtr<UUserWidget>(FSoftObjectPath(*PathScoreWBP)).LoadSynchronous();
	if(ScoreWidgetClass != nullptr)
	{
		ScoreWidget = CreateWidget<UScoreWidget>(GetWorld(), ScoreWidgetClass);
		if(ScoreWidget)
		{
			ScoreWidget->AddToViewport();
		}
	}
}

void APuyoHUD::HideTitleWidget()
{
	if(TitleWidget->IsInViewport())
	{
		TitleWidget->RemoveFromParent();
	}
}

void APuyoHUD::HideScoreWidget()
{
	if(ScoreWidget->IsInViewport())
	{
		ScoreWidget->RemoveFromParent();
	}
}

void APuyoHUD::ShowScoreText(int32 InScore)
{
	ScoreWidget->SetScoreText(InScore);
}

void APuyoHUD::ShowGameOverText()
{
	ScoreWidget->SetGameOverText();
}

void APuyoHUD::HideGameOverText()
{
	ScoreWidget->HideGameOverText();
}

bool APuyoHUD::IsTitleWidgetViewport()
{
	return TitleWidget->IsInViewport();
}

bool APuyoHUD::IsScoreWidgetViewport()
{
	return ScoreWidget->IsInViewport();
}

bool APuyoHUD::GetIsLobby()
{
	return TitleWidget->bIsLobby;
}
