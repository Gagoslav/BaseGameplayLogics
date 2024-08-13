// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseData/BaseEnums.h"
#include "EquipableItems.generated.h"

// Just a base abstract class designed to be inherit from by all equipable objects

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEquipmentStateChanged, bool, bIsEquipped);
class UAnimMontage;
class ATPSBaseCharacter;

UCLASS(Abstract, NotBlueprintable)
class TPSSHOOTER_API AEquipableItems : public AActor
{
	GENERATED_BODY()

public:
	virtual void SetOwner(AActor* NewOwner) override;

	EEquipedItemType GetEquipedItemType() const { return EquipedItemType; }

	inline FName GetUnEquippedSocketName() const { return UnEquippedSocketName; }
	inline FName GetEquippedSocketName() const{ return EquippedSocketName; }

	// Base implementation is Delegate execution
	virtual void Equip();
	virtual void UnEquip();

	// It will behave a bit differently in ARangeWeapon
	virtual EReticleType GetReticleType() const{ return ReticleType; }

	inline UAnimMontage* GetCharacterAnimMontage() const { return CharacterEquipAnimMontage; }

protected:

	UPROPERTY(BlueprintAssignable)
	FOnEquipmentStateChanged OnEquipmentStateChanged;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ItemType")
	// Each equipable item has its type that should be set in editor
	EEquipedItemType EquipedItemType;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ItemType")
	// Socket is set at position on the skeletal mesh where we need to just store an item
	FName UnEquippedSocketName = NAME_None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ItemType")
	// Socket is set at position on the skeletal mesh where we need to equip an item
	FName EquippedSocketName = NAME_None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ItemType")
	UAnimMontage* CharacterEquipAnimMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Reticle")
	EReticleType ReticleType = EReticleType::None;

	ATPSBaseCharacter* GetCharacterOwner() const;

private:

	TWeakObjectPtr<ATPSBaseCharacter> CachedCharacterOwner;
};
