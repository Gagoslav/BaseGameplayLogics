// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/Notifies/AnimNotify_AttachEquippedItem.h"
#include "Character/TPSBaseCharacter.h"
#include "Components/CharacterEquipmentComponent.h"

void UAnimNotify_AttachEquippedItem::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	ATPSBaseCharacter* CharacterOwner = Cast<ATPSBaseCharacter>(MeshComp->GetOwner());
	if (!IsValid(CharacterOwner))
	{
		return;
	}
	CharacterOwner->GetCharacterEquipmentComponent_Mutable()->AttachCurrentItemToEquippedSocket();
}
