// Fill out your copyright notice in the Description page of Project Settings.


#include "PuyoPlayState.h"

void APuyoPlayState::SetState(EStateEnum newState)
{
	state = newState;
}

EStateEnum APuyoPlayState::GetState()
{
	return state;
}

void APuyoPlayState::SetScore(int32 InScore)
{
	playerScore = InScore;
}

int32 APuyoPlayState::GetScore()
{
	return playerScore;
}

void APuyoPlayState::UpdateScore(int32 InScore)
{
	playerScore += InScore;
}

void APuyoPlayState::CalcScore(int32 InRensa, int32 InChain, int32 InColor)
{
	InRensa = FMath::Min(InRensa, RensaBonus.Num() - 1);
	InChain = FMath::Min(InChain, ChainBonus.Num() - 1);
	InColor = FMath::Min(InColor, ColorBonus.Num() - 1);
	int32 Scale = RensaBonus[InRensa] * ChainBonus[InChain] * ColorBonus[InColor];
	if(Scale == 0)
	{
		Scale = 1;
	}
	UpdateScore(Scale * InChain * 10);
}

void APuyoPlayState::SetCombo(int32 InCombo)
{
	playerCombo = InCombo;
}

int32 APuyoPlayState::GetCombo()
{
	return playerCombo;
}

void APuyoPlayState::IncrementCombo()
{
	playerCombo++;
}