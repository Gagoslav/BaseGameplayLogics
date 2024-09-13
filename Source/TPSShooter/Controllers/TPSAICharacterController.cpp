// Fill out your copyright notice in the Description page of Project Settings.


#include "Controllers/TPSAICharacterController.h"
#include"AI/Characters/TPSAICharacter.h"
#include "Perception/AISense_Sight.h"
#include "Components/PatrollingPathComponent.h"

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
		MoveToLocation(ClosestWayPoint);
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
	AActor* ClosestActor = GetClosestSensedActor(UAISense_Sight::StaticClass());
	if (IsValid(ClosestActor))
	{
		if (!IsTargetReached(ClosestActor->GetActorLocation()))
		{
			MoveToActor(ClosestActor); // This includes our chaarcter as well
		}
		bIsPatrolling = false;
	}
	else if(PatrollingComponent->CanPatroll())
	{
		FVector WayPoint = bIsPatrolling? PatrollingComponent->SelectNextWaypoint() : PatrollingComponent->SelectClosestWaypoint();
		if (!IsTargetReached(WayPoint))
		{
			MoveToLocation(WayPoint); // this is patrolling path's waypoint
		}
		bIsPatrolling = true;
	}
}

bool ATPSAICharacterController::IsTargetReached(FVector TargetLocation) const
{
	return (TargetLocation - CachedAICharacter->GetActorLocation()).SizeSquared() <= FMath::Square(TargetReachRadius);
}
