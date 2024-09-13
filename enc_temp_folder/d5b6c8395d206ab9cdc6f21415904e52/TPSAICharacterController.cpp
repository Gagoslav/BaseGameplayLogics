// Fill out your copyright notice in the Description page of Project Settings.


#include "Controllers/TPSAICharacterController.h"
#include"AI/Characters/TPSAICharacter.h"
#include "Perception/AISense_Sight.h"

void ATPSAICharacterController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);
	if (IsValid(InPawn))
	{
		checkf(InPawn->IsA<ATPSAICharacter>(), TEXT("ATPSAICharacterController::SetPawn(APawn* InPawn) InPawn should be only an object of ATPSAICharacter class"));
		CachedAICharacter = StaticCast<ATPSAICharacter*>(InPawn);
	}
	else
	{
		CachedAICharacter = nullptr;
	}
}

void ATPSAICharacterController::ActorsPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	Super::ActorsPerceptionUpdated(UpdatedActors);
	if(!CachedAICharacter.IsValid())
	{
		return;
	}

	AActor* ClosestActor = GetClosestSensedActor(UAISense_Sight::StaticClass());
	if (IsValid(ClosestActor))
	{
		// Is a default method of AIController class, moves a pawn to actor or location
		MoveToActor(ClosestActor);
	}
}
