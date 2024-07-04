// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/LedgeDetectorComponent.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "BaseData/BaseEnums.h"
#include "Utils/TPSTraceUtils.h"
#include "TPSGameInstance.h"
#include "Subsystems/TPSDebugSubsystem.h"
#include "Kismet/GameplayStatics.h"




// Called when the game starts


void ULedgeDetectorComponent::BeginPlay()
{
	// IDK why it cannot detect Super::BeginPlay
	Super::BeginPlay();
	// Make an assert that only character may be an owner of LedgeDetectorComponent, othervise crash the editor with corresponding warning
	checkf(GetOwner()->IsA<ACharacter>(), TEXT("ULedgeDetectorComponent::BeginPlay() Only character can use the ledge detection component"));
	// Initialize cached owner character
	InCharacter = StaticCast<ACharacter*>(GetOwner());
}

bool ULedgeDetectorComponent::DetectLedge(OUT FLedgeDescription& LedgeDescription)
{
	UCapsuleComponent* OwnerCapsuleComponent = InCharacter->GetCapsuleComponent();

	// Base conditions for all sweep testes
	FCollisionQueryParams QueryParams;

	// We should set the common query parameters of sweep cast to ignore the owning character 
	QueryParams.bTraceComplex = true;
	QueryParams.AddIgnoredActor(GetOwner());


#if ENABLE_DRAW_DEBUG
	// According to the latest approach we need to encapsulate draw debug system in respective game instance's subsystem
	UTPSDebugSubsystem* DebugSubsystem = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<UTPSDebugSubsystem>();

	// Gets permission to draw debug or not to draw debug from game instance subsystem (and we may pass the permission through realized console command to game instance subsystem)
	bool bIsDebugEnabled = DebugSubsystem->IsCategoryEnabled(DebugCategoryLedgeDetection);
	float DrawTime = 3.0f;

#else 
	bool bIsDebugEnabled = false;

#endif

	// Offset of capsule's bottom part just not to coinside with floor
	float BottomZOffset = 2.0f;

	// World Location of the bottom part of our character's Capsule component (with some offset)
	FVector CharacterBottom = InCharacter->GetActorLocation() - (OwnerCapsuleComponent->GetScaledCapsuleHalfHeight() - BottomZOffset) * FVector::UpVector;


	/* 1 Do Forward check */ 
	// Necessary fields for capsule sweep test
	float ForwardCheckCapsuleRadius = OwnerCapsuleComponent->GetScaledCapsuleRadius();
	float ForwardCheckCapsuleHalfHeight = (MaximumLedgeHeight - MinimumLedgeHeight) * 0.5;// We want to climb on the height that is more than the one of our character

	FHitResult ForwardCheckHitResult;

	// Differs from Character capsulelocation by its Z component, as we wanna start mantle logic implementation from Minimum ledge heigt
	// and we also want to check height higher than our character
	FVector ForwardStartLocation = CharacterBottom + (MinimumLedgeHeight + ForwardCheckCapsuleHalfHeight) * FVector::UpVector;

	// Just add to Start location some offset along Character's forward vector
	FVector ForwardEndLocation = ForwardStartLocation + InCharacter->GetActorForwardVector() * ForwardCheckDistance;

	

	// Call wrapper function SweepCapsuleSingleByChanel as we initialized it in TSPTraceUtils namespace, that will make draw debug
	if (!TPSTraceUtils::SweepCapsuleSingleByChanel(GetWorld(), ForwardCheckHitResult, ForwardStartLocation, ForwardEndLocation, ForwardCheckCapsuleRadius, ForwardCheckCapsuleHalfHeight, FQuat::Identity, ECC_Climbing, QueryParams, FCollisionResponseParams::DefaultResponseParam, bIsDebugEnabled, DrawTime))
	{
		return false;
	}
	

	/* 2 Do Downward check */
	FHitResult DownwardCheckHitResult;
	float DownwardCheckSphereRadius = OwnerCapsuleComponent->GetScaledCapsuleRadius();

	// Depth of downward sphere cast above the ledge as we don't want 
	// the starting point of cast to be the very edge of the ledge, where forward cast hit
	float DownwardCheckDepthOffset = 10.0f;
	FVector DownwardStartLocation = ForwardCheckHitResult.ImpactPoint - ForwardCheckHitResult.ImpactNormal * DownwardCheckDepthOffset;

	// Correct the height of Downward sweep cast starting point
	DownwardStartLocation.Z = CharacterBottom.Z + MaximumLedgeHeight + DownwardCheckSphereRadius;
	FVector DownwardEndLocation(DownwardStartLocation.X, DownwardStartLocation.Y, CharacterBottom.Z);

	// Call wrapper function SweepSphereSingleByChanel as we initialized it in TSPTraceUtils namespace, that will make draw debug
	if (!TPSTraceUtils::SweepSphereSingleByChanel(GetWorld(), DownwardCheckHitResult, DownwardStartLocation, DownwardEndLocation, DownwardCheckSphereRadius, ECC_Climbing, QueryParams, FCollisionResponseParams::DefaultResponseParam, bIsDebugEnabled, DrawTime))
	{
		return false;
	}


	/* 3 Do Overlap check(to make sure we have no impediments at the place we want to get) */
	// Overlap capsule is just the same as character has
	float OverlapCapsuleRadius = OwnerCapsuleComponent->GetScaledCapsuleRadius();
	float OverlapCapsuleHalfHeight = OwnerCapsuleComponent->GetScaledCapsuleHalfHeight();
	float OverlapCapsuleFloorOffset = 2.0f;
	FVector OverlapLocation = DownwardCheckHitResult.ImpactPoint + (OverlapCapsuleHalfHeight+ OverlapCapsuleFloorOffset) * FVector::UpVector;

	if (TPSTraceUtils::OverlapCapsuleBlockingByProfile(GetWorld(), OverlapLocation, OverlapCapsuleRadius, OverlapCapsuleHalfHeight, FQuat::Identity, CollisionProfilePawn, QueryParams, bIsDebugEnabled, DrawTime))
	{
		return false;
	}
	

	LedgeDescription.Location = OverlapLocation; // As overlap location includes the difference by Z axis 
	LedgeDescription.Rotation = (ForwardCheckHitResult.ImpactNormal * FVector(-1.0f, 1.0f, 0.0f)).ToOrientationRotator();
	LedgeDescription.LedgeNormal = ForwardCheckHitResult.ImpactNormal;
	return true;
}




