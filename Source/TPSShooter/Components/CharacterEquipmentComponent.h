// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BaseData/BaseEnums.h"
#include "CharacterEquipmentComponent.generated.h"


typedef TArray <int32, TInlineAllocator<(uint32)EAmunitionType::MAX>> TAmunitionArray;

class ARangeWeapon;
enum class EEquipedItemType : uint8;

DECLARE_MULTICAST_DELEGATE_TwoParams (FOnCurrentWeaponAmmoChanged, int32, int32)

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TPSSHOOTER_API UCharacterEquipmentComponent : public UActorComponent
{
	GENERATED_BODY()

public:

    EEquipedItemType GetCurrentEquipedItemType() const;
	virtual void BeginPlay()override;
	inline ARangeWeapon* GetCurrentWeapon() const { return CurrentEquippedWeapon; }

	void ReloadCurrentWeapon();

	FOnCurrentWeaponAmmoChanged OnCurrentWeaponAmmoChangedEvent;


protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Loadout")
	// Pointer to any class that is derived from ARangeWeapon!
	TSubclassOf<ARangeWeapon> SideArmClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Loadout")
	TMap<EAmunitionType, int32> MaxAmunitionAmount;

private:

	TAmunitionArray AmunitionArray;

	UFUNCTION()
	void OnWeaponReloadComplete();

	void CreateLoadOut();

	UFUNCTION()
	// UFunction called back from ARangeWeapon by delegate 
	void OnCurrentWeaponUpdatedAmmo(int32 Ammo);

	int32 GetAvailableAmunitionForCurrentWeapon();

	// Private pointer to weapon that will be initialized by spawning an object of SideArmClass
	ARangeWeapon* CurrentEquippedWeapon;

	// Cached character owner of this component
	TWeakObjectPtr<class ATPSBaseCharacter> InBaseCharacter;


		
};
