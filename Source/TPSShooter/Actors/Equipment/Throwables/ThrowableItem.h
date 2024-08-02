// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Equipment/EquipableItems.h"
#include "ThrowableItem.generated.h"

/**
 * This item at the moment just have one function that spawns a projectile sets its owner (character) and launches it
 * this is a base class for all types of throwable items
 */
UCLASS(Blueprintable)
class TPSSHOOTER_API AThrowableItem : public AEquipableItems
{
	GENERATED_BODY()
public:

	// Main Function of Throwable item
	void Throw();

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Throwable")
	TSubclassOf<class ATPSProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Throwable", meta = (UIMin = -90.0f, UIMax = 90.0f, ClampMin = -90.0f, ClampMax = 90.0f))
	float ThrowAngle;
};
