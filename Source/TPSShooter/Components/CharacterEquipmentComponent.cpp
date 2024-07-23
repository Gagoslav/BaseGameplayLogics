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
	checkf(IsValid(CurrentEquippedWeapon), TEXT("UCharacterEquipmentComponent::ReloadCurrentWeapon() should have Valid weapon"));
	int32 AvailableAmunition = GetAvailableAmunitionForCurrentWeapon();
	if (AvailableAmunition <= 0)
	{
		return;
	}
	CurrentEquippedWeapon->StartReload();




}





void UCharacterEquipmentComponent::OnWeaponReloadComplete()
{
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

	if (!IsValid(SideArmClass))
	{
		return;
	}
	// Spawn the weapon item that we heve chosen from editor
	CurrentEquippedWeapon = GetWorld()->SpawnActor<ARangeWeapon>(SideArmClass);
	CurrentEquippedWeapon->AttachToComponent(InBaseCharacter->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, SocketCharacterWeapon);
	// Here we set an owner for our spawned weapon that is a character
	// As owner should be hard object pointer we use Get() to case a SoftObjectPtr to simple ptr
	CurrentEquippedWeapon->SetOwner(InBaseCharacter.Get());

	// Bind the delegate of ARangeWeapon to the OnCurrentWeaponUpdatedAmmo function of UCharacterEquipmentComponent
	CurrentEquippedWeapon->OnAmmoChangedEvent.AddUFunction(this, FName("OnCurrentWeaponUpdatedAmmo"));
	CurrentEquippedWeapon->OnReloadCompleteEvent.AddUFunction(this, FName("OnWeaponReloadComplete"));

	// Call this method also during weapon creation to have an initial amount of bullets
	OnCurrentWeaponUpdatedAmmo(CurrentEquippedWeapon->GetAmmo());


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


