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
	AAIController* AIController = OwnerComp.GetAIOwner();
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	if (!IsValid(AIController) || !IsValid(Blackboard))
	{
		return;
	}

	ATPSBaseCharacter* Character = Cast<ATPSBaseCharacter>(AIController->GetPawn());
	if (!IsValid(Character))
	{
		return;
	}

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
