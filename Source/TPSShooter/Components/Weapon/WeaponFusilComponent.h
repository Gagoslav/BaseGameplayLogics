// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "WeaponFusilComponent.generated.h"


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
		
};
