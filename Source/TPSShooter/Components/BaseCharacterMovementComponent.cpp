// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/BaseCharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "Components/LedgeDetectorComponent.h"
#include "BaseData/BaseEnums.h"
#include "Curves/CurveVector.h"
#include "Actors/Interactive/Environment/Ladder.h"
#include "Character/TPSBaseCharacter.h"



// Gets owner Base player character of this movement component
ATPSBaseCharacter* UBaseCharacterMovementComponent::GetBaseCharacterOwner() const
{
	return StaticCast<ATPSBaseCharacter*>(GetCharacterOwner());
}

// This implementation doesn't allow to change rotation if (bOrientRotation = true) while our character is on ladder
void UBaseCharacterMovementComponent::PhysicsRotation(float DeltaTime)
{
	if (bForceRotation)
	{
		/* Code copy pasted from UCharacterMovementComponent */ 
		
		FRotator CurrentRotation = UpdatedComponent->GetComponentRotation(); // Normalized
		CurrentRotation.DiagnosticCheckNaN(TEXT("CharacterMovementComponent::PhysicsRotation(): CurrentRotation"));

		FRotator DeltaRot = GetDeltaRotation(DeltaTime);
		DeltaRot.DiagnosticCheckNaN(TEXT("CharacterMovementComponent::PhysicsRotation(): GetDeltaRotation"));
		// Accumulate a desired new rotation.
		const float AngleTolerance = 1e-3f;

		if (!CurrentRotation.Equals(ForceTargetRotation, AngleTolerance))
		{
			FRotator DesiredRotation = ForceTargetRotation;
			// PITCH
			if (!FMath::IsNearlyEqual(CurrentRotation.Pitch, DesiredRotation.Pitch, AngleTolerance))
			{
				DesiredRotation.Pitch = FMath::FixedTurn(CurrentRotation.Pitch, DesiredRotation.Pitch, DeltaRot.Pitch);
			}

			// YAW
			if (!FMath::IsNearlyEqual(CurrentRotation.Yaw, DesiredRotation.Yaw, AngleTolerance))
			{
				DesiredRotation.Yaw = FMath::FixedTurn(CurrentRotation.Yaw, DesiredRotation.Yaw, DeltaRot.Yaw);
			}

			// ROLL
			if (!FMath::IsNearlyEqual(CurrentRotation.Roll, DesiredRotation.Roll, AngleTolerance))
			{
				DesiredRotation.Roll = FMath::FixedTurn(CurrentRotation.Roll, DesiredRotation.Roll, DeltaRot.Roll);
			}

			// Set the new rotation.
			DesiredRotation.DiagnosticCheckNaN(TEXT("CharacterMovementComponent::PhysicsRotation(): DesiredRotation"));
			MoveUpdatedComponent(FVector::ZeroVector, DesiredRotation, /*bSweep*/ false);
		}
		else
		{
			ForceTargetRotation = FRotator::ZeroRotator;
			bForceRotation = false;
		}
		return;
	}
	if (IsOnLadder())
	{
		return;
	}
	Super::PhysicsRotation(DeltaTime);

}

void UBaseCharacterMovementComponent::StartSprint()
{
	// Just set the flag bIsSprinting to true and switches Maximal acceleration on for gamepad 
	bIsSprinting = 1;
	bForceMaxAccel = 1;
}

void UBaseCharacterMovementComponent::EndSprint()
{
	// Just set the flag bIsSprinting to false and turns Maximal acceleration down for gamepad
	bIsSprinting = 0;
	bForceMaxAccel = 0;
}

void UBaseCharacterMovementComponent::StartMantle(const FMantlingMovementParameters& InMantlingParams)
{

	// Initialize FMantlingMovementParameters field
	CurrentMantlingParams = InMantlingParams;
	// change movement mode to the corresponding customly created one, as in case EMovementMode::MOVE_Custom in SetMovementMode, we need second argument (customly created CMOVE)
	SetMovementMode(EMovementMode::MOVE_Custom, (uint8)ECustomMovementMode::CMOVE_Mantling);

}

void UBaseCharacterMovementComponent::EndMantle()
{
	// Return movement mode to the walking one
	SetMovementMode(EMovementMode::MOVE_Walking);
}

bool UBaseCharacterMovementComponent::IsMantling() const
{
	// All methods within character movement component that return a flag of movement state should first check if we have (UpdatedComponent)
	// and then check the EMovementMode state
	return UpdatedComponent && MovementMode == MOVE_Custom && CustomMovementMode == (uint8)ECustomMovementMode::CMOVE_Mantling;
}

void UBaseCharacterMovementComponent::AttachToLadder(const ALadder* Ladder)
{
	// Initialize constant pointer to ladder
	CurrentLadder = Ladder;
	FRotator TargetOrientationRotation = CurrentLadder->GetActorForwardVector().ToOrientationRotator();
	TargetOrientationRotation.Yaw += 180.0f;

	FVector LadderUpVector = CurrentLadder->GetActorUpVector();
	FVector LadderForwardVector = CurrentLadder->GetActorForwardVector();
	// Get Height the character should attach on ladder at
	float ActorToLadderProjection = GetActorToCurrentLadderProjection(CurrentLadder, GetActorLocation());

	// Calculate the location where the character should attach
	FVector NewCharacterLocation = CurrentLadder->GetActorLocation() + ActorToLadderProjection * LadderUpVector + LadderToCharacterOffsetX * LadderForwardVector;
	if (CurrentLadder->GetIsOnTop())
	{
		NewCharacterLocation = CurrentLadder->GetAttachFromTopAnimMontageStartingLocation();
	}
	// Set character's transform
	GetOwner()->SetActorLocation(NewCharacterLocation);
	GetOwner()->SetActorRotation(TargetOrientationRotation);

	// Set appropriate movement mode
	SetMovementMode(EMovementMode::MOVE_Custom, (uint8)ECustomMovementMode::CMOVE_Ladder);
}


void UBaseCharacterMovementComponent::DetachFromLadder(EDetachFromLadderMethod DetachFromLadderMethod /*EDetachFromLadderMethod::Fall*/)
{
	switch (DetachFromLadderMethod)
	{
	case EDetachFromLadderMethod::ReachingTheBottom:
	{
		SetMovementMode(MOVE_Walking);
		break;
	}
	case EDetachFromLadderMethod::ReachingTheTop:
	{
		GetBaseCharacterOwner()->Mantle(true);
		break;
	}
	case EDetachFromLadderMethod::JumpOff:
	{
		FVector JumpDirection = CurrentLadder->GetActorForwardVector();
		SetMovementMode(EMovementMode::MOVE_Falling);
		FVector JumpVelocity = JumpDirection * JumpOffFromLadderSpeed;
		ForceTargetRotation = JumpDirection.ToOrientationRotator();
		bForceRotation = true;
		Launch(JumpVelocity);
		break;
	}

	case EDetachFromLadderMethod::Fall:
	default:
	{
		SetMovementMode(EMovementMode::MOVE_Falling);
		break;
	}
	}

}


bool UBaseCharacterMovementComponent::IsOnLadder() const
{
	// All methods within character movement component that return a flag of movement state should first check if we have (UpdatedComponent)
	// and then check the EMovementMode state
	return UpdatedComponent && MovementMode == EMovementMode::MOVE_Custom && CustomMovementMode == (uint8)ECustomMovementMode::CMOVE_Ladder;
}



// Extracted function
float UBaseCharacterMovementComponent::GetActorToCurrentLadderProjection(const ALadder* Ladder, const FVector& CharacterLocation) const
{
	checkf(IsValid(Ladder), TEXT("UBaseCharacterMovementComponent::GetCharacterToCurrentLadderProjection(const ALadder* Ladder) cannot be invoked, if current ladder is NULL"))

	FVector LadderUpVector = Ladder->GetActorUpVector();
	FVector LadderToCharacterDistance = CharacterLocation - Ladder->GetActorLocation();
	// Find projection with the use of DotProduct
	return FVector::DotProduct(LadderUpVector, LadderToCharacterDistance);
}



float UBaseCharacterMovementComponent::GetLadderSpeedRatio() const
{
	checkf(IsValid(CurrentLadder), TEXT("UBaseCharacterMovementComponent::GetLadderSpeedRatio() cannot be invoked, if current ladder is NULL"));
	FVector LadderUpVector = CurrentLadder->GetActorUpVector();
	// To get a ladder speed ratio (for situation we use gamepad and want to climb the ladder steps slowly)
	// we need to get Velocitie's up value by DotProduct and divide it by maximal climbing speed
	// thus getting the ratio (value between 0 and 1)
	return FVector::DotProduct(LadderUpVector, Velocity) / ClimbingLadderMaxSpeed;

}



float UBaseCharacterMovementComponent::GetMaxSpeed() const
{
	// Cach the max speed in not sprinting mode from base character movement settings 
	float Result = Super::GetMaxSpeed();
	if (bIsSprinting)
	{
		// Set new maximal speed for sprinting
		Result = SprintSpeed;
	}
	else if (IsOnLadder())
	{
		Result = ClimbingLadderMaxSpeed;
	}
	return Result;
}

void UBaseCharacterMovementComponent::OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PreviousMovementMode, PreviousCustomMode);
	// if current movement mode is swimming mode
	if (MovementMode == MOVE_Swimming)
	{
		// Set to swimming capsule size
		CharacterOwner->GetCapsuleComponent()->SetCapsuleSize(SwimmingCapsuleRadius, SwimmingHalfHeight);
	}
	else if (PreviousMovementMode == MOVE_Swimming)
	{
		// Set to normal capsule size
		// Get the default object of character class to get its capsule component's parameters
		ACharacter* DefaultCharacter = CharacterOwner->GetClass()->GetDefaultObject<ACharacter>();
		CharacterOwner->GetCapsuleComponent()->SetCapsuleSize(DefaultCharacter->GetCapsuleComponent()->GetUnscaledCapsuleRadius(), DefaultCharacter->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight(), true);
	}

	if (PreviousMovementMode == MOVE_Custom && PreviousCustomMode == (uint8)ECustomMovementMode::CMOVE_Ladder)
	{

	}

	// Implement logic for Mantling custom movement mode
	if (MovementMode == MOVE_Custom)
	{
		// Every time where our MovementMode is MOVE_Custom we MUST deal with CustomMovementMode
		switch (CustomMovementMode)
		{
		case (uint8)ECustomMovementMode::CMOVE_Mantling:
		{

			// Set timer for Mantling state and as soon as its time is expired call EndMantle function that will simply change the MovementMode to MOVE_Walking
			GetWorld()->GetTimerManager().SetTimer(MantlingTimer, this, &UBaseCharacterMovementComponent::EndMantle, CurrentMantlingParams.Duration, false);
			break;
		}
		default:
			break;
		}
	}
}

// ENCAPSULATE ALL LOGICS RELATED TO MOVEMENT DURING MANTLE AND LADDER IN BASE CHARACTER MOVEMENT COMPONENT
// Function is called every tick and is designed to define custom physics
void UBaseCharacterMovementComponent::PhysCustom(float DeltaTime, int32 Iterations)
{
	switch (CustomMovementMode)
	{
	case (uint8)ECustomMovementMode::CMOVE_Mantling:
	{
		PhysMantling(DeltaTime, Iterations);
		break;
	}
	case (uint8)ECustomMovementMode::CMOVE_Ladder:
	{
		PhysLadder(DeltaTime, Iterations);
		break;
	}
	default:
		break;
	}

	// Call custom implementation
	UCharacterMovementComponent::PhysCustom(DeltaTime, Iterations);
}


// Extracted Method Mantling
void UBaseCharacterMovementComponent::PhysMantling(float DeltaTime, uint32 Iterations)
{
	// Every tick (PhysCustom) get necessary time from normalized timer and add start time when the movement in AnimMontage starts  
	float ElapsedTime = GetWorld()->GetTimerManager().GetTimerElapsed(MantlingTimer) + CurrentMantlingParams.StartTime;

	// Value under the particular time in curve vector
	FVector MantlingCurveValue = CurrentMantlingParams.MantlingCurve->GetVectorValue(ElapsedTime);

	// Get Alpha from subcurves from CurveVector implemented in engine
	float PositionAlpha = MantlingCurveValue.X;
	float XYCorrectionAlpha = MantlingCurveValue.Y;
	float ZCorrectionAlpha = MantlingCurveValue.Z;

	// As we have curve vector that uses 3 float curves we can correct our movement by lerping 3 values
	FVector CorrectedInitialLocation = FMath::Lerp(CurrentMantlingParams.InitialLocation, CurrentMantlingParams.InitialAnimationLocation, XYCorrectionAlpha);
	CorrectedInitialLocation.Z = FMath::Lerp(CurrentMantlingParams.InitialLocation.Z, CurrentMantlingParams.InitialAnimationLocation.Z, ZCorrectionAlpha);

	FVector NewLocation = FMath::Lerp(CorrectedInitialLocation, CurrentMantlingParams.TargetLocation, PositionAlpha);
	FRotator NewRotation = FMath::Lerp(CurrentMantlingParams.InitialRotation, CurrentMantlingParams.TargetRotation, PositionAlpha);

	// Vector of direction used in SafeMoveUpdatedComponent for it to know in what direction move our character (Delta)
	FVector Delta = NewLocation - GetActorLocation();
	FHitResult HitResult;
	SafeMoveUpdatedComponent(Delta, NewRotation, false, HitResult); // This method is designed to move our character by direction 
	
}

// Extracted Method Ladder
void UBaseCharacterMovementComponent::PhysLadder(float DeltaTime, uint32 Iterations)
{
	CalcVelocity(DeltaTime, 1.0f, false, ClimbingOnLadderBrakingDeceleraction);
	FVector Delta = Velocity * DeltaTime; // Velocity is a field of MovementComponent
	if (HasAnimRootMotion())
	{
		FHitResult HitResult;
		SafeMoveUpdatedComponent(Delta, GetOwner()->GetActorRotation(), false, HitResult); // ignore collisions in root motion
		UE_LOG(LogTemp, Warning, TEXT("HasAnimRootMotion()"));
		return;
	}

	FVector NewPos = GetActorLocation() + Delta;
	float NewPosProjection = GetActorToCurrentLadderProjection(CurrentLadder, NewPos);

	if (NewPosProjection < MinLadderBottomOffset)
	{
		DetachFromLadder(EDetachFromLadderMethod::ReachingTheBottom);
		return;
	}
	else if (NewPosProjection > (CurrentLadder->GetLadderHeight() - MaxLadderTopOffset))
	{
		DetachFromLadder(EDetachFromLadderMethod::ReachingTheTop);
		return;
	}

	FHitResult HitResult;
	SafeMoveUpdatedComponent(Delta, GetOwner()->GetActorRotation(), true, HitResult); // This method is designed to move our character by direction 
}
