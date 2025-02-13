// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Controllers/TPSAIController.h"
#include "TPSAICharacterController.generated.h"


class ATPSAICharacter;

UCLASS()
class TPSSHOOTER_API ATPSAICharacterController : public ATPSAIController
{
	GENERATED_BODY()

public:

	virtual void SetPawn(APawn* InPawn) override;

	// default method of AIController, should be overriden to define the logic of AI character when it reaches the patrolling path way point
	virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	float TargetReachRadius = 50.0f;

	virtual void BeginPlay() override;
	virtual void ActorsPerceptionUpdated(const TArray<AActor*>& UpdatedActors) override;

private:
	void TryMoveToNextTarget();
	bool IsTargetReached(FVector TargetLocation) const;

	TWeakObjectPtr<ATPSAICharacter> CachedAICharacter;
	bool bIsPatrolling = false;
};
