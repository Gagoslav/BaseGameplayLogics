// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/ExplosionComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"

void UExplosionComponent::Explode(AController* Controller)
{

	TArray<AActor*> IgnoredActors;
	IgnoredActors.Add(GetOwner());
	UGameplayStatics::ApplyRadialDamageWithFalloff(
		GetWorld(), 
		MaxDamage,
		MinDamage,
		GetComponentLocation(),
		InnerRadius,
		OuterRadius,
		DamageFalloff,
		DamageTypeClass,
		IgnoredActors,
		GetOwner(),
		Controller, 
		ECC_Visibility
	);
	if (ExplosionVFX != nullptr)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionVFX, GetComponentLocation()); // Is a Cascade particle system
	}
	
	if (OnExplosion.IsBound())
	{
		OnExplosion.Broadcast();
	}
}
