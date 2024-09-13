// Fill out your copyright notice in the Description page of Project Settings.


#include "Controllers/TPSAIController.h"
#include "Perception/AIPerceptionComponent.h"

ATPSAIController::ATPSAIController()
{
	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));

}

AActor* ATPSAIController::GetClosestSensedActor(TSubclassOf<UAISense> SenseClass) const
{

	if (!IsValid(GetPawn()))
	{
		return nullptr;
	}

	TArray<AActor*>SensedActors;

	// Initializing the SeenActors (GetCurrentlyPerceivedActors takes OUT reference), PerceptionComponent is default field of AAIController
	PerceptionComponent->GetCurrentlyPerceivedActors(SenseClass, SensedActors);

	AActor* ClosestActor = nullptr;
	float MinSquaredDistance = FLT_MAX;
	FVector PawnLocation = GetPawn()->GetActorLocation();

	// Iterate all seen actors and find the closest one
	for (AActor* SeenActor : SensedActors)
	{
		float CurrentSquaredDistance = (PawnLocation - SeenActor->GetActorLocation()).SizeSquared();
		if (CurrentSquaredDistance < MinSquaredDistance)
		{
			MinSquaredDistance = CurrentSquaredDistance;
			ClosestActor = SeenActor;
		}
	}

	return ClosestActor;
} 
