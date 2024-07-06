// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/LedgeDetectorComponent.h"
#include "BaseCharacterMovementComponent.generated.h"






enum class ECustomMovementMode : uint8;
class ALadder;

UENUM(BlueprintType)
enum class EDetachFromLadderMethod :uint8
{
	Fall = 0,
	ReachingTheTop,
	ReachingTheBottom,
	JumpOff
};

// Structure containing main fields necessary to manipulate anim montage of Mantle
struct FMantlingMovementParameters
{
	FVector InitialLocation = FVector::ZeroVector; // Like Character's current location
	FRotator InitialRotation = FRotator::ZeroRotator; // Like Character's current rotation

	FVector TargetLocation = FVector::ZeroVector; // Like location of FLedgeDescription
	FRotator TargetRotation = FRotator::ZeroRotator; // Like rotation of FLedgeDescription

	FVector InitialAnimationLocation = FVector::ZeroVector; // Like initial character's location if he used the whole animation (from 0 second)

	float Duration = 1.0f; // Duration of Anim montage // Hard coded
	float StartTime = 0.0f; // The moment when we start working with curve

	UCurveVector* MantlingCurve; // Curve vector that will regulate the behaviour of animation montage
};

UCLASS()
class TPSSHOOTER_API UBaseCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:
	// Method of UCharacterMovementComponent resposible for changing rotation every tick, works every tick
	virtual void PhysicsRotation(float DeltaTime) override; 

	inline bool GetIsSprinting() { return bIsSprinting; }
	void StartSprint();
	void EndSprint();

	void StartMantle(const FMantlingMovementParameters & InMantlingParams);
	void EndMantle();
	bool IsMantling()const;

	// Methods associated with ladder 

	void AttachToLadder(const ALadder* Ladder);
	void DetachFromLadder(EDetachFromLadderMethod DetachFromLadderMethod = EDetachFromLadderMethod::Fall);
	bool IsOnLadder()const;
	inline const ALadder* GetCurrentLadder() const { return CurrentLadder; }
	float GetLadderSpeedRatio() const;

	virtual float GetMaxSpeed() const override;

protected:

	// Default parameter for sprint speed
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Movement: Sprint", meta =(ClampMin= 0.0f, UIMin = 0.0f))
	float SprintSpeed = 1200.0f;

	virtual void OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode) override;
	virtual void PhysCustom(float DeltaTime, int32 Iterations) override; // is called every tick

	void PhysMantling(float DeltaTime, uint32 Iterations);
	void PhysLadder(float DeltaTime, uint32 Iterations);
	float GetActorToCurrentLadderProjection(const ALadder* Ladder, const FVector & CharacterLocation) const;

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Movement: Swimming", meta = (ClampMin = 0.0f, UIMin = 0.0f))
	// Default radius of collision capsule when character is swimming
	float SwimmingCapsuleRadius = 60.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Movement: Swimming", meta = (ClampMin = 0.0f, UIMin = 0.0f))
	// Default half height of collision capsule when character is swimming
	float SwimmingHalfHeight = 60.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Movement: Ladder", meta = (ClampMin = 0.0f, UIMin = 0.0f))
	// Speed when character is on ladder
	float ClimbingLadderMaxSpeed = 200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Movement: Ladder", meta = (ClampMin = 0.0f, UIMin = 0.0f))
	// Deceleration (the speed the character will decrement its acceleration)
	float ClimbingOnLadderBrakingDeceleraction = 2048.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Movement: Ladder", meta = (ClampMin = 0.0f, UIMin = 0.0f))
	float LadderToCharacterOffsetX = 60.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Movement: Ladder", meta = (ClampMin = 0.0f, UIMin = 0.0f))
	float MaxLadderTopOffset = 50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Movement: Ladder", meta = (ClampMin = 0.0f, UIMin = 0.0f))
	float MinLadderBottomOffset = 90.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Movement: Ladder", meta = (ClampMin = 0.0f, UIMin = 0.0f))
	float JumpOffFromLadderSpeed = 500.0f;

	class ATPSBaseCharacter* GetBaseCharacterOwner() const;

private:
	
	// Mantling parameters initialized within base character passed from it
	FMantlingMovementParameters CurrentMantlingParams;

	// Timer that will reset the movement mode to walking one as soon as it's expired
	FTimerHandle MantlingTimer;
	float TargetMantlingTime = 1.0f;

	bool bIsSprinting;
	const ALadder* CurrentLadder = nullptr;

	FRotator ForceTargetRotation = FRotator::ZeroRotator;
	bool bForceRotation = false;
};

