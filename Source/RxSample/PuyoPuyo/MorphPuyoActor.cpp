// Fill out your copyright notice in the Description page of Project Settings.


#include "MorphPuyoActor.h"

// Sets default values
AMorphPuyoActor::AMorphPuyoActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	USceneComponent* DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	RootComponent = DefaultSceneRoot;

	MorphPuyoMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MorphPuyoMesh"));
	USkeletalMesh* MorphPuyoMeshAsset = LoadObject<USkeletalMesh>(NULL, TEXT("/Game/Mesh/Puyo/blue/blue.blue"));
	MorphPuyoMesh->SetSkeletalMesh(MorphPuyoMeshAsset);
	MorphPuyoMesh->SetupAttachment(RootComponent);
}

void AMorphPuyoActor::MorphPuyo(EMorphState state)
{
	switch (state)
	{
	case right:
		MorphRight();
		break;
	case left:
		MorphLeft();
		break;
	case up:
		MorphUp();
		break;
	case down:
		MorphDown();
		break;
	}
}


