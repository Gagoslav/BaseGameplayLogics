// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PatrollingPathComponent.generated.h"

class APatrollingPath;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TPSSHOOTER_API UPatrollingPathComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	bool CanPatroll() const;
	FVector SelectClosestWaypoint();
	FVector SelectNextWaypoint();

protected:

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Path")
	APatrollingPath* PatrollingPath;

private:
	int32 CurrentWaypointIndex = -1;
		
};
