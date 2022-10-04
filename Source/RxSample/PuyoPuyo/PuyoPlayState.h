// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "PuyoPlayState.generated.h"

/**
 * 
 */
UENUM()
enum EStateEnum
{
	start,
	checkFall,
	fall,
	checkErase,
	erase,
	newPuyo,
	playing,
	moving,
	rotating,
	fix,
	gameover,
	lobby
};

UCLASS()
class RXSAMPLE_API APuyoPlayState : public APlayerState
{
	GENERATED_BODY()

public:
	//state
	EStateEnum state;

	void SetState(EStateEnum newState);
    EStateEnum GetState();

	//score
	int32 playerScore;
	int32 playerCombo;
	TArray<int32> RensaBonus = {0, 8, 16, 32, 64, 128, 160, 192, 224, 256, 288, 320, 352, 384, 416, 448, 480, 512, 544, 576, 608, 640, 672};
	TArray<int32> ChainBonus = {0, 0, 0, 0, 2, 3, 4, 5, 6, 7, 10, 10};
	TArray<int32> ColorBonus = {0, 0, 3, 6, 12, 24};

	void SetScore(int32 InScore);
	int32 GetScore();
	void UpdateScore(int32 InScore);
	void CalcScore(int32 InRensa, int32 InChain, int32 InColor);
	void SetCombo(int32 InCombo);
	int32 GetCombo();
	void UpdateCombo(int32 InCombo);
};


