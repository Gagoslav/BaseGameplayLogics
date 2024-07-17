// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/TPSBaseCharacter.h"
#include "Components/BaseCharacterMovementComponent.h"
#include "Components/CharacterEquipmentComponent.h"
#include "Components/LedgeDetectorComponent.h"
#include "Components/CharacterAttributes.h"
#include "Curves/CurveVector.h"
#include "Actors/Interactive/Environment/Ladder.h"
#include "BaseData/BaseEnums.h"
#include "Engine/DamageEvents.h"
#include "Actors/Equipment/RangeWeapon.h"

// In initialization list we reset the character movement component of base class (ACharacter) to customly created one, using SetDefaultSubobjectClass
ATPSBaseCharacter::ATPSBaseCharacter(const FObjectInitializer& ObjectInitializer) 
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UBaseCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	
	// Initialize our custom character movement component
	BaseCharacterMovementComponent = StaticCast<UBaseCharacterMovementComponent*>(GetCharacterMovement());
	LedgeDetectorComponent = CreateDefaultSubobject<ULedgeDetectorComponent>(TEXT("LedgeDetector"));

	CharacterEquipmentComponent = CreateDefaultSubobject<UCharacterEquipmentComponent>(TEXT("CharacterEquipment"));
	CharacterAttributesComponent = CreateDefaultSubobject<UCharacterAttributes>(TEXT("CharacterAttributes"));

	// Make sure our 3rd person character's mesh will cast shadows
	GetMesh()->CastShadow = true;
	GetMesh()->bCastDynamicShadow = true;
}


void ATPSBaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Add (bind) a function to Multicast Delegate OnDeathEvent
	CharacterAttributesComponent->OnDeathEvent.AddUObject(this, &ATPSBaseCharacter::OnDeath);
}

void ATPSBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// Every tick check the sprint state
	TryChangeSprintState();
	
}

void ATPSBaseCharacter::StartSprint()
{
	// Change the "sprint requested" flag to true  
	bShouldSprint = 1;
	// Uncrouch if character is crouched
	if (bIsCrouched)
	{
		UnCrouch();
	}
}

void ATPSBaseCharacter::EndSprint()
{
	//Change the "sprint requested" flag to false
	bShouldSprint = 0;
}



void ATPSBaseCharacter::OnStartAimingInternal()
{
}

void ATPSBaseCharacter::OnStopAimingInternal()
{
}

void ATPSBaseCharacter::TryChangeSprintState()
{
	// checks if character can sprint and starts or ends sprinting accordingly
	if (bShouldSprint && !GetBaseCharacterMovement()->GetIsSprinting())
	{
		GetBaseCharacterMovement()->StartSprint();
		// Run event overridden in blueprints
		OnStartSprint();
	}
	if (!bShouldSprint && GetBaseCharacterMovement()->GetIsSprinting())
	{
		GetBaseCharacterMovement()->EndSprint();
		// Run event overridden in blueprints
		OnEndSprint();
	}
}

const FMantlingSettings& ATPSBaseCharacter::GetMantlingSettings(float LedgeHeight) const
{
	return LedgeHeight > LowMantleMaxHeight ? HighMantleSettings : LowMantleSettings;
}

void ATPSBaseCharacter::OnDeath()
{
	// Just sets movement mode to None, thus disabling movement
	GetBaseCharacterMovement()->DisableMovement();
	
	// Play animation of death from code 
	float Duration = PlayAnimMontage(OnDeathAnimMontage);
	if (Duration == 0.0f)
	{
		EnableRagdoll();
	}

}



void ATPSBaseCharacter::EnableRagdoll()
{
	// There is a moment, when we switch CollisionProfileName to Ragdoll, that enables query and physics, it can overweight the animation
	GetMesh()->SetCollisionProfileName(CollisionProfileRagdoll);
	GetMesh()->SetSimulatePhysics(true);
}

void ATPSBaseCharacter::ChangeCrouchState()
{
	// tries to change crouch state, is bound to the same input
	// checks the crouch state of character and Crouch or Uncrouch accordingly
	if (GetCharacterMovement()->IsCrouching())
	{
		UnCrouch();
	}
	else
	{
		Crouch();
	}
}

bool ATPSBaseCharacter::CanMantle() const
{
	return !GetBaseCharacterMovement()->IsOnLadder() && CharacterAttributesComponent->IsAlive();
}

void ATPSBaseCharacter::OnMantle(const FMantlingSettings& MantlingSettings, float MantlingAnimationStart)
{
	// Don't have any realization, override in derived class
}


void ATPSBaseCharacter::Mantle(bool bForce /*= false*/)
{
	// If character is not on ladder and is not forced to mantle then execute early return
	if (!(CanMantle() || bForce)) // the snippet inside braces may be considered as single logical expression (true() or false(!))
	{
		return;
	}
	
	FLedgeDescription LedgeDescription;
	if (LedgeDetectorComponent->DetectLedge(LedgeDescription)) // As DetectLedge methos takes in OUT LedgeDescription structure by reference, it initializes him by reference
	{
		// initializing mantling parameters
		FMantlingMovementParameters MantlingParams;

		MantlingParams.InitialLocation = GetActorLocation();
		MantlingParams.InitialRotation = GetActorRotation();
		MantlingParams.TargetLocation = LedgeDescription.Location;
		MantlingParams.TargetRotation = LedgeDescription.Rotation;

		//Get height the character is going to reach
		float MantlingHight = (MantlingParams.TargetLocation - MantlingParams.InitialLocation).Z;

		//Initialize mantling settings with getter, determining if it's a high mantle or low mantle
		const FMantlingSettings& MantlingSettings = GetMantlingSettings(MantlingHight);

		if (!IsValid(MantlingSettings.MantlingCurve))
		{
			return;
		}

		// declare auxiliary parameters to get curve vector duration
		float MinRange;
		float MaxRange;
		
		MantlingSettings.MantlingCurve->GetTimeRange(MinRange, MaxRange); // This method takes in OUT references of float/double types and as result initializes them, gets duration of curve
		MantlingParams.Duration = MaxRange - MinRange; // get duration of curve

		// Initialize MantlingCurve from character's mantling settings
		MantlingParams.MantlingCurve = MantlingSettings.MantlingCurve;
		
		

		//EXPLORE !!!
		//float StartTime = MantlingSettings.MaxHeightStartTime + (MantlingHight - MantlingSettings.MinHeight) / (MantlingSettings.MaxHeight - MantlingSettings.MinHeight) * (MantlingSettings.MaxHeightStartTime - MantlingSettings.MinHeightStartTime);
		FVector2D SourceRange(MantlingSettings.MinHeight, MantlingSettings.MaxHeight);
		FVector2D TargetRange(MantlingSettings.MinHeightStartTime, MantlingSettings.MaxHeightStartTime);
		MantlingParams.StartTime = FMath::GetMappedRangeValueClamped(SourceRange, TargetRange, MantlingHight);

		// Correct Animation start position
		MantlingParams.InitialAnimationLocation = MantlingParams.TargetLocation - MantlingSettings.AnimationCorrectionZ * FVector::UpVector + MantlingSettings.AnimationCorrectionXY * LedgeDescription.LedgeNormal;

		// Implement physical movement in BaseCharacterMovement
		GetBaseCharacterMovement()->StartMantle(MantlingParams);

		// Play animation of mantling
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		AnimInstance->Montage_Play(MantlingSettings.MantlingMontage, 1.0f, EMontagePlayReturnType::Duration, MantlingParams.StartTime); // Call this method because we must specify its start time
		//PlayAnimMontage(MantlingSettings.MantlingMontage)  - Here we cannot specify its start time
		OnMantle(MantlingSettings, MantlingParams.StartTime);
	}
}

bool ATPSBaseCharacter::CanJumpInternal_Implementation() const
{
	// This sentence forbids character to jump when the mantling action is taking place
	return Super::CanJumpInternal_Implementation() && !GetBaseCharacterMovement()->IsMantling();
}

void ATPSBaseCharacter::ClimbLadderUp(float Value)
{
	// TODO implement climbing ladder up and down
	if (GetBaseCharacterMovement()->IsOnLadder() && !FMath::IsNearlyZero(Value))
	{
		FVector LadderDirectionVector = GetBaseCharacterMovement()->GetCurrentLadder()->GetActorUpVector();
		AddMovementInput(LadderDirectionVector, Value);
	}
}

void ATPSBaseCharacter::InteractWithLadder() // fires when we press space bar
{
	// TODO implement Interact with ladder
	// If we already are on the ladder we should detach from it and jump off
	if (GetBaseCharacterMovement()->IsOnLadder())
	{
		GetBaseCharacterMovement()->DetachFromLadder(EDetachFromLadderMethod::JumpOff);
	}
	// Othervise get the ladder whose interactive volume we overlapping and attach to it
	else
	{
		// Initialize our ladder pointer
		const ALadder* Ladder = GetAvailableLadder();
		if (IsValid(Ladder))
		{
			// If we are on top of the object the ladder is leading to, play descending animation montage
			if (Ladder->GetIsOnTop())
			{
				PlayAnimMontage(Ladder->GetAttachFromTopAnimMontage());
			}
			// Attach to our initialized ladder
			GetBaseCharacterMovement()->AttachToLadder(Ladder);
		}
		
	}
}

const ALadder* ATPSBaseCharacter::GetAvailableLadder() const
{
	const ALadder* Result = nullptr;
	// Loop all interactive actor's array till we find a ladder
	for (auto Actor : AvailableInteractiveActors)
	{
		if (Actor->IsA<ALadder>())
		{
			Result = StaticCast<const ALadder*>(Actor);
			break;
		}
	}
	return Result;
}

void ATPSBaseCharacter::Falling()
{
	// Ensures the UCharacterMovementComponent::NotifyJumpApex() is triggered not only when we jump, but also when we fall
	GetBaseCharacterMovement()->bNotifyApex = true; // Property of CharacterMovementComponent that signifies character has reached the peak point of its jump
}

void ATPSBaseCharacter::NotifyJumpApex()
{
	Super::NotifyJumpApex();
	// Here, at the highest point of jump we set the location from where we fall
	CurrentFallApex = GetActorLocation();
}

void ATPSBaseCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
	// Get the height we falled from
	float FallHeight = (CurrentFallApex - GetActorLocation()).Z;
	if (IsValid(FallDamageCurve))
	{
		// Get the appropriate damage value from curve
		float DamageAmount = FallDamageCurve->GetFloatValue(FallHeight);
		// Apply that damage
		TakeDamage(DamageAmount, FDamageEvent(), GetController(), Hit.GetActor());
	}
}

void ATPSBaseCharacter::StartFire()
{
	ARangeWeapon* CurrentWeapon = CharacterEquipmentComponent->GetCurrentWeapon();
	if (IsValid(CurrentWeapon))
	{
		CurrentWeapon->StartFire();
	}
}


void ATPSBaseCharacter::StopFire()
{
	ARangeWeapon* CurrentWeapon = CharacterEquipmentComponent->GetCurrentWeapon();
	if (IsValid(CurrentWeapon))
	{
		CurrentWeapon->StopFire();
	}
}

void ATPSBaseCharacter::StartAiming()
{
	ARangeWeapon* CurrentRangeWeapon = GetCharacterEquipmentComponent()->GetCurrentWeapon();
	if (!IsValid(CurrentRangeWeapon))
	{
		return;
	}
	bIsAiming = true;
	CurrentAimingMovementSpeed = CurrentRangeWeapon->GetAimMovementMaxSpeed();
	CurrentRangeWeapon->StartAim();
	OnStartAiming();
}

void ATPSBaseCharacter::StopAiming()
{
	if (!bIsAiming)
	{
		return;
	}
	ARangeWeapon* CurrentRangeWeapon = GetCharacterEquipmentComponent()->GetCurrentWeapon();
	if (IsValid(CurrentRangeWeapon))
	{
		CurrentRangeWeapon->StopAim();
	}
	bIsAiming = false;
	CurrentAimingMovementSpeed = 0.0f;
	OnStopAiming();
}

void ATPSBaseCharacter::OnStartAiming_Implementation()
{
	OnStartAimingInternal();
}

void ATPSBaseCharacter::OnStopAiming_Implementation()
{
	OnStopAimingInternal();
}

float ATPSBaseCharacter::GetAimingMovementSpeed() const
{
	return CurrentAimingMovementSpeed;
	
}

void ATPSBaseCharacter::RegisterInteractiveActor(AInteractiveActor* InInteractiveActor)
{
	AvailableInteractiveActors.AddUnique(InInteractiveActor);
}

void ATPSBaseCharacter::UnRegisterInteractiveActor(AInteractiveActor* InInteractiveActor)
{
	// RemoveSingleSwap works faster than Remove as Remove works like std::remove(iter begin, iter end, T value) followed by std::erase(iter) and keeps an order of elems
	// but RemoveSingleSwap (for single element) just swaps the element with the last one in array than erases it
	AvailableInteractiveActors.RemoveSingleSwap(InInteractiveActor);
}


