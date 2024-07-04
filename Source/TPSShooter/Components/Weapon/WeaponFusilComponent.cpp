// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Weapon/WeaponFusilComponent.h"
#include "BaseData/BaseEnums.h"
#include "Engine/DamageEvents.h"
#include "DrawDebugHelpers.h"

void UWeaponFusilComponent::Shot(FVector ShotStart, FVector ShotDirection, AController* Controller)
{
	FVector MuzleLocation = GetComponentLocation();
	
	FVector ShotEnd = ShotStart + FiringRange * ShotDirection;

	FHitResult ShotResult;
	DrawDebugLine(GetWorld(), MuzleLocation, ShotEnd, FColor::Red, false, 1.0f, 0, 3.0f);

	if (GetWorld()->LineTraceSingleByChannel(ShotResult, ShotStart, ShotEnd, ECC_Bullet))
	{
		ShotEnd = ShotResult.ImpactPoint;
		
		DrawDebugSphere(GetWorld(), ShotEnd, 10.0f, 24, FColor::Red, false, 1.0f);
		AActor* HitActor = ShotResult.GetActor();

		if (IsValid(HitActor))
		{
			HitActor->TakeDamage(DamageAmount,FDamageEvent {}, Controller, GetOwner());
		}
	}
	
	
}
