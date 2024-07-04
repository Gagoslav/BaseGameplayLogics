// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/TPSPlayerCharacter.h"
#include "FPPlayerCharacter.generated.h"

/**
 * 
 */
UCLASS()
class TPSSHOOTER_API AFPPlayerCharacter : public ATPSPlayerCharacter
{
	GENERATED_BODY()
public:
	// As our base class contains FObjectInitializer we need to pass it here as an argument
	AFPPlayerCharacter(const FObjectInitializer& ObjectInitializer);
	
	virtual void PossessedBy(AController* NewController) override;

	// Override the methods of ACharacter
	virtual void OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;
	virtual void OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;

	virtual FRotator GetViewRotation() const override;

	virtual void Tick(float DeltaTime) override;

	virtual void OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMovementMode = 0) override;

	
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character | First person")
	// Skeletal mesh component that will represent the character's arms
	USkeletalMeshComponent* FirstPersonMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character | First person")
	// Unlike 3rd person camera, the 1st person camera doesn't have any spring arm and is attached to 1st person skeletal mesh
	UCameraComponent* FirstPersonCameraComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character | First person | Camera | Ladder | Pitch", meta = (UIMin = -89.0f, UIMax = 89.0f));
	float LadderCameraMinPitch = -60.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character | First person | Camera | Ladder | Pitch", meta = (UIMin = -89.0f, UIMax = 89.0f));
	float LadderCameraMaxPitch = 80.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character | First person | Camera | Ladder | Pitch", meta = (UIMin = 0.0f, UIMax = 359.0f));
	float LadderCameraMinYaw = 5.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character | First person | Camera | Ladder | Pitch", meta = (UIMin = 0.0f, UIMax = 359.0f));
	float LadderCameraMaxYaw = 175.0f;


	virtual void OnMantle(const FMantlingSettings& MantlingSettings, float MantlingAnimationStart) override;


private:
	FTimerHandle FPMontageTimer;

	// Just return control over input as soon as mantle animation is over
	void OnFPMontageTimerElapsed();
	TWeakObjectPtr<class ATPSPlayerController> TPSPlayerController;
	bool IsFPMontagePlaying() const;

	void OnLadderStopped();

	void OnLadderStarted();
};
