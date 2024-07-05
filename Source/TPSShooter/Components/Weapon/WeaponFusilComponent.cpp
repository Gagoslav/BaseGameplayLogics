// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Weapon/WeaponFusilComponent.h"
#include "BaseData/BaseEnums.h"
#include "Engine/DamageEvents.h"
#include "DrawDebugHelpers.h"
#include "Subsystems/TPSDebugSubsystem.h"
#include "Kismet/GameplayStatics.h"

void UWeaponFusilComponent::Shot(FVector ShotStart, FVector ShotDirection, AController* Controller)
{
	// Initialize necessary params for line trace
	
	// Location from where we start shooting
	FVector MuzleLocation = GetComponentLocation();
	FVector ShotEnd = ShotStart + FiringRange * ShotDirection;
	FHitResult ShotResult;

#if ENABLE_DRAW_DEBUG
	// According to the latest approach we need to encapsulate draw debug system in respective game instance's subsystem
	UTPSDebugSubsystem* DebugSubsystem = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<UTPSDebugSubsystem>();

	// Gets permission to draw debug or not to draw debug from game instance subsystem (and we may pass the permission through realized console command to game instance subsystem)
	bool bIsDebugEnabled = DebugSubsystem->IsCategoryEnabled(DebugCategoryRangeWeapon);
	float DrawTime = 3.0f;

#else 
bool bIsDebugEnabled = false;

#endif

	// Implement line trace (it's used in most shooter games for firing mechanics)
	if (GetWorld()->LineTraceSingleByChannel(ShotResult, ShotStart, ShotEnd, ECC_Bullet))
	{
		ShotEnd = ShotResult.ImpactPoint;

		if (bIsDebugEnabled)
		{
			DrawDebugSphere(GetWorld(), ShotEnd, 10.0f, 24, FColor::Red, false, DrawTime);
		}

		AActor* HitActor = ShotResult.GetActor();
		if (IsValid(HitActor))
		{
			HitActor->TakeDamage(DamageAmount,FDamageEvent {}, Controller, GetOwner());
		}
	}
	if (bIsDebugEnabled)
	{
		DrawDebugLine(GetWorld(), MuzleLocation, ShotEnd, FColor::Red, false, DrawTime, 0, 3.0f);
	}
	
	
}
