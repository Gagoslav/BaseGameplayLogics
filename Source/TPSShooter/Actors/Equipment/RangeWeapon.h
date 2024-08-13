// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Equipment/EquipableItems.h"
#include "RangeWeapon.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnAmmoChanged, int32);
DECLARE_MULTICAST_DELEGATE(FOnReloadComplete);

UENUM(BlueprintType)
enum class EWeaponFireMode : uint8
{
	Single,
	FullAutomate
};

UENUM(BlueprintType)
enum class EReloadType : uint8
{
	FullClip,
	ByBullet // For shotgun
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

	void StartReload();
	void EndReload(bool bIsSuccess);

	FTransform GetForegripTransform() const;

	inline float GetAimFOV() const { return AimFOV; }
	inline float GetAimMovementMaxSpeed() const { return AimMovementMaxSpeed; }

	// Function related to ammunition
	inline int32 GetAmmo() const { return CurrentAmmo; }
	inline int32 GetMaxAmmo() const { return MaxAmmo; }
	void SetAmmo(int32 NewAmmo);
	bool CanShoot()const;

	EAmunitionType GetAmmoType() const;

	FOnAmmoChanged OnAmmoChangedEvent;
	FOnReloadComplete OnReloadCompleteEvent;

	virtual EReticleType GetReticleType() const override;

protected:
	virtual void BeginPlay() override;

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
	UAnimMontage* WeaponReloadMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animations | Weapon")
	// FullClip reload type adds ammo only when the whole animation is successfully played
	// ByBullet reload type requires section "EndReload" in character reload animation 
	EReloadType ReloadType = EReloadType::FullClip;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animations | Weapon")
	// Animation of character designed for particular weapon, is stored in Weapon class and is played from pointer to character
	UAnimMontage* CharacterFireMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animations | Weapon")
	UAnimMontage* CharacterReloadMontage;

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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon | Parameters | Ammo", meta = (ClampMin = 1, UIMin = 1))
	EAmunitionType AmmoType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon | Parameters | Ammo", meta = (ClampMin = 1, UIMin = 1))
	int32 MaxAmmo = 30;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon | Parameters | Ammo")
	bool bAutoReload = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Reticle")
	EReticleType AimReticleType = EReticleType::Default;
	
private:

	int32 CurrentAmmo = 0;
	float GetCurrentBulletSpreadAngle() const;

	bool bIsAiming = false;
	bool bIsReloading = false;
	bool bIsFiring = false;

	float PlayAnimMontage(UAnimMontage* AnimMontage);
	void StopAnimMontage(UAnimMontage* AnimMontage, float BlendOutTime = 0.0f);
	float GetShotTimerInterval();
	void MakeShot();

	void OnShotTimerElapsed();


	FTimerHandle ShotTimer;
	FTimerHandle ReloadTimer;
};
