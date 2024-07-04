// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Equipment/EquipableItems.h"
#include "RangeWeapon.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class TPSSHOOTER_API ARangeWeapon : public AEquipableItems
{
	GENERATED_BODY()
public:

	ARangeWeapon();

	void Fire();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UWeaponFusilComponent* WeaponMuzle;
	
};
