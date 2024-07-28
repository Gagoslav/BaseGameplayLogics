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

void UWeaponFusilComponent::Shot(FVector ShotStart, FVector ShotDirection, AController* Controller, float SpreadAngle)
{
	// How many shots per one fire action we do
	for (int i = 0; i < BulletsPerShot; i++)
	{
		// Add spread offset from screen's sentral point
		ShotDirection += GetBulletSpreadOffset(FMath::RandRange(0.0f, SpreadAngle), ShotDirection.ToOrientationRotator());

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
				// Special type of UDamageEvent
				FPointDamageEvent DamageEvent;
				DamageEvent.HitInfo = ShotResult;
				DamageEvent.ShotDirection = ShotDirection;
				DamageEvent.DamageTypeClass = DamageTypeClass;

				// Apply damage
				HitActor->TakeDamage(DamageAmount, DamageEvent, Controller, GetOwner());
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

}

FVector UWeaponFusilComponent::GetBulletSpreadOffset(float Angle, FRotator ShotRotation) const
{
	float SpreadSize = FMath::Tan(Angle); // Get normalized length of Spread vector (counter located cathetus || tangent)
	float RotationAngle = FMath::RandRange(0.0f, 2 * PI); // random number from 0 to 360, like pick random point on circumference

	float SpreadY = FMath::Cos(RotationAngle);
	float SpreadZ = FMath::Sin(RotationAngle);
	FVector Result = (ShotRotation.RotateVector(FVector::RightVector) * SpreadY + ShotRotation.RotateVector(FVector::UpVector) * SpreadZ) * SpreadSize; // Find offset Direction and time it by ratio 
	//UE_LOG(LogTemp, Warning, TEXT("%f %f %f"),Result.X, Result.Y, Result.Z );

	return Result;
}
