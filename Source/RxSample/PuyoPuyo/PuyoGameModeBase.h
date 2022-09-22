// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Rx.h"
#include "PuyoPlayerController.h"
#include "PuyoMesh.h"
#include "StagePawn.h"
#include "PuyoPlayerController.h"
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

	void SubscribeTest();
private:
	void InitializeGame(APuyoConfigActor* Config);
	void Loop();

	FString mode;
	int frame;
	int combinationCount = 0;

	AStagePawn* StagePawn;
	APuyoPlayerController* PuyoPlayerController;
};
