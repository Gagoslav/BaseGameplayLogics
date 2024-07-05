// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/TPSBaseCharacterAnimInstance.h"
#include "Components/BaseCharacterMovementComponent.h"
#include "Character/TPSBaseCharacter.h"
#include "Components/CharacterEquipmentComponent.h"

void UTPSBaseCharacterAnimInstance::NativeBeginPlay()
{
	//Call base implementation
	Super::NativeBeginPlay();

	// Make an assert to check that the owner pawn of this anim instanse must be object instanciated from ATPSBaseCharacter or its derived passport
	checkf(TryGetPawnOwner()->IsA<ATPSBaseCharacter>(), TEXT("Pawn owner should be ATPSBaseCharacter, otherwise we will have a crash"));
	InPlayerCharacter = StaticCast<ATPSBaseCharacter*>(TryGetPawnOwner());
	MovementComponent = InPlayerCharacter->GetBaseCharacterMovement();
}

void UTPSBaseCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	// Call base implementation
	Super::NativeUpdateAnimation(DeltaSeconds);

	// if we don't have owner character then make early return
	if (!InPlayerCharacter.IsValid())
	{
		return;
	}
	
	// Get main values every frame to change owner character's mesh accordingly
	
	Speed = MovementComponent->Velocity.Size();
	Direction = CalculateDirection(MovementComponent->Velocity, InPlayerCharacter->GetActorRotation());
	bIsFalling = MovementComponent->IsFalling();
	bIsCrouching = MovementComponent->IsCrouching();
	bIsSprinting = MovementComponent->GetIsSprinting();
	bIsCharacterRotOriented = MovementComponent->bOrientRotationToMovement;
	bIsSwimming = MovementComponent->IsSwimming();
	bIsOnLadder = MovementComponent->IsOnLadder();
	if (bIsOnLadder)
	{
		LadderSpeedRatio = MovementComponent->GetLadderSpeedRatio();
	}
	

	const UCharacterEquipmentComponent* CharacterEquipment = InPlayerCharacter->GetCharacterEquipmentComponent();
	CurrentEquipedItem = CharacterEquipment->GetCurrentEquipedItemType();

	AimRotation = InPlayerCharacter->GetBaseAimRotation(); // Is a default method of APawn
}
