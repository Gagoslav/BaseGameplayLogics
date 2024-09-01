// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/TPSPlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/BaseCharacterMovementComponent.h"
#include "Actors/Equipment/RangeWeapon.h"
#include "Components/CharacterEquipmentComponent.h"

ATPSPlayerCharacter::ATPSPlayerCharacter(const FObjectInitializer& ObjectInitializer):
	Super(ObjectInitializer)
{

	// Disable Controller rotation
	bUseControllerRotationPitch = 0;
	bUseControllerRotationRoll = 0;
	bUseControllerRotationYaw = 0;
	
	// Initialize base classes' main components
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->bUsePawnControlRotation = 1;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(SpringArm);
	CameraComponent->bUsePawnControlRotation = 0;

	// Set base values for character movement component
	GetBaseCharacterMovement()->bOrientRotationToMovement = 1;
	GetBaseCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
}

void ATPSPlayerCharacter::MoveForward(float Value)
{
	// Consider some small inaccuracies and check if we are on ground or in air, but not swimming for example
	if (GetBaseCharacterMovement()->IsMovingOnGround()|| GetBaseCharacterMovement()->IsFalling() && !FMath::IsNearlyZero(Value, 1e-6))
	{
		FRotator YawRotator(0.0f, GetControlRotation().Yaw, 0.0f);
		// to move a character along the mouse direction
		FVector Forward = YawRotator.RotateVector(FVector::ForwardVector);
		// Call base method for movement of APawn class
		AddMovementInput(Forward, Value);
	}
	
}

void ATPSPlayerCharacter::MoveRight(float Value)
{
	// Consider some small inaccuracies and check if we are on ground or in air, but not swimming for example
	if (GetBaseCharacterMovement()->IsMovingOnGround() || GetBaseCharacterMovement()->IsFalling() && !FMath::IsNearlyZero(Value, 1e-6))
	{
		FRotator YawRotator(0.0f, GetControlRotation().Yaw, 0.0f);
		// to move a character along the mouse direction
		FVector Right = YawRotator.RotateVector(FVector::RightVector);
		// Call base method for movement of APawn class
		AddMovementInput(Right, Value);
	}
	
}

// Create implementation of pure virtual methods of base class 
void ATPSPlayerCharacter::Turn(float Value)
{
	
	AddControllerYawInput(Value);
}

void ATPSPlayerCharacter::LookUp(float Value)
{
	
	AddControllerPitchInput(Value);
}

// Jump method is initially realized in ACharacter class
void ATPSPlayerCharacter::Jump()
{
	Super::Jump();
	
}


// Unlike Move forward here we can change swimming direction by looking also Up and Down
// using our mouse
void ATPSPlayerCharacter::SwimForward(float Value)
{
	// Consider some small inaccuracies and that our movement mode is Swimming
	// to avoid implementing two actions simultanously (will see input context later)
	if (GetBaseCharacterMovement()->IsSwimming() && !FMath::IsNearlyZero(Value, 1e-6))
	{
		// Add Pitch as we also may change direction To Up/Down
		FRotator PitchYawRotator(GetControlRotation().Pitch, GetControlRotation().Yaw, 0.0f);
		// to move a character along the mouse direction
		FVector Forward = PitchYawRotator.RotateVector(FVector::ForwardVector);
		// Call base method for movement of APawn class
		AddMovementInput(Forward, Value);
		
	}
}

// Is the same code as MoveRight
void ATPSPlayerCharacter::SwimRight(float Value)
{
	// Consider some small inaccuracies and that our movement mode is Swimming
	// to avoid implementing two actions simultanously (will see input context later)
	if (GetBaseCharacterMovement()->IsSwimming() && !FMath::IsNearlyZero(Value, 1e-6))
	{
		FRotator YawRotator(0.0f, GetControlRotation().Yaw, 0.0f);
		// to move a character along the mouse direction
		FVector Right = YawRotator.RotateVector(FVector::RightVector);
		// Call base method for movement of APawn class
		AddMovementInput(Right, Value);
		
	}
}

void ATPSPlayerCharacter::SwimUp(float Value)
{
	// Consider some small inaccuracies and that our movement mode is Swimming
	// to avoid implementing two actions simultanously (will see input context later)
	if (GetBaseCharacterMovement()->IsSwimming() && !FMath::IsNearlyZero(Value, 1e-6))
	{
		// We just add the up vector as movement direction
		AddMovementInput(FVector::UpVector, Value);
	}

}


void ATPSPlayerCharacter::OnStartCrouch(float HeightAdjust, float ScaledHeightAdjust)
{
	// Call base method implementation
	Super::OnStartCrouch(HeightAdjust, ScaledHeightAdjust);
	// Eliminate jitter effect by returning camera to the base position
	SpringArm->TargetOffset += FVector(0.0f, 0.0f, HeightAdjust);
}


void ATPSPlayerCharacter::OnEndCrouch(float HeightAdjust, float ScaledHeightAdjust)
{
	// Call base method implementation
	Super::OnEndCrouch(HeightAdjust, ScaledHeightAdjust);
	// Eliminate jitter effect by returning camera to the base position
	SpringArm->TargetOffset -= FVector(0.0f, 0.0f, HeightAdjust);
	
}

bool ATPSPlayerCharacter::CanJumpInternal_Implementation() const
{
	// We consider that we always can jump
	return bIsCrouched || Super::CanJumpInternal_Implementation();
}

void ATPSPlayerCharacter::OnStartAimingInternal()
{
	Super::OnStartAimingInternal();
	// Be sure that our player character isn't possessed by AI controller (that's possible)
	APlayerController* PlayerController = GetController<APlayerController>();
	if (!IsValid(PlayerController))
	{
		return;
	}
	// Get Camera Manager (is just public field in APlayerController class)
	APlayerCameraManager* CameraManager = PlayerController->PlayerCameraManager;
	if (IsValid(CameraManager))
	{
		ARangeWeapon* CurrentRangeWeapon = CharacterEquipmentComponent->GetCurrentWeapon();
		// Sets new field of view
		CameraManager->SetFOV(CurrentRangeWeapon->GetAimFOV()); // Main function SetFOV, why we made this all
	}

}

void ATPSPlayerCharacter::OnStopAimingInternal()
{
	Super::OnStopAimingInternal();
	APlayerController* PlayerController = GetController<APlayerController>();
	if (!IsValid(PlayerController))
	{
		return;
	}
	// Get Camera Manager (is just public field in APlayerController class)
	APlayerCameraManager* CameraManager = PlayerController->PlayerCameraManager;
	if (IsValid(CameraManager))
	{
		ARangeWeapon* CurrentRangeWeapon = CharacterEquipmentComponent->GetCurrentWeapon();
		// Resets the FOV to default value
		CameraManager->UnlockFOV();
	}
}







