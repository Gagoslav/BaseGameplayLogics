// Copyright Epic Games, Inc. All Rights Reserved.

#include "TPSShooterGameMode.h"
#include "TPSShooterCharacter.h"
#include "UObject/ConstructorHelpers.h"

ATPSShooterGameMode::ATPSShooterGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
