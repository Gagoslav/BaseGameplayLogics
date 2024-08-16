// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Weapon/MeleeHitRegistrator.h"
#include "BaseData/BaseEnums.h"
#include "Utils/TPSTraceUtils.h"
#include "Subsystems/TPSDebugSubsystem.h"
#include "Kismet/GameplayStatics.h"

UMeleeHitRegistrator::UMeleeHitRegistrator()
{
	PrimaryComponentTick.bCanEverTick = true;
	SphereRadius = 5.0f;
	SetCollisionProfileName(CollisionProfileNoCollision);
}

void UMeleeHitRegistrator::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	// if we have hit anything process hit registration
	if (bIsHitregistrationEnabled)
	{
		ProcessHitRegistration();
	}
	// Cach the Previous component hit result as we should get hit direction within ProcessHitRegistration
	PreviousComponentHitLocation = GetComponentLocation();
}

void UMeleeHitRegistrator::ProcessHitRegistration()
{
	FVector CurrentLocation = GetComponentLocation();
	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	// Ignore owning character
	QueryParams.AddIgnoredActor(GetOwner());

#if ENABLE_DRAW_DEBUG 
	UTPSDebugSubsystem* DebugSubsystem = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<UTPSDebugSubsystem>();
	bool bIsDebugEnabled = DebugSubsystem->IsCategoryEnabled(DebugCategoryMelee);
#else
	bool bIsDebugEnabled = false;
#endif

	// Use our Custom trace utils to scan if we have hit anything 
	bool bHasHit = TPSTraceUtils::SweepSphereSingleByChanel(
		GetWorld(),
		HitResult,
		PreviousComponentHitLocation,
		CurrentLocation,
		GetScaledSphereRadius(),
		ECC_Melee,
		QueryParams,
		FCollisionResponseParams::DefaultResponseParam,
		bIsDebugEnabled,
		5.0f
	);
	if (bHasHit)
	{
		// If we have hit, then get its direction and execute delegate
		FVector Direction = (CurrentLocation - PreviousComponentHitLocation).GetSafeNormal();
		if (OnMeleeHitRegistered.IsBound())
		{
			OnMeleeHitRegistered.Broadcast(HitResult, Direction);
		}
	}
}

void UMeleeHitRegistrator::SetIsHitRegistrationEnabled(bool bIsEnabled)
{
	bIsHitregistrationEnabled = bIsEnabled;
}

