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

void UCharacterEquipmentComponent::Fire()
{
	if (IsValid(CurrentEquippedWeapon))
	{
		CurrentEquippedWeapon->Fire();
	}
}

void UCharacterEquipmentComponent::CreateLoadOut()
{
	if (!IsValid(SideArmClass))
	{
		return;
	}
	// Spawn the weapon item that we heve chosen from editor
	CurrentEquippedWeapon = GetWorld()->SpawnActor<ARangeWeapon>(SideArmClass);
	CurrentEquippedWeapon->AttachToComponent(InBaseCharacter->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, SocketCharacterWeapon);
	// As owner should be hard object pointer we use Get() to case a SoftObjectPtr to simple ptr
	CurrentEquippedWeapon->SetOwner(InBaseCharacter.Get());
}


// Called every frame
