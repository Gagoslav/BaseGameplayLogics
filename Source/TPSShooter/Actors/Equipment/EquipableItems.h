// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseData/BaseEnums.h"
#include "EquipableItems.generated.h"


UCLASS(Abstract, NotBlueprintable)
class TPSSHOOTER_API AEquipableItems : public AActor
{
	GENERATED_BODY()

public:
	EEquipedItemType GetEquipedItemType() const { return EquipedItemType; }
protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ItemType")
	EEquipedItemType EquipedItemType;

	

};
