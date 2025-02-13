// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Controllers/TPSAIController.h"
#include "AITurretController.generated.h"


class ATurret;

UCLASS()
class TPSSHOOTER_API AAITurretController : public ATPSAIController
{
	GENERATED_BODY()

public:

	// Default function for all controller classes
	virtual void SetPawn(APawn* InPawn) override;
	virtual void ActorsPerceptionUpdated(const TArray<AActor*>& UpdatedActors) override;

private:
	TWeakObjectPtr<ATurret> CachedTurret;
	
};
