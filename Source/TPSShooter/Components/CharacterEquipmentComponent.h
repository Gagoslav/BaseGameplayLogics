// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CharacterEquipmentComponent.generated.h"

class ARangeWeapon;
enum class EEquipedItemType : uint8;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TPSSHOOTER_API UCharacterEquipmentComponent : public UActorComponent
{
	GENERATED_BODY()

public:

    EEquipedItemType GetCurrentEquipedItemType() const;
	virtual void BeginPlay()override;

	void Fire();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Loadout")
	TSubclassOf<ARangeWeapon> SideArmClass;

private:
	void CreateLoadOut();

	ARangeWeapon* CurrentEquippedWeapon;
	TWeakObjectPtr<class ATPSBaseCharacter> InBaseCharacter;


		
};
