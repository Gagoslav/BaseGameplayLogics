// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Equipment/EquipableItems.h"
#include "RangeWeapon.generated.h"

UENUM(BlueprintType)
enum class EWeaponFireMode : uint8
{
	Single,
	FullAutomate
};


class UAnimMontage;

UCLASS(Blueprintable)
class TPSSHOOTER_API ARangeWeapon : public AEquipableItems
{
	GENERATED_BODY()
public:

	ARangeWeapon();

	void StartFire();
	
	void StopFire();

	FTransform GetForegripTransform() const;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	// Weapon's visual representation
	class USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	// Weapon's muzzle is a distinct component that is responsible for damage amount and fire distance
	class UWeaponFusilComponent* WeaponMuzle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animations | Weapon")
	// Animation of weapon's skeletal mesh
	UAnimMontage* WeaponFireMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animations | Weapon")
	// Animation of character designed for particular weapon, is stored in Weapon class and is played from pointer to character
	UAnimMontage* CharacterFireMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon | Parameters", meta = (ClampMin = 1.0f, UIMin = 1.0f))
	// Rate of fire in rounds per minute
	float RateOfFire = 600.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon | Parameters")
	EWeaponFireMode WeaponFireMode = EWeaponFireMode::Single;
	
private:
	float PlayAnimMontage(UAnimMontage* AnimMontage);
	float GetShotTimerInterval();
	void MakeShot();


	FTimerHandle ShotTimer;
};
