// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTasks/BTTask_RandomPointAroundTarget.h"
#include "BTTask_RandomPointAroundTarget.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"

UBTTask_RandomPointAroundTarget::UBTTask_RandomPointAroundTarget()
{
	// Here we make only cosmetic change, to see the node in editor witha new name
	NodeName = "Random point around target";
}

EBTNodeResult::Type UBTTask_RandomPointAroundTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Here we don't call base method's implementation as it returns EBTNodeResult::Success by default

	AAIController* AIController = OwnerComp.GetAIOwner(); // Get AIController from BehaviorTree
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent(); //  Get Blackboard from BehaviorTree
	if (!IsValid(AIController) || !IsValid(Blackboard))
	{
		return EBTNodeResult::Failed;
	}

	// Get AI pawn
	APawn* Pawn = AIController->GetPawn();
	if (!IsValid(Pawn))
	{
		return EBTNodeResult::Failed;
	}

	// For getting navigation system we should #include "NavigationSystem.h"
	UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(Pawn);
	if (!IsValid(NavSys))
	{
		return EBTNodeResult::Failed;
	}

	// Initialize Target actor as a Blackboard's key of object type, casted to actor
	AActor* TargetActor = Cast<AActor>(Blackboard->GetValueAsObject(TargetKey.SelectedKeyName));
	if (IsValid(TargetActor))
	{
		return EBTNodeResult::Failed;
	}


	FNavLocation NavLocation;
	bool bIsFound = NavSys->GetRandomReachablePointInRadius(TargetActor->GetActorLocation(), Radius, NavLocation);
	if (!bIsFound)
	{
		return EBTNodeResult::Failed;
	}

	// Write the Blackboard's key in FVector type
	Blackboard->SetValueAsVector(LocationKey.SelectedKeyName, NavLocation.Location);
	return EBTNodeResult::Succeeded;
}


