// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_EnableRagdoll.generated.h"

/**
 *  This Anim notify just should set new CollisionProfileName and physic simulation
 */
UCLASS()
class TPSSHOOTER_API UAnimNotify_EnableRagdoll : public UAnimNotify
{
	GENERATED_BODY()
public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation);
};
