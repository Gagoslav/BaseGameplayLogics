// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PatrollingPath.generated.h"

UCLASS()
class TPSSHOOTER_API APatrollingPath : public AActor
{
	GENERATED_BODY()

public:
	const TArray<FVector>& GetWayPoints() const { return WayPoints; }
	
protected:
	
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Patrolling points", meta = (MakeEditWidget))
	TArray<FVector>WayPoints;

};
