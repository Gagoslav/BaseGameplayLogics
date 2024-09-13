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

protected:

	virtual void ActorsPerceptionUpdated(const TArray<AActor*>& UpdatedActors) override;

private:
	TWeakObjectPtr<ATPSAICharacter> CachedAICharacter;
};
