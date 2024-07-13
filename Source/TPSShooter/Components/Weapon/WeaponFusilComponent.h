// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "WeaponFusilComponent.generated.h"

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
	void Shot(FVector ShotStart, FVector ShotDirection, AController* Controller);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "Muzle attributes")
	float FiringRange = 5000.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Muzle attributes | Damage")
	float DamageAmount = 20.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Muzle attributes | VFX")
	// Particle system for firing
	UNiagaraSystem* MuzzleFlashFX;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Muzle attributes | VFX")
	// Particle system for firing
	UNiagaraSystem* TraceFX;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Muzle attributes | Decals")
	FDecalInfo DefaultDecalInfo;
};
