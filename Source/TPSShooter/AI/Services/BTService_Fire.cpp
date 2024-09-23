// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Services/BTService_Fire.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/TPSBaseCharacter.h"
#include "Components/CharacterEquipmentComponent.h"
#include "Actors/Equipment/RangeWeapon.h"

UBTService_Fire::UBTService_Fire()
{
	NodeName = "Fire";
}

void UBTService_Fire::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds); 
	// The logic here is somehow similar to the one of UBT_Task

	AAIController* AIController = OwnerComp.GetAIOwner(); // Get AIController from BehaviorTree
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent(); //  Get Blackboard from BehaviorTree
	if (!IsValid(AIController) || !IsValid(Blackboard))
	{
		return;
	}

	checkf(AIController->GetPawn()->IsA<ATPSBaseCharacter>(), TEXT("UBTService_Fire::TickNode possessed by AIController's pawn should be of ATPSBaseCharacter type"));
	ATPSBaseCharacter* Character = StaticCast<ATPSBaseCharacter*>(AIController->GetPawn());

	// Get our possessed AI character's Weapon through its Equipment component
	const UCharacterEquipmentComponent* EquipmentComponent = Character->GetCharacterEquipmentComponent();
	ARangeWeapon* Weapon = EquipmentComponent->GetCurrentWeapon();

	if (!IsValid(Weapon))
	{
		return;
	}

	AActor* CurrentTarget = Cast<AActor>(Blackboard->GetValueAsObject(TargetKey.SelectedKeyName)); // instead of hardcoding the name of blackboard's variable
	if (!IsValid(CurrentTarget))
	{
		// it's better to call character's stop fire as there may be other logics implemented, before the weapon ends shooting
		Character->StopFire();  
		return;
	}

	// It's better to count in squred distance not to count root if we need just to compare distances
	float DistSq = FVector::DistSquared(CurrentTarget->GetActorLocation(), Character->GetActorLocation());
	if (DistSq > FMath::Square(MaxFireDistance))
	{
		Character->StopFire();
		return;
	}

	if (!Weapon->IsReloading() || Weapon->IsFiring())
	{
		Character->StartFire();
	}
}
