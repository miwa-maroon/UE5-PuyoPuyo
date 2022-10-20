// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CameraSwitchingActor.generated.h"

UCLASS()
class RXSAMPLE_API ACameraSwitchingActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACameraSwitchingActor();


public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* TitleCamera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* PlayCamera;

	UFUNCTION(BlueprintImplementableEvent)
	void SwitchTitleCamera();

	UFUNCTION(BlueprintImplementableEvent)
	void SwitchPlayCamera();
};
