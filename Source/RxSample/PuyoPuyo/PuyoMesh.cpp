// Fill out your copyright notice in the Description page of Project Settings.


#include "PuyoMesh.h"

// Sets default values
APuyoMesh::APuyoMesh()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APuyoMesh::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APuyoMesh::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APuyoMesh::Initialize()
{
}

AStaticMeshActor* APuyoMesh::GetPuyo(int32 index)
{
	FString SMName = "/Game/Mesh/Puyo/" + m_PuyoColors[index];
	UStaticMesh* Puyo = LoadObject<UStaticMesh>(nullptr, *SMName);
	AStaticMeshActor* PuyoActor = GetWorld()->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(), FVector(0, 0, 0), FRotator(0, 0, 0));
	PuyoActor->SetMobility(EComponentMobility::Movable);
	UStaticMeshComponent* PuyoMesh = PuyoActor->GetStaticMeshComponent();
	if(PuyoMesh)
	{
		PuyoMesh->SetStaticMesh(Puyo);
	}
	return PuyoActor;
}

void APuyoMesh::DestroyPuyo(AStaticMeshActor* puyo)
{
	puyo->Destroy();
}

void APuyoMesh::PrepareBatankyu()
{
}

void APuyoMesh::Batankyu()
{
}

