// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/PlayerHUDWidget.h"
#include "Character/TPSBaseCharacter.h"
#include "Components/CharacterAttributes.h"
#include "UI/Widget/RaticleWidget.h"
#include "UI/Widget/AmmoWidget.h"
#include "Blueprint/WidgetTree.h"

URaticleWidget* UPlayerHUDWidget::GetRaticleWidget() const
{
	return WidgetTree->FindWidget<URaticleWidget>(RaticalWidgetName);
}

UAmmoWidget* UPlayerHUDWidget::GetAmmoWidget() const
{
	return WidgetTree->FindWidget<UAmmoWidget>(AmmoWidgetName);
}

float UPlayerHUDWidget::GetHealthPercent()const
{
	// Default is full HP
	float Result = 1.0f;
	APawn* Pawn = GetOwningPlayerPawn();
	ATPSBaseCharacter* Character = Cast<ATPSBaseCharacter>(Pawn);
	if (IsValid(Character))
	{
		// Get health percentage from Character attribute component
		Result = Character->GetCharacterAttributesComponent()->GetHealthPercent();
	}
	return Result;
}
