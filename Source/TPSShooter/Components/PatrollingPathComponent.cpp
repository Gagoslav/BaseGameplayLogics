// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/PatrollingPathComponent.h"
#include "Actors/Navigation/PatrollingPath.h"

bool UPatrollingPathComponent::CanPatroll() const
{
	return IsValid(PatrollingPath) && PatrollingPath->GetWayPoints().Num() > 0;
}

FVector UPatrollingPathComponent::SelectClosestWaypoint()
{

	// Get AI character location and get transform of Patrolling path, to be able to change the local coordinates of waypoints to the global ones
	FVector OwnerLocation = GetOwner()->GetActorLocation();
	const TArray<FVector>WayPoints = PatrollingPath->GetWayPoints();
	FTransform PathTransform = PatrollingPath->GetActorTransform();

	FVector ClosestWayPoint;
	float MinSqrdDist = FLT_MAX;

	for (int32 i = 0; i < WayPoints.Num(); ++i)
	{

		FVector WayPointWorld = PathTransform.TransformPosition(WayPoints[i]);
		float CurrentSqrdDistance = (OwnerLocation - WayPointWorld).SizeSquared();
		if (CurrentSqrdDistance < MinSqrdDist)
		{
			MinSqrdDist = CurrentSqrdDistance;
			ClosestWayPoint = WayPointWorld; // set world coordinates of closest to AI character way point
			CurrentWaypointIndex = i; // get that waypoint's index
		}
	}
	return ClosestWayPoint;
}

FVector UPatrollingPathComponent::SelectNextWaypoint()
{
	// Just get next waypoint index and get world location of that waypoint
	++CurrentWaypointIndex;
	const TArray<FVector>WayPoints = PatrollingPath->GetWayPoints();
	if (CurrentWaypointIndex == PatrollingPath->GetWayPoints().Num())
	{
		CurrentWaypointIndex = 0;
	}
	FTransform PathTransform = PatrollingPath->GetActorTransform();
	FVector WayPoint = PathTransform.TransformPosition(WayPoints[CurrentWaypointIndex]);
	return WayPoint;
}
