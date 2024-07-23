// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHUDWidget.generated.h"

class URaticleWidget;
class UAmmoWidget;

/**
 * 
 */
UCLASS()
class TPSSHOOTER_API UPlayerHUDWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	// Getter that will use WidgetTree to find the appropriate widget by its name (FName RaticalWidgetName)
	URaticleWidget* GetRaticleWidget() const;
	UAmmoWidget* GetAmmoWidget() const;
protected:
	UFUNCTION(BlueprintCallable)
	float GetHealthPercent() const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget names")
	FName RaticalWidgetName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget names")
	FName AmmoWidgetName;
	
};
