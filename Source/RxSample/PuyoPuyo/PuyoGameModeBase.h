// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Rx.h"
#include "PuyoPlayerController.h"
#include "PuyoMesh.h"
#include "StagePawn.h"
#include "PuyoPlayerController.h"
#include "PuyoPlayState.h"
#include "PuyoHUD.h"
#include "TitleWidget.h"
#include "ScoreWidget.h"
#include "PuyoGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class RXSAMPLE_API APuyoGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	APuyoGameModeBase();

protected:
	rxcpp::subjects::subject<bool> Test;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

	void SubscribeTest();
private:
	void InitializeGame(APuyoConfigActor* Config);
	void Loop();

	FString mode;
	int frame;
	float time;

	AStagePawn* StagePawn;
	APuyoPlayerController* PuyoPlayerController;
	APuyoMesh* PuyoMesh;
	APuyoConfigActor* PuyoConfigActor;
	


private:
	void InitializeScore();
	void UpdateScore(int32 InScore);
	void CalcScore(int32 InRensa, int32 InChain, int32 InColor);
	void ShowScoreText();
	void ShowGameOverText();
	void HideGameOverText();

	int32 Score = 0;
	int combinationCount = 0;
	TArray<int32> RensaBonus = {0, 8, 16, 32, 64, 128, 160, 192, 224, 256, 288, 320, 352, 384, 416, 448, 480, 512, 544, 576, 608, 640, 672};
	TArray<int32> ChainBonus = {0, 0, 0, 0, 2, 3, 4, 5, 6, 7, 10, 10};
	TArray<int32> ColorBonus = {0, 0, 3, 6, 12, 24};

    TSubclassOf<class UUserWidget> TitleWidgetClass;
	TSubclassOf<class UUserWidget> ScoreWidgetClass;

	UPROPERTY(VisibleInstanceOnly, Category="Runtime")
	UTitleWidget* TitleWidget;
	
    UPROPERTY(VisibleInstanceOnly, Category="Runtime")
    UScoreWidget* ScoreWidget;
};
