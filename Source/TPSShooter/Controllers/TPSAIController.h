// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "TPSAIController.generated.h"


class UAISense;

UCLASS()
class TPSSHOOTER_API ATPSAIController : public AAIController
{
	GENERATED_BODY()

public:
	ATPSAIController();

protected:

	// This is main function of base AI controller class that uses GetCurrentlyPercievedActors under hood and 
	AActor* GetClosestSensedActor(TSubclassOf<UAISense> SenseClass) const;
	
	
};
