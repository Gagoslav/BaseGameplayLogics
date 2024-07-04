// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/FPPlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/BaseCharacterMovementComponent.h"
#include "BaseData/BaseEnums.h"
#include "Controllers//TPSPlayerController.h"

AFPPlayerCharacter::AFPPlayerCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	FirstPersonMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	FirstPersonMeshComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonMeshComponent->SetRelativeLocation(FVector(0.0f, 0.0f, -86.0f));
	FirstPersonMeshComponent->CastShadow = false;
	FirstPersonMeshComponent->bCastDynamicShadow = false;
	FirstPersonMeshComponent->SetOnlyOwnerSee(true);
	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(FirstPersonMeshComponent, SocketFPCamera);
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Make owner not to see the 3rd person character's skeletal mesh, using ACharacter::GetMesh()
	GetMesh()->SetOwnerNoSee(true);
	GetMesh()->bCastHiddenShadow = true;

	// This will allow the camera manager to choose our FirstPersonCameraComponent as camera, by disabling base classe's camera
	CameraComponent->bAutoActivate = false;

	// Disable SpringArm
	SpringArm->bAutoActivate = false;
	// Change controlling method 
	SpringArm->bUsePawnControlRotation = false;

	// In first person view mode we don's use bOrientRotationToMovement
	GetBaseCharacterMovement()->bOrientRotationToMovement = false;
	bUseControllerRotationYaw = true;

}

void AFPPlayerCharacter::PossessedBy(AController* NewController) // Is called only on server
{
	Super::PossessedBy(NewController);
	TPSPlayerController = Cast<ATPSPlayerController>(Controller);
}

void AFPPlayerCharacter::OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust)
{
	Super::OnStartCrouch(HalfHeightAdjust, ScaledHalfHeightAdjust);
	const AFPPlayerCharacter* DefaultCharacter = GetDefault<AFPPlayerCharacter>(GetClass());
	// We declare a NON const reference to change (SET) the value of relative location through it using appropriate GetRelativeLocation_DirectMutable()
	FVector& FirstPersonMeshRelativeLocation = FirstPersonMeshComponent->GetRelativeLocation_DirectMutable(); // this method returns a reference of Relative location's FVector, so we can change it
	FirstPersonMeshRelativeLocation.Z = DefaultCharacter->FirstPersonMeshComponent->GetRelativeLocation().Z + HalfHeightAdjust;
}

void AFPPlayerCharacter::OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust)
{
	Super::OnEndCrouch(HalfHeightAdjust, ScaledHalfHeightAdjust);
	const AFPPlayerCharacter* DefaultCharacter = GetDefault<AFPPlayerCharacter>(GetClass()); // GetDefault method returns a const class default object differently from GetClass()->GetDefaultObject<Class>()
	// We declare a NON const reference to change (SET) the value of relative location through it using appropriate GetRelativeLocation_DirectMutable()
	FVector& FirstPersonMeshRelativeLocation = FirstPersonMeshComponent->GetRelativeLocation_DirectMutable(); // this method returns a reference of Relative location's FVector, so we can change it
	FirstPersonMeshRelativeLocation.Z = DefaultCharacter->FirstPersonMeshComponent->GetRelativeLocation().Z ;
}

FRotator AFPPlayerCharacter::GetViewRotation() const
{
	FRotator Result = Super::GetViewRotation();
	if(IsFPMontagePlaying())
	{
		FRotator SocketRotation =  FirstPersonMeshComponent->GetSocketRotation(SocketFPCamera);
		Result.Yaw = SocketRotation.Yaw;
		Result.Roll = SocketRotation.Roll;
		Result.Pitch += SocketRotation.Pitch;
	}
	return Result;
}

void AFPPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (IsFPMontagePlaying() && TPSPlayerController.IsValid()) // exclude the cases character is possessed by AI controller
	{
		FRotator TargetControlRotation = TPSPlayerController->GetControlRotation();
		// Correct target rotation as if we look forward
		TargetControlRotation.Pitch = 0.0f;
		float BlendSpeed = 300.0f;
		TargetControlRotation = FMath::RInterpTo(TPSPlayerController->GetControlRotation(), TargetControlRotation, DeltaTime, BlendSpeed); // Unlike FMath::Lerp, this method uses time (not alpha)
		TPSPlayerController->SetControlRotation(TargetControlRotation);
	}
}

void AFPPlayerCharacter::OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMovementMode)
{
	// This is a virtual method that exists as in ACharacterMovementComponent as well as in ACharacter class
	Super::OnMovementModeChanged(PrevMovementMode, PreviousCustomMovementMode);
	if (GetBaseCharacterMovement()->IsOnLadder())
	{
		OnLadderStarted();
	}
	else if (PreviousCustomMovementMode == (uint8)ECustomMovementMode::CMOVE_Ladder)
	{
		OnLadderStopped();
	}
}

void AFPPlayerCharacter::OnLadderStopped()
{
	if (TPSPlayerController.IsValid())
	{
		// If our PREVIOUS custom movement mode was ladder, meaning we don't climb on ladder any longer
		// Start calculating Pitch angle for camera again
		TPSPlayerController->SetIgnoreCameraPitch(false);

		// Also restart using Controller's yaw
		bUseControllerRotationYaw = true;

		// As soon as we skipped ladder state we will reset our view angle to default values by getting them from default camera manager object
		APlayerCameraManager* CameraManager = TPSPlayerController->PlayerCameraManager;
		APlayerCameraManager* DefaultCameraManager = CameraManager->GetClass()->GetDefaultObject<APlayerCameraManager>(); // returns non const default object unlike GetDefault<UClass>(GetClass())
		CameraManager->ViewPitchMin = DefaultCameraManager->ViewPitchMin;
		CameraManager->ViewPitchMax = DefaultCameraManager->ViewPitchMax;
		CameraManager->ViewYawMin = DefaultCameraManager->ViewYawMin;
		CameraManager->ViewYawMax = DefaultCameraManager->ViewYawMax;
	}
}

void AFPPlayerCharacter::OnLadderStarted()
{
	if (TPSPlayerController.IsValid())
	{
		// Ignore calculating Pitch angle for camera to not be able to take hands up/down while climbing on ladder 
		TPSPlayerController->SetIgnoreCameraPitch(true);

		// Also stop using Controller's yaw
		bUseControllerRotationYaw = false;

		// When we are on ladder we should limit our view angle so we will set the values from UPROPERTIES using camera manager
		APlayerCameraManager* CameraManager = TPSPlayerController->PlayerCameraManager;
		CameraManager->ViewPitchMin = LadderCameraMinPitch;
		CameraManager->ViewPitchMax = LadderCameraMaxPitch;
		CameraManager->ViewYawMin = LadderCameraMinYaw;
		CameraManager->ViewYawMax = LadderCameraMaxYaw;

	}
}

bool AFPPlayerCharacter::IsFPMontagePlaying() const
{
	// Get an anim instance of this class to check if it has an anim instance amd is any animation is played
	UAnimInstance* FPSAnimInstance = FirstPersonMeshComponent->GetAnimInstance();
	return IsValid(FPSAnimInstance) && FPSAnimInstance->IsAnyMontagePlaying();
		
}

void AFPPlayerCharacter::OnMantle(const FMantlingSettings& MantlingSettings, float MantlingAnimationStart)
{
	Super::OnMantle(MantlingSettings, MantlingAnimationStart);
	UAnimInstance* FPSAnimInstance = FirstPersonMeshComponent->GetAnimInstance();
	if (IsValid(FPSAnimInstance) && MantlingSettings.FirstPersonMantlingMontage)
	{
		
		if (TPSPlayerController.IsValid())
		{
			TPSPlayerController->SetIgnoreLookInput(true); // method ignores controller rotation 
			TPSPlayerController->SetIgnoreMoveInput(true); // method disables movement input
		}
		float MontageDuration = FPSAnimInstance->Montage_Play(MantlingSettings.FirstPersonMantlingMontage, 1.0f, EMontagePlayReturnType::Duration, MantlingAnimationStart); // Play FPAnimMontage and get its duration
		GetWorld()->GetTimerManager().SetTimer(FPMontageTimer, this, &AFPPlayerCharacter::OnFPMontageTimerElapsed, MontageDuration, false);
		UE_LOG(LogTemp, Warning, TEXT("Mantle"));
	}
}

void AFPPlayerCharacter::OnFPMontageTimerElapsed()
{
	
	
	if (TPSPlayerController.IsValid())
	{
		TPSPlayerController->SetIgnoreLookInput(false);
		TPSPlayerController->SetIgnoreMoveInput(false);
	}
}
