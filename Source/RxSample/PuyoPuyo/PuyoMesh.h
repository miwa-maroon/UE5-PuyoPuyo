// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/StaticMeshActor.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "PuyoMesh.generated.h"

UCLASS()
class RXSAMPLE_API APuyoMesh : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APuyoMesh();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Initialize();
	AStaticMeshActor* GetPuyo(int32 index);
	void DestroyPuyo(AStaticMeshActor* puyo);
	void PrepareBatankyu();
	void Batankyu();

private:
	TArray<FString> m_PuyoColors = {"Puyo_Red", "Puyo_Blue", "Puyo_Green", "Puyo_Yellow", "Puyo_Purple"};
};
