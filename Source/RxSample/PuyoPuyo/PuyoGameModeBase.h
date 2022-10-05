// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PuyoMesh.h"
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
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

	APuyoMesh* PuyoMesh;
};
