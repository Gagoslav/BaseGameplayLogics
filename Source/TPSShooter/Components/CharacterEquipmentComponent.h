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
	inline ARangeWeapon* GetCurrentWeapon() const { return CurrentEquippedWeapon; }

	

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Loadout")
	// Pointer to any class that is derived from ARangeWeapon!
	TSubclassOf<ARangeWeapon> SideArmClass;

private:
	void CreateLoadOut();

	// Private pointer to weapon that will be initialized by spawning an object of SideArmClass
	ARangeWeapon* CurrentEquippedWeapon;

	// Cached character owner of this component
	TWeakObjectPtr<class ATPSBaseCharacter> InBaseCharacter;


		
};
