// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/Notifies/AnimNotify_ReloadAmmo.h"
#include "Character/TPSBaseCharacter.h"
#include "Components/CharacterEquipmentComponent.h"

void UAnimNotify_ReloadAmmo::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	UAnimNotify::Notify(MeshComp, Animation); // Super didn't work, IDK why
	ATPSBaseCharacter* CharacterOwner = Cast<ATPSBaseCharacter>(MeshComp->GetOwner());
	if (!IsValid(CharacterOwner))
	{
		return;
	}
	CharacterOwner->GetCharacterEquipmentComponent_Mutable()->ReloadAmmoInCurrentWeapon(NumberOfAmmo, true); // Especial case for Shotguns
}
