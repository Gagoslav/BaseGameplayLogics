// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/FPSCharacterAnimInstance.h"
#include "Character/FPPlayerCharacter.h"
#include "Controllers/TPSPlayerController.h"

void UFPSCharacterAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
	checkf(TryGetPawnOwner()->IsA<AFPPlayerCharacter>(), TEXT("UFPSCharacterAnimInstance::NativeBeginPlay() only AFPPlayerCharacter can use this anim instance"));
	InFPPlayerCharacter = StaticCast<AFPPlayerCharacter*>(TryGetPawnOwner());
}

void UFPSCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (!InFPPlayerCharacter.IsValid())
	{
		return;
	}
	
	PlayerCameraPitchAngle = CalculateCameraPitchAngle();
	
}

float UFPSCharacterAnimInstance::CalculateCameraPitchAngle() const
{
	float Result = 0.0f;
	ATPSPlayerController* Controller = InFPPlayerCharacter->GetController<ATPSPlayerController>();
	if (IsValid(Controller) && !Controller->GetIgnoreCameraPitch())
	{
		
		Result = Controller->GetControlRotation().Pitch;
	}
	return Result;
}

