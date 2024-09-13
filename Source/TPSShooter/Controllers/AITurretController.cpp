// Fill out your copyright notice in the Description page of Project Settings.


#include "Controllers/AITurretController.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AIPerceptionComponent.h"
#include "AI/Characters/Turret.h"


void AAITurretController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);
	if (IsValid(InPawn))
	{
		// The pawn under this controller must be of ATurret class only
		checkf(InPawn->IsA<ATurret>(), TEXT("AAITurretController::SetPawn AAITurretController can possess only ATurret"));
		CachedTurret = StaticCast<ATurret*>(InPawn);
	}
	else
	{
		CachedTurret = nullptr;
	}
}

void AAITurretController::ActorsPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	
	Super::ActorsPerceptionUpdated(UpdatedActors);
	AActor* ClosestActor = GetClosestSensedActor(UAISense_Sight::StaticClass());
	
	// Initialize Current target of cached turret
	CachedTurret->SetCurrentTarget(ClosestActor);
}
