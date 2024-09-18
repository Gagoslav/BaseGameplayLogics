// Fill out your copyright notice in the Description page of Project Settings.


#include "Controllers/TPSAICharacterController.h"
#include"AI/Characters/TPSAICharacter.h"
#include "Perception/AISense_Sight.h"
#include "Components/PatrollingPathComponent.h"
#include "BaseData/BaseEnums.h"
#include "BehaviorTree/BlackboardComponent.h"

void ATPSAICharacterController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);
	if (IsValid(InPawn))
	{
		checkf(InPawn->IsA<ATPSAICharacter>(), TEXT("ATPSAICharacterController::SetPawn(APawn* InPawn) InPawn should be only an object of ATPSAICharacter class"));
		CachedAICharacter = StaticCast<ATPSAICharacter*>(InPawn);
		// Default method of AIController, runs behavior tree that defines logic for AI character
		RunBehaviorTree(CachedAICharacter->GetBehaviorTree());
	}
	else
	{
		CachedAICharacter = nullptr;
	}
}

void ATPSAICharacterController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	Super::OnMoveCompleted(RequestID, Result);
	if (!Result.IsSuccess())
	{
		return;
	}
	TryMoveToNextTarget();
}

void ATPSAICharacterController::BeginPlay()
{
	Super::BeginPlay();
	// Initialize local Patrolling component
	UPatrollingPathComponent* PatrollingComponent = CachedAICharacter->GetPatrollingPath();
	if (PatrollingComponent->CanPatroll())
	{
		// Move pawn to the Closest waypoint
		FVector ClosestWayPoint = PatrollingComponent->SelectClosestWaypoint();
		if (IsValid(Blackboard))
		{
			// At the start of game we need to patrol, initialize the values of blackboard
			Blackboard->SetValueAsVector(BB_NextLocation, ClosestWayPoint);
			Blackboard->SetValueAsObject(BB_CurrentTarget, nullptr);
		}
		// Set Pawn is patrolling
		bIsPatrolling = true;
	}
}

void ATPSAICharacterController::ActorsPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	Super::ActorsPerceptionUpdated(UpdatedActors);
	if(!CachedAICharacter.IsValid())
	{
		return;
	}
	// On actor's perception updated continue moving along patrolling path
	TryMoveToNextTarget();
}

void ATPSAICharacterController::TryMoveToNextTarget()
{

	UPatrollingPathComponent* PatrollingComponent = CachedAICharacter->GetPatrollingPath();
	// As in ActorsPerceptionUpdated we need to Get closest sensed actor
	AActor* ClosestActor = GetClosestSensedActor(UAISense_Sight::StaticClass()); // initialize 
	if (IsValid(ClosestActor))
	{
		if (IsValid(Blackboard))
		{
			// If AI character notices an enemy (player) then it runs to him
			Blackboard->SetValueAsObject(BB_CurrentTarget, ClosestActor);
			// and turns to him
			SetFocus(ClosestActor, EAIFocusPriority::Gameplay);
		}
		bIsPatrolling = false;
	}
	else if(PatrollingComponent->CanPatroll())
	{
		FVector WayPoint = bIsPatrolling? PatrollingComponent->SelectNextWaypoint() : PatrollingComponent->SelectClosestWaypoint();
		if (IsValid(Blackboard))
		{
			ClearFocus(EAIFocusPriority::Gameplay);
			// Otherwise AI character continues patrolling
			Blackboard->SetValueAsVector(BB_NextLocation, WayPoint);
			Blackboard->SetValueAsObject(BB_CurrentTarget, nullptr);
		}
		bIsPatrolling = true;
	}
}

bool ATPSAICharacterController::IsTargetReached(FVector TargetLocation) const
{
	return (TargetLocation - CachedAICharacter->GetActorLocation()).SizeSquared() <= FMath::Square(TargetReachRadius);
}
