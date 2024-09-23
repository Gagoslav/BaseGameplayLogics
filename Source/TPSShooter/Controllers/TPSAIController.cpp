// Fill out your copyright notice in the Description page of Project Settings.


#include "Controllers/TPSAIController.h"
#include "Perception/AIPerceptionComponent.h"

ATPSAIController::ATPSAIController()
{
	// In constructor we only initialize the perception component
	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));

}

AActor* ATPSAIController::GetClosestSensedActor(TSubclassOf<UAISense> SenseClass) const
{

	if (!IsValid(GetPawn()))
	{
		return nullptr;
	}

	// Declare an array where we will store all sensed actors
	TArray<AActor*>SensedActors;

	// Initializing the SeenActors (GetCurrentlyPerceivedActors takes OUT reference), PerceptionComponent is default field of AAIController
	PerceptionComponent->GetCurrentlyPerceivedActors(SenseClass, SensedActors);

	AActor* ClosestActor = nullptr;
	float MinSquaredDistance = FLT_MAX;
	FVector PawnLocation = GetPawn()->GetActorLocation();

	// Iterate all seen actors and find the closest one
	for (AActor* SeenActor : SensedActors)
	{
		// it's preferable to count in squared to prevent from extra calculations of root
		float CurrentSquaredDistance = (PawnLocation - SeenActor->GetActorLocation()).SizeSquared();
		if (CurrentSquaredDistance < MinSquaredDistance)
		{
			MinSquaredDistance = CurrentSquaredDistance;
			ClosestActor = SeenActor;
		}
	}

	return ClosestActor;
} 
