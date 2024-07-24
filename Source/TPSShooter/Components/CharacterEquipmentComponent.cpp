// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/CharacterEquipmentComponent.h"
#include "Character/TPSBaseCharacter.h"
#include "Actors/Equipment/RangeWeapon.h"
#include "BaseData/BaseEnums.h"





EEquipedItemType UCharacterEquipmentComponent::GetCurrentEquipedItemType() const
{
	EEquipedItemType Result = EEquipedItemType::None;
	// Perform logic only if we have weapon in arms, othervise EEquipedItemType::None
	if (IsValid(CurrentEquippedWeapon))
	{
		// Takes a type of equipped item from equipped item object
		Result = CurrentEquippedWeapon->GetEquipedItemType();
	}
	return Result;

}

// Called when the game starts
void UCharacterEquipmentComponent::BeginPlay()
{
	Super::BeginPlay();
	checkf(GetOwner()->IsA<ATPSBaseCharacter>(), TEXT("UCharacterEquipmentComponent::BeginPlay() Owner of Equipment component can be only ABaseCharacter actor"));
	InBaseCharacter = StaticCast<ATPSBaseCharacter*>(GetOwner());
	CreateLoadOut();


}

void UCharacterEquipmentComponent::ReloadCurrentWeapon()
{
	// Make sure we have any equipped weapon
	checkf(IsValid(CurrentEquippedWeapon), TEXT("UCharacterEquipmentComponent::ReloadCurrentWeapon() should have a Valid weapon"));
	int32 AvailableAmunition = GetAvailableAmunitionForCurrentWeapon();

	// If we don't have any amunition for a particular weapon at all then make return
	
	if (!CanReload(AvailableAmunition))
	{
		return;
	}
	
	CurrentEquippedWeapon->StartReload();

}

bool UCharacterEquipmentComponent::CanReload(int32 AmmoNum)
{
	return AmmoNum > 0 && !bIsEquipping;

}

void UCharacterEquipmentComponent::EquipItemInSlot(EEquipmentSlots Slot)
{
	if (bIsEquipping)
	{
		return;
	}
	
	// First we need to take away the current equipped item to the character's back and disactivate all delegates related to that particullar item 

	UnEquipCurrentItem();

	// Then we get/select the necessary item from the array of character's items
	CurrentEquippedItem = ItemsArray[(uint32)Slot];
	CurrentEquippedWeapon = Cast<ARangeWeapon>(CurrentEquippedItem);

	// And here we equip the selected items to character's arms if this item is valid
	if (IsValid(CurrentEquippedItem))
	{
		UAnimMontage* EquipAnimMontage = CurrentEquippedItem->GetCharacterAnimMontage();
		if (IsValid(EquipAnimMontage))
		{
			bIsEquipping = true;
			float EquipDuration = InBaseCharacter->PlayAnimMontage(EquipAnimMontage);
			GetWorld()->GetTimerManager().SetTimer(EquipTimer, this, &UCharacterEquipmentComponent::EquipAnimationFinished, EquipDuration, false);
		}
		else
		{
			AttachCurrentItemToEquippedSocket();
			CurrentEquippedSlot = Slot;
		}
		

	}

	// Finally we bind the events of our equipped weapon
	if (IsValid(CurrentEquippedWeapon))
	{
		// Bind the delegate of ARangeWeapon to the OnCurrentWeaponUpdatedAmmo function of UCharacterEquipmentComponent
		OnCurrentWeaponAmmoChangedHandle = CurrentEquippedWeapon->OnAmmoChangedEvent.AddUFunction(this, FName("OnCurrentWeaponUpdatedAmmo"));
		OnCurrentWeaponReloadedHandle = CurrentEquippedWeapon->OnReloadCompleteEvent.AddUFunction(this, FName("OnWeaponReloadComplete"));

		// Call this method also during weapon creation to have an initial amount of bullets
		OnCurrentWeaponUpdatedAmmo(CurrentEquippedWeapon->GetAmmo());
	}
}

void UCharacterEquipmentComponent::AttachCurrentItemToEquippedSocket()
{
	CurrentEquippedItem->AttachToComponent(InBaseCharacter->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, CurrentEquippedItem->GetEquippedSocketName());
}

void UCharacterEquipmentComponent::UnEquipCurrentItem()
{
	// Attach weapon to it's "Unequipped" position on character
	if (IsValid(CurrentEquippedItem))
	{
		CurrentEquippedItem->AttachToComponent(InBaseCharacter->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, CurrentEquippedItem->GetUnEquippedSocketName());
	}

	if (IsValid(CurrentEquippedWeapon))
	{
		CurrentEquippedWeapon->StopFire();
		CurrentEquippedWeapon->EndReload(false);
		// UnBind the delegate of ARangeWeapon to the OnCurrentWeaponUpdatedAmmo function of UCharacterEquipmentComponent as we have removed our weapon from hands
		CurrentEquippedWeapon->OnAmmoChangedEvent.Remove(OnCurrentWeaponAmmoChangedHandle);
		CurrentEquippedWeapon->OnReloadCompleteEvent.Remove(OnCurrentWeaponReloadedHandle);
	}
	CurrentEquippedSlot = EEquipmentSlots::None;
}

void UCharacterEquipmentComponent::EquipNextItem()
{
	uint32 CurrentSlotIndex = (uint32)CurrentEquippedSlot;
	uint32 NextSlotIndex = NextItemsArraySlotIndex(CurrentSlotIndex); // ++
	while (CurrentSlotIndex != NextSlotIndex && !IsValid(ItemsArray[NextSlotIndex])) // this loop help us to  prevent empty arms selection
	{
		NextSlotIndex = NextItemsArraySlotIndex(NextSlotIndex);
	}
	if (CurrentSlotIndex != NextSlotIndex)
	{
		EquipItemInSlot((EEquipmentSlots)NextSlotIndex);
	}
}

void UCharacterEquipmentComponent::EquipPreviousItem()
{
	uint32 CurrentSlotIndex = (uint32)CurrentEquippedSlot;
	uint32 PreviousSlotIndex = PreviousItemsArraySlotIndex(CurrentSlotIndex);
	while (CurrentSlotIndex != PreviousSlotIndex && !IsValid(ItemsArray[PreviousSlotIndex])) // this loop help us to  prevent empty arms selection
	{
		PreviousSlotIndex = PreviousItemsArraySlotIndex(PreviousSlotIndex);
	}
	if (CurrentSlotIndex != PreviousSlotIndex)
	{
		EquipItemInSlot((EEquipmentSlots)PreviousSlotIndex);
	}
}

void UCharacterEquipmentComponent::OnWeaponReloadComplete()
{
	// Callback

	int32 AvailableAmunition = GetAvailableAmunitionForCurrentWeapon();
	int32 CurrentAmmo = CurrentEquippedWeapon->GetAmmo();
	int32 AmmoToReload = CurrentEquippedWeapon->GetMaxAmmo() - CurrentAmmo; // how much shoots we have already done
	int32 ReloadedAmmo = FMath::Min(AvailableAmunition, AmmoToReload); // When we have last magazine and have more shot ammo then add the remaining ammo

	AmunitionArray[(uint32)CurrentEquippedWeapon->GetAmmoType()] -= ReloadedAmmo; // Reduce shot ammo from overall ammo number for particular weapon
	CurrentEquippedWeapon->SetAmmo(ReloadedAmmo + CurrentAmmo); // to fullfill the magazine
}

void UCharacterEquipmentComponent::CreateLoadOut()
{
	AmunitionArray.AddZeroed((uint32)EAmunitionType::MAX);
	for (const TPair<EAmunitionType, int32>& AmmoPair : MaxAmunitionAmount)
	{
		AmunitionArray[(uint32)AmmoPair.Key] = FMath::Max(AmmoPair.Value, 0);
	}

	ItemsArray.AddZeroed((uint32)EEquipmentSlots::MAX);
	for (const TPair<EEquipmentSlots, TSubclassOf<AEquipableItems>>& ItemPair : ItemsLoadout)
	{
		if (!IsValid(ItemPair.Value))
		{
			continue; // if we don't have a particular weapon just continue
		}
		AEquipableItems* Item = GetWorld()->SpawnActor<AEquipableItems>(ItemPair.Value);
		Item->AttachToComponent(InBaseCharacter->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, Item->GetUnEquippedSocketName());
		Item->SetOwner(InBaseCharacter.Get());
		ItemsArray[(uint32)ItemPair.Key] = Item;
	}


}

uint32 UCharacterEquipmentComponent::NextItemsArraySlotIndex(uint32 CurrentSlotIndex)
{
	if (CurrentSlotIndex == ItemsArray.Num() - 1)
	{
		return 0;
	}
	else
	{
		return CurrentSlotIndex + 1;
	}
	
}

uint32 UCharacterEquipmentComponent::PreviousItemsArraySlotIndex(uint32 CurrentSlotIndex)
{
	if (CurrentSlotIndex == 0)
	{
		return ItemsArray.Num() - 1;
	}
	else
	{
		return CurrentSlotIndex - 1;
	}
	
}

void UCharacterEquipmentComponent::EquipAnimationFinished()
{
	bIsEquipping = false;
	AttachCurrentItemToEquippedSocket();
}

void UCharacterEquipmentComponent::OnCurrentWeaponUpdatedAmmo(int32 Ammo)
{
	// The function is called back from ARangeWeapon::SetAmmo and calls back the UAmmoWidget::UpdateAmmoCount from ATPSPlayerController object
	if (OnCurrentWeaponAmmoChangedEvent.IsBound())
	{
		OnCurrentWeaponAmmoChangedEvent.Broadcast(Ammo, GetAvailableAmunitionForCurrentWeapon());
	}

}

int32 UCharacterEquipmentComponent::GetAvailableAmunitionForCurrentWeapon()
{
	checkf(GetCurrentWeapon(), TEXT("UCharacterEquipmentComponent::GetAvailableAmunitionForCurrentWeapon(ARangeWeapon* RangeWeaponItem) We don't have any weapon"));
	return AmunitionArray[(uint32)GetCurrentWeapon()->GetAmmoType()];
}


