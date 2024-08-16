// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/Notifies/SetMeleeHitRegEnabled.h"
#include "Character/TPSBaseCharacter.h"
#include "Components/CharacterEquipmentComponent.h"
#include "Actors/Equipment/MeleeEquipableItems.h"

void USetMeleeHitRegEnabled::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	ATPSBaseCharacter* CharacterOwner = Cast<ATPSBaseCharacter>(MeshComp->GetOwner());
	if (!IsValid(CharacterOwner))
	{
		return;
	}
	AMeleeEquipableItems* MeleeItem = CharacterOwner->GetCharacterEquipmentComponent()->GetCurrentMeleeItem();
	if (IsValid(MeleeItem))
	{
		MeleeItem->SetIsHitRegistrationEnabled(bIsHitRegistrationEnabled);
	}

}
