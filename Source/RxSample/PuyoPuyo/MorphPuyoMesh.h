// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MorphPuyoActor.h"
#include "PuyoMesh.h"
#include "Animation/SkeletalMeshActor.h"
#include "MorphPuyoMesh.generated.h"

/**
 * 
 */
UCLASS()
class RXSAMPLE_API AMorphPuyoMesh : public APuyoMesh
{
	GENERATED_BODY()

public:
	AMorphPuyoActor* GetMorphPuyo(int32 colorIndex);
	void DestroyMorphPuyo(AMorphPuyoActor* InMorphPuyo);
private:
	TArray<FString> m_MorphPuyoColors = {"purple", "blue", "green", "yellow", "red"};

	UPROPERTY(EditAnywhere)
	FString PathMorphPuyoBP = "/Game/Blueprints/BP_PuyoActor.BP_PuyoActor_C";

	FString PathMorphPuyo = "/Game/Mesh/Puyo";

	AMorphPuyoActor* BP_MorphPuyoActor;
};
