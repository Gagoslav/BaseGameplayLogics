// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "SetMeleeHitRegEnabled.generated.h"

/**
 * 
 */
UCLASS()
class TPSSHOOTER_API USetMeleeHitRegEnabled : public UAnimNotify
{
	GENERATED_BODY()

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Melee Weapon")
	bool bIsHitRegistrationEnabled = false;
	
};
