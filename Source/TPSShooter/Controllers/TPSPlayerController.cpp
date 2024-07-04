 // Fill out your copyright notice in the Description page of Project Settings.


#include "Controllers/TPSPlayerController.h"
#include "Character/TPSBaseCharacter.h"

void ATPSPlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);
	// Use dynamic cast Not to crash if we have invalid type
	InBaseCharacter = Cast<ATPSBaseCharacter>(InPawn);

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
	InputComponent->BindAction("Fire", EInputEvent::IE_Pressed, this, &ATPSPlayerController::Fire);

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

void ATPSPlayerController::Fire()
{
	if (InBaseCharacter.IsValid())
	{
		InBaseCharacter->Fire();
	}
}
