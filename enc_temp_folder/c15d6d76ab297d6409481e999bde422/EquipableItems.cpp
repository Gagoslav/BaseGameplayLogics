// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Equipment/EquipableItems.h"

void AEquipableItems::Equip()
{
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
