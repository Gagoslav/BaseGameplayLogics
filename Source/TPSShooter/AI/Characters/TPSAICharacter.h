// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/TPSBaseCharacter.h"
#include "TPSAICharacter.generated.h"


class UPatrollingPathComponent;
class UBehaviorTree;

UCLASS(Blueprintable)
class TPSSHOOTER_API ATPSAICharacter : public ATPSBaseCharacter
{
	GENERATED_BODY()
public:
	// As we have changed the default character movement component to our custom created one and used FObjectInitializer in TPSBaseCharacter,
	// we need to use the same constructor
	ATPSAICharacter(const FObjectInitializer & ObjectInitializer);

	
	UPatrollingPathComponent* GetPatrollingPath() const {  return AIPatrollingComponent;}
	UBehaviorTree* GetBehaviorTree() const { return BehaviorTree; }

protected:

	// Declaring main components appropriate fpr AI character
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UPatrollingPathComponent* AIPatrollingComponent;


	// Add behavior tree
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	UBehaviorTree* BehaviorTree;
		
};
