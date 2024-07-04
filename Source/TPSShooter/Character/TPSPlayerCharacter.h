// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Character/TPSBaseCharacter.h"
#include "TPSPlayerCharacter.generated.h"

/**
 * 
 */
UCLASS(Blueprintable) // as parent class in Abstract and Not Blueprintable
class TPSSHOOTER_API ATPSPlayerCharacter : public ATPSBaseCharacter
{
	GENERATED_BODY()
public:
	ATPSPlayerCharacter(const FObjectInitializer& ObjectInitializer);
	
	// Create implementations of pure virtual functions of base class
	virtual void MoveForward(float Value) override;
	virtual void MoveRight(float Value) override;
	virtual void Turn(float Value) override;
	virtual void LookUp(float Value) override;
	virtual void Jump() override;

	virtual void SwimForward(float Value) override;
	virtual void SwimRight(float Value) override;
	virtual void SwimUp(float Value) override;

	// Override ACharacter's methods responsible for logic that is fired as soon as character Crouch/Uncrouch
	virtual void OnStartCrouch(float HeightAdjust, float ScaledHeightAdjust) override;
	virtual void OnEndCrouch(float HeightAdjust, float ScaledHeightAdjust) override;

	virtual bool CanJumpInternal_Implementation() const override;

	
	
	
	

protected:
	// Declare base components for player character

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character|Camera");
	class UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character|SpringArm");
	class USpringArmComponent* SpringArm;
	
};
