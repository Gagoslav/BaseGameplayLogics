// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseData/BaseEnums.h"
#include "EquipableItems.generated.h"

// Just a base abstract class designed to be inherit from by all equipable objects

class UAnimMontage;

UCLASS(Abstract, NotBlueprintable)
class TPSSHOOTER_API AEquipableItems : public AActor
{
	GENERATED_BODY()

public:
	EEquipedItemType GetEquipedItemType() const { return EquipedItemType; }

	inline FName GetUnEquippedSocketName() const { return UnEquippedSocketName; }
	inline FName GetEquippedSocketName() const{ return EquippedSocketName; }

	inline UAnimMontage* GetCharacterAnimMontage() const { return CharacterEquipAnimMontage; }

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ItemType")
	// Each equipable item has its type that should be set in editor
	EEquipedItemType EquipedItemType;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ItemType")
	FName UnEquippedSocketName = NAME_None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ItemType")
	FName EquippedSocketName = NAME_None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ItemType")
	UAnimMontage* CharacterEquipAnimMontage;

};
