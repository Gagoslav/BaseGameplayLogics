// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_Fire.generated.h"

// Unlike UBT_Task the services run each tick 


UCLASS()
class TPSSHOOTER_API UBTService_Fire : public UBTService
{
	GENERATED_BODY()
	
public:
	// We use constructor only for setting a node's name that will be seen in blueprint editor
	UBTService_Fire();

protected:
	// Base virtual metod of UBTServise that must be overriden in derived classes
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	FBlackboardKeySelector TargetKey;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	float MaxFireDistance = 800.0f;
};
