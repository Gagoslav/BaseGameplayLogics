// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_RandomPointAroundTarget.generated.h"




UCLASS()
class TPSSHOOTER_API UBTTask_RandomPointAroundTarget : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	// In constructor we only initialize the node name
	UBTTask_RandomPointAroundTarget();

	// Main virtual function of UBTTaskNode that must be overriden in derived classes
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	// Radius that the AI character will stop within as soon as it reaches its target
	float Radius = 500.0f;

	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	FBlackboardKeySelector TargetKey;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	FBlackboardKeySelector LocationKey;
	
	// FBlackboardKeySelector is a structure that can hold a pointer to a Blackboard Key, that will be read from Blackboard's blueprint
};
