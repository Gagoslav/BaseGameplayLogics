// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/Notifies/AnimNotify_LaunchThrowable.h"
#include "Character/TPSBaseCharacter.h"
#include "Components/CharacterEquipmentComponent.h"

void UAnimNotify_LaunchThrowable::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	ATPSBaseCharacter* CharacterOwner = Cast<ATPSBaseCharacter>(MeshComp->GetOwner()); // Notifiers should do dynamic cast
	if (!IsValid(CharacterOwner))
	{
		return;
	}
	// Throw Item
	CharacterOwner->GetCharacterEquipmentComponent_Mutable()->LaunchCurrentThrowableItem();
}
