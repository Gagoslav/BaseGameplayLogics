// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Weapon/WeaponFusilComponent.h"
#include "BaseData/BaseEnums.h"
#include "Engine/DamageEvents.h"
#include "DrawDebugHelpers.h"
#include "Subsystems/TPSDebugSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h" // Niagara is a distinct plugin and we need to connect (add) it in Build.cs
#include "NiagaraComponent.h"
#include "Components/DecalComponent.h"

void UWeaponFusilComponent::Shot(FVector ShotStart, FVector ShotDirection, AController* Controller)
{
	// Initialize necessary params for line trace
	
	// Location from where we start shooting
	FVector MuzzleLocation = GetComponentLocation();
	FVector ShotEnd = ShotStart + FiringRange * ShotDirection;
	FHitResult ShotResult;
	//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Blue, FString::Printf(TEXT("%f, %f, %f"), ShotDirection.X, ShotDirection.Y, ShotDirection.Z));

	if (MuzzleFlashFX != nullptr)
	{
		// If we have Niagara system then spawn it at pistol's muzzle
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), MuzzleFlashFX, MuzzleLocation, GetComponentRotation()); 
	}


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
			// Apply damage
			HitActor->TakeDamage(DamageAmount,FDamageEvent {}, Controller, GetOwner());
		}

		UDecalComponent* DecalComponent = UGameplayStatics::SpawnDecalAtLocation(GetWorld(), DefaultDecalInfo.DecalMaterial, DefaultDecalInfo.DecalSize, ShotResult.ImpactPoint, ShotResult.ImpactNormal.ToOrientationRotator());
		if (IsValid(DecalComponent))
		{
			DecalComponent->SetFadeScreenSize(0.001f); // Fadeout will be applied if decal take 1/1000 percent of screen. otherwise on long distance it will fade automatically
			DecalComponent->SetFadeOut(DefaultDecalInfo.DecalLifetime, DefaultDecalInfo.DecalFadeoutTime);
		}
	}

	if (TraceFX != nullptr)
	{
		// If we have Niagara system then spawn it at pistol's muzzle
		UNiagaraComponent* TraceFXComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), TraceFX, MuzzleLocation, GetComponentRotation());
		// This function may initialize user defined value by its name that is set in Particle system in editor 
		TraceFXComponent->SetVectorParameter(FXParamTraceEnd, ShotEnd); 
	}

	if (bIsDebugEnabled)
	{
		DrawDebugLine(GetWorld(), MuzzleLocation, ShotEnd, FColor::Red, false, DrawTime, 0, 3.0f);
	}
	
	
}
