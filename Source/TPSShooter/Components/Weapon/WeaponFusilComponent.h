// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "WeaponFusilComponent.generated.h"

// As some types of weapon will use different registration types 
// Like we need a projectile for a rifle
UENUM(BlueprintType)
enum class EHitRegistrationType :uint8
{
	HitScan,
	Projectile
};

USTRUCT(BlueprintType)
// Structre to define general data about decals
struct FDecalInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Decal Info")
	UMaterialInterface* DecalMaterial;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Decal Info")
	FVector DecalSize = FVector(5.0f, 5.0f, 5.0f);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Decal Info")
	float DecalLifetime = 10.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Decal Info")
	float DecalFadeoutTime = 5.0f;
};

class UNiagaraSystem;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TPSSHOOTER_API UWeaponFusilComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Function that will implement firing action
	void Shot(FVector ShotStart, FVector ShotDirection, float SpreadAngle);

	

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "Muzle attributes")
	float FiringRange = 5000.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Muzle attributes", meta = (ClampMin=1, UIMin = 1))
	int32 BulletsPerShot = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Muzle attributes | Damage")
	float DamageAmount = 20.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Muzle attributes | Damage")
	TSubclassOf<class UDamageType> DamageTypeClass;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Muzle attributes | VFX")
	// Particle system for firing
	UNiagaraSystem* MuzzleFlashFX;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Muzle attributes | VFX")
	// Particle system for bullet trace
	UNiagaraSystem* TraceFX;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Muzle attributes | Decals")
	FDecalInfo DefaultDecalInfo;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Muzle attributes | Hit Registration")
	EHitRegistrationType HitRegistration = EHitRegistrationType::HitScan;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Muzle attributes | Hit Registration", meta = (EditCondition = "HitRegistration == EHitRegistrationType::Projectile")) // this is like when the condition is wrong it will be grey
	TSubclassOf<class ATPSProjectile> ProjectileClass;

private:
	FVector GetBulletSpreadOffset(float Angle, FRotator ShotRotation) const;

	bool HitScan(const FVector & ShotStart, OUT FVector& ShotEnd, float DrawTime, const FVector & ShotDirection);
	void LaunchProjectile(const FVector& LaunchStart, const FVector& LaunchDirection);

	APawn* GetOwningPawn() const;
	AController* GetController() const;

	UFUNCTION()
	void ProcessHit(const FHitResult& HitResult, const FVector& Direction);
};
