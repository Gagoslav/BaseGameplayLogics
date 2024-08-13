// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Equipment/EquipableItems.h"
#include "Character/TPSBaseCharacter.h"

void AEquipableItems::SetOwner(AActor* NewOwner)
{
	Super::SetOwner(NewOwner);
	if (IsValid(NewOwner))
	{
		checkf(GetOwner()->IsA<ATPSBaseCharacter>(), TEXT("AEquipableItems::SetOwner() Only character can be an owner of equipable item"));
		CachedCharacterOwner = GetCharacterOwner();
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
