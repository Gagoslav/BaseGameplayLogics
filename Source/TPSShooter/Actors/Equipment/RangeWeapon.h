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

	void StartAim();
	void StopAim();

	FTransform GetForegripTransform() const;

	inline float GetAimFOV() const { return AimFOV; }
	inline float GetAimMovementMaxSpeed() const { return AimMovementMaxSpeed; }

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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon | Parameters", meta = (ClampMin = 0.0f, UIMin = 0.0f, ClampMax = 2.0f, UIMax = 2.0f))
	// Bullet spread half angle in degrees
	float SpreadAngle = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon | Parameters | Aiming", meta = (ClampMin = 0.0f, UIMin = 0.0f, ClampMax = 2.0f, UIMax = 2.0f))
	// Aim spread angle is always less than spread angle during simple fire
	float AimSpreadAngle = 0.25f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon | Parameters | Aiming", meta = (ClampMin = 0.0f, UIMin = 0.0f))
	// The character moves always slower when he is aiming and this parameter also depends on the type of weapon, that's why it's described in weapon class
	float AimMovementMaxSpeed = 200.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon | Parameters | Aiming", meta = (ClampMin = 0.0f, UIMin = 0.0f, ClampMax = 120.0f, UIMax = 120.0f))
	// Field of view of character is always less while aiming
	float AimFOV = 60.0f;
	
private:
	float GetCurrentBulletSpreadAngle() const;


	bool bIsAiming;
	float PlayAnimMontage(UAnimMontage* AnimMontage);
	float GetShotTimerInterval();
	void MakeShot();

	FVector GetBulletSpreadOffset (float Angle, FRotator ShotRotation) const;


	FTimerHandle ShotTimer;
};
