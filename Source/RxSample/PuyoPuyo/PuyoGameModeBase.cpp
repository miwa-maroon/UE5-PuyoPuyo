// Fill out your copyright notice in the Description page of Project Settings.


#include "PuyoGameModeBase.h"


#include "PuyoPlayerController.h"
#include "Kismet/GameplayStatics.h"


APuyoGameModeBase::APuyoGameModeBase()
{
	PlayerControllerClass = APuyoPlayerController::StaticClass();
	DefaultPawnClass = AStagePawn::StaticClass();
	PlayerStateClass = APuyoPlayState::StaticClass();
	HUDClass = APuyoHUD::StaticClass();
}

void APuyoGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
	PuyoMesh = GetWorld()->SpawnActor<APuyoMesh>(APuyoMesh::StaticClass());
}




