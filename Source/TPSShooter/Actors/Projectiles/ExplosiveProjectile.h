// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Projectiles/TPSProjectile.h"
#include "ExplosiveProjectile.generated.h"

// This class is the same projectile (inherited) with UExplosiveComponent

class UExplosionComponent;

UCLASS()
class TPSSHOOTER_API AExplosiveProjectile : public ATPSProjectile
{
	GENERATED_BODY()
public:
	AExplosiveProjectile();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UExplosionComponent* ExplosionComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Explosion")
	float DetonationTime = 2.0f;

	virtual void OnProjectileLaunched() override;

private:

	AController* GetController();
	void OnDetonationTimerElapsed();
	FTimerHandle DetonationTimer;
	
};
