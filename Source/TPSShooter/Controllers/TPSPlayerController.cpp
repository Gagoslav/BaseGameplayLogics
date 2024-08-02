// Fill out your copyright notice in the Description page of Project Settings.


#include "Controllers/TPSPlayerController.h"
#include "Character/TPSBaseCharacter.h"
#include "Blueprint/UserWidget.h"
#include "UI/Widget/PlayerHUDWidget.h"
#include "UI/Widget/RaticleWidget.h"
#include "UI/Widget/AmmoWidget.h"
#include "Components/CharacterEquipmentComponent.h"

void ATPSPlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);
	// Use dynamic cast Not to crash if we have invalid type
	InBaseCharacter = Cast<ATPSBaseCharacter>(InPawn);
	// By default pipeline the HUD widget is created and initialized (including bindings) in player controller class
	CreateAndInitializeWidgets();

}

void ATPSPlayerController::SetupInputComponent()
{
	// As with usual virtual methods first call base implementation
	Super::SetupInputComponent();

	// Start make Bindings with input component
	InputComponent->BindAxis("MoveForward", this, &ATPSPlayerController::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ATPSPlayerController::MoveRight);
	InputComponent->BindAxis("Turn", this, &ATPSPlayerController::Turn);
	InputComponent->BindAxis("LookUp", this, &ATPSPlayerController::LookUp);
	// Action InteractWithLadder must be called earlier than Mantle
	InputComponent->BindAction("InteractWithLadder", EInputEvent::IE_Pressed, this, &ATPSPlayerController::InteractWithLadder);
	// Action Mantle and Jump are bound to the same input, and as first we have mantle and after jump, mantle will be performed first
	InputComponent->BindAction("Mantle", EInputEvent::IE_Pressed, this, &ATPSPlayerController::Mantle);
	InputComponent->BindAction("Jump", EInputEvent::IE_Pressed, this, &ATPSPlayerController::Jump);
	InputComponent->BindAction("Crouch", EInputEvent::IE_Pressed, this, &ATPSPlayerController::ChangeCrouchState);
	InputComponent->BindAction("Sprint", EInputEvent::IE_Pressed, this, &ATPSPlayerController::StartSprint);
	InputComponent->BindAction("Sprint", EInputEvent::IE_Released, this, &ATPSPlayerController::EndSprint);
	InputComponent->BindAxis("SwimForward", this, &ATPSPlayerController::SwimForward);
	InputComponent->BindAxis("SwimRight", this, &ATPSPlayerController::SwimRight);
	InputComponent->BindAxis("SwimUp", this, &ATPSPlayerController::SwimUp);
	InputComponent->BindAxis("ClimbingLadderUp", this, &ATPSPlayerController::ClimbLadderUp);
	InputComponent->BindAction("Fire", EInputEvent::IE_Pressed, this, &ATPSPlayerController::PlayerStartFire);
	InputComponent->BindAction("Fire", EInputEvent::IE_Released, this, &ATPSPlayerController::PlayerStopFire);
	InputComponent->BindAction("Aim", EInputEvent::IE_Pressed, this, &ATPSPlayerController::StartAiming);
	InputComponent->BindAction("Aim", EInputEvent::IE_Released, this, &ATPSPlayerController::StopAiming);
	InputComponent->BindAction("Reload", EInputEvent::IE_Pressed, this, &ATPSPlayerController::Reload);
	InputComponent->BindAction("NextItem", EInputEvent::IE_Pressed, this, &ATPSPlayerController::NextItem);
	InputComponent->BindAction("PreviousItem", EInputEvent::IE_Pressed, this, &ATPSPlayerController::PreviousItem);
	InputComponent->BindAction("EquipPrimaryItem", EInputEvent::IE_Pressed, this, &ATPSPlayerController::EquipPrimaryItem);
}


// Implementation of methods within controller class supposes calls of appropriate
// methods within abstract Base Character class, that will call the ones in Player character


void ATPSPlayerController::MoveForward(float Value)
{
	if (InBaseCharacter.IsValid())
	{
		InBaseCharacter->MoveForward(Value);
	}

}

void ATPSPlayerController::MoveRight(float Value)
{
	if (InBaseCharacter.IsValid())
	{
		InBaseCharacter->MoveRight(Value);
	}
}

void ATPSPlayerController::Turn(float Value)
{
	if (InBaseCharacter.IsValid())
	{
		InBaseCharacter->Turn(Value);
	}
}

void ATPSPlayerController::LookUp(float Value)
{
	if (InBaseCharacter.IsValid())
	{
		InBaseCharacter->LookUp(Value);
	}
}

void ATPSPlayerController::Mantle()
{
	if (InBaseCharacter.IsValid())
	{
		InBaseCharacter->Mantle();
	}
}

void ATPSPlayerController::Jump()
{
	if (InBaseCharacter.IsValid())
	{
		InBaseCharacter->Jump();
	}
}

void ATPSPlayerController::ChangeCrouchState()
{
	if (InBaseCharacter.IsValid())
	{
		InBaseCharacter->ChangeCrouchState();
	}
}

void ATPSPlayerController::StartSprint()
{
	if (InBaseCharacter.IsValid())
	{
		InBaseCharacter->StartSprint();
	}
}

void ATPSPlayerController::EndSprint()
{
	if (InBaseCharacter.IsValid())
	{
		InBaseCharacter->EndSprint();
	}
}

void ATPSPlayerController::SwimForward(float Value)
{
	if (InBaseCharacter.IsValid())
	{
		InBaseCharacter->SwimForward(Value);
	}
}

void ATPSPlayerController::SwimRight(float Value)
{
	if (InBaseCharacter.IsValid())
	{
		InBaseCharacter->SwimRight(Value);
	}
}

void ATPSPlayerController::SwimUp(float Value)
{
	if (InBaseCharacter.IsValid())
	{
		InBaseCharacter->SwimUp(Value);
	}
}

void ATPSPlayerController::ClimbLadderUp(float Value)
{
	if (InBaseCharacter.IsValid())
	{
		InBaseCharacter->ClimbLadderUp(Value);
	}
}

void ATPSPlayerController::InteractWithLadder()
{
	if (InBaseCharacter.IsValid())
	{
		InBaseCharacter->InteractWithLadder();
	}
}

void ATPSPlayerController::PlayerStartFire()
{
	if (InBaseCharacter.IsValid())
	{
		InBaseCharacter->StartFire();
	}
}

void ATPSPlayerController::PlayerStopFire()
{
	if (InBaseCharacter.IsValid())
	{
		InBaseCharacter->StopFire();
	}
}

void ATPSPlayerController::StartAiming()
{
	if (InBaseCharacter.IsValid())
	{
		InBaseCharacter->StartAiming();
	}
}

void ATPSPlayerController::StopAiming()
{
	if (InBaseCharacter.IsValid())
	{
		InBaseCharacter->StopAiming();
	}
}

void ATPSPlayerController::Reload()
{
	if (InBaseCharacter.IsValid())
	{
		InBaseCharacter->Reload();
	}
}

void ATPSPlayerController::NextItem()
{
	if (InBaseCharacter.IsValid())
	{
		InBaseCharacter->NextItem();
	}
}

void ATPSPlayerController::PreviousItem()
{
	if (InBaseCharacter.IsValid())
	{
		InBaseCharacter->PreviousItem();
	}
}

void ATPSPlayerController::EquipPrimaryItem()
{
	if (InBaseCharacter.IsValid())
	{
		InBaseCharacter->EquipPrimaryItem();
	}
}

void ATPSPlayerController::CreateAndInitializeWidgets()
{
	// Make sure we haven't already created and added to viewport the HUD widget 
	if (!IsValid(PlayerHUDWidget))
	{
		PlayerHUDWidget = CreateWidget<UPlayerHUDWidget>(GetWorld(), PlayerHUDWidgetClass);
		if (IsValid(PlayerHUDWidget))
		{
			PlayerHUDWidget->AddToViewport();
		}
	}

	// Get Widgets that are within the HUD widget
	if (IsValid(PlayerHUDWidget)&& InBaseCharacter.IsValid())
	{
		URaticleWidget* RaticleWidget = PlayerHUDWidget->GetRaticleWidget();
		if (IsValid(RaticleWidget))
		{
			// Bind the delegate of ABaseCharacter with OnAimingStateChanged function of URaticleWidget
			InBaseCharacter->OnAimingStateChanged.AddUFunction(RaticleWidget, FName("OnAimingStateChanged"));
		}

		UAmmoWidget* AmmoWidget = PlayerHUDWidget->GetAmmoWidget();
		if (IsValid(AmmoWidget))
		{
			UCharacterEquipmentComponent* CharacterEquipment = InBaseCharacter->GetCharacterEquipmentComponent_Mutable();

			// Bind the delegate of UCharacterEquipmentComponent with UpdateAmmoCount function of UAmmoWidget
			CharacterEquipment->OnCurrentWeaponAmmoChangedEvent.AddUFunction(AmmoWidget, FName("UpdateAmmoCount"));
		}
	}


}
