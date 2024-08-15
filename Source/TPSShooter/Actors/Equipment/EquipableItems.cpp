// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Equipment/EquipableItems.h"
#include "Character/TPSBaseCharacter.h"

void AEquipableItems::SetOwner(AActor* NewOwner)
{
	Super::SetOwner(NewOwner);
	// If we have an owner that should be casted to ATPSBaseCharacter otherwise make a crush
	if (IsValid(NewOwner))
	{
		checkf(GetOwner()->IsA<ATPSBaseCharacter>(), TEXT("AEquipableItems::SetOwner() Only character can be an owner of equipable item"));
		CachedCharacterOwner = StaticCast<ATPSBaseCharacter*>(NewOwner);
	}
	else
	{
		CachedCharacterOwner = nullptr;
	}
}

void AEquipableItems::Equip()
{
	// Execute a delegate 
	if (OnEquipmentStateChanged.IsBound())
	{
		OnEquipmentStateChanged.Broadcast(true);
	}
}

void AEquipableItems::UnEquip()
{
	if (OnEquipmentStateChanged.IsBound())
	{
		OnEquipmentStateChanged.Broadcast(false);
	}
}

ATPSBaseCharacter* AEquipableItems::GetCharacterOwner() const
{
	return CachedCharacterOwner.IsValid() ? CachedCharacterOwner.Get() : nullptr;
}
