// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Characters/TPSAICharacter.h"
#include "Components/PatrollingPathComponent.h"

ATPSAICharacter::ATPSAICharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

	// Just initialized our patrolling path component, default for an AI character
	AIPatrollingComponent = CreateDefaultSubobject<UPatrollingPathComponent>(TEXT("AIPatrollingComponent"));
}
