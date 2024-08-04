// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/RaticleWidget.h"
#include "Actors/Equipment/EquipableItems.h"
#include "BaseData/BaseEnums.h"

void URaticleWidget::OnEquippedItemChanged_Implementation(const AEquipableItems* EquippedItem)
{
	CurrentEquippedItem = EquippedItem;
	SetupCurrentReticle();
}

void URaticleWidget::OnAimingStateChanged_Implementation(bool bIsAiming)
{
	SetupCurrentReticle();
}

void URaticleWidget::SetupCurrentReticle()
{
	CurrentReticle = CurrentEquippedItem.IsValid() ? CurrentEquippedItem->GetReticleType() : EReticleType::None;
}
