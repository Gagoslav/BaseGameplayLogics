// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RaticleWidget.generated.h"

class AEquipableItems;
enum class EReticleType : uint8;


UCLASS()
class TPSSHOOTER_API URaticleWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintNativeEvent)
	// Event binds to the delegate when character is aiming
	void OnAimingStateChanged(bool bIsAiming);

	UFUNCTION(BlueprintNativeEvent)
	void OnEquippedItemChanged(const AEquipableItems* EquippedItem);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Reticle")
	EReticleType CurrentReticle;

private:
	TWeakObjectPtr<const AEquipableItems> CurrentEquippedItem;

	void SetupCurrentReticle();
};
