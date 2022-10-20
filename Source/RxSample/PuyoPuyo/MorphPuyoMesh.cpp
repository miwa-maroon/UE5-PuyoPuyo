// Fill out your copyright notice in the Description page of Project Settings.


#include "MorphPuyoMesh.h"

AMorphPuyoActor* AMorphPuyoMesh::GetMorphPuyo(int32 colorIndex)
{
	TSubclassOf<AActor> MorphPuyoClass = TSoftClassPtr<AActor>(FSoftObjectPath(*PathMorphPuyoBP)).LoadSynchronous();
	UE_LOG(LogTemp, Log, TEXT("MorphPuyoClass: %s"), *MorphPuyoClass->GetName());
	if(MorphPuyoClass != nullptr)
	{
		BP_MorphPuyoActor = GetWorld()->SpawnActor<AMorphPuyoActor>(MorphPuyoClass);
		USkeletalMeshComponent* SkeletalMeshComponent = Cast<USkeletalMeshComponent>(BP_MorphPuyoActor->GetComponentByClass(USkeletalMeshComponent::StaticClass()));
		UE_LOG(LogTemp, Log, TEXT("SkeletalMeshComponent: %s"), *SkeletalMeshComponent->GetName());
		FString PathMorphPuyoMesh = PathMorphPuyo + "/" + m_MorphPuyoColors[colorIndex - 1] + "/" + m_MorphPuyoColors[colorIndex - 1] + "." + m_MorphPuyoColors[colorIndex - 1];
        USkeletalMesh* SkeletalMesh = Cast<USkeletalMesh>(TSoftObjectPtr<USkeletalMesh>(FSoftObjectPath(*PathMorphPuyoMesh)).LoadSynchronous());
		UE_LOG(LogTemp, Log, TEXT("SkeletalMesh: %s"), *SkeletalMesh->GetName());
		SkeletalMeshComponent->SetSkeletalMesh(SkeletalMesh);
	}
	return BP_MorphPuyoActor;
}

void AMorphPuyoMesh::DestroyMorphPuyo(AMorphPuyoActor* InMorphPuyo)
{
	InMorphPuyo->Destroy();
}
