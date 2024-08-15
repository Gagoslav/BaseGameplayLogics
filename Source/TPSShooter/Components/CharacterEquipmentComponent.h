// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BaseData/BaseEnums.h"
#include "CharacterEquipmentComponent.generated.h"



typedef TArray <int32, TInlineAllocator<(uint32)EAmunitionType::MAX>> TAmunitionArray;
typedef TArray <class AEquipableItems*, TInlineAllocator<(uint32)EEquipmentSlots::MAX>> TItemsArray;

class AThrowableItem;
class ARangeWeapon;
class AMeleeEquipableItems;
enum class EEquipedItemType : uint8;

DECLARE_MULTICAST_DELEGATE_TwoParams (FOnCurrentWeaponAmmoChanged, int32, int32)
DECLARE_MULTICAST_DELEGATE_OneParam (FOnEquippedItemChanged, const AEquipableItems*)

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TPSSHOOTER_API UCharacterEquipmentComponent : public UActorComponent
{
	GENERATED_BODY()

public:

    EEquipedItemType GetCurrentEquipedItemType() const;
	virtual void BeginPlay()override;
	inline ARangeWeapon* GetCurrentWeapon() const { return CurrentEquippedWeapon; }

	void ReloadCurrentWeapon();

	void ReloadAmmoInCurrentWeapon(int32 NumberOfAmmo = 0, bool bCheckIsFull = false); // For shotgun

	bool CanReload(int32 AmmoNum);

	FOnCurrentWeaponAmmoChanged OnCurrentWeaponAmmoChangedEvent;
	FOnEquippedItemChanged OnEquippedItemChanged;

	// Method responsible for equiping of selected item/weapon on character's hand
	void EquipItemInSlot(EEquipmentSlots Slot);

	void AttachCurrentItemToEquippedSocket(); 

	void LaunchCurrentThrowableItem();

	void UnEquipCurrentItem();

	// Methods that switch weapons
	void EquipNextItem();
	void EquipPreviousItem();
	
	inline bool GetIsEquipping() const { return bIsEquipping; }
	inline AMeleeEquipableItems* GetCurrentMeleeItem() const { return CurrentMeleeItem; }


protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Loadout")
	TMap<EAmunitionType, int32> MaxAmunitionAmount;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Loadout")
	TMap<EEquipmentSlots, TSubclassOf<class AEquipableItems>> ItemsLoadout;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Loadout")
	TSet<EEquipmentSlots> IgnoreSlotsWhileSwitching;

private:

	void CreateLoadOut();
	
	TAmunitionArray AmunitionArray;
	TItemsArray ItemsArray;

	UFUNCTION()
	void OnWeaponReloadComplete();

	UFUNCTION()
	// UFunction called back from ARangeWeapon by delegate 
	void OnCurrentWeaponUpdatedAmmo(int32 Ammo);

	uint32 NextItemsArraySlotIndex(uint32 CurrentSlotIndex);
	uint32 PreviousItemsArraySlotIndex(uint32 CurrentSlotIndex);
	void EquipAnimationFinished();

	bool bIsEquipping = false;
	
	FDelegateHandle OnCurrentWeaponAmmoChangedHandle;
	FDelegateHandle OnCurrentWeaponReloadedHandle;

	int32 GetAvailableAmunitionForCurrentWeapon();

	// Private pointer to weapon that will be initialized by spawning an object of SideArmClass
	ARangeWeapon* CurrentEquippedWeapon;

	EEquipmentSlots PreviousEquippedSlot;
	EEquipmentSlots CurrentEquippedSlot;

	AMeleeEquipableItems* CurrentMeleeItem;
	AEquipableItems* CurrentEquippedItem;
	AThrowableItem* CurrentThrowableItem;

	// Cached character owner of this component
	TWeakObjectPtr<class ATPSBaseCharacter> InBaseCharacter;

	FTimerHandle EquipTimer;
		
};
