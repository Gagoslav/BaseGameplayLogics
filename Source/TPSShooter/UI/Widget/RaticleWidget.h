// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RaticleWidget.generated.h"

/**
 * 
 */
UCLASS()
class TPSSHOOTER_API URaticleWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintImplementableEvent)
	// Event binds to the delegate when character is aiming
	void OnAimingStateChanged(bool bIsAiming);
};
