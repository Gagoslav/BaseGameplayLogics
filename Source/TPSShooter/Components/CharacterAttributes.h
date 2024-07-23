// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CharacterAttributes.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnDeathEventSignature);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TPSSHOOTER_API UCharacterAttributes : public UActorComponent
{
	GENERATED_BODY()

public:	
	
	UCharacterAttributes();
	// Just for debugging purposes
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Declare an instance of delegate (EVENT)
	FOnDeathEventSignature OnDeathEvent;

	inline bool IsAlive() { return Health > 0.0f; }
	inline float GetHealthPercent() const { return Health / MaxHealth; }

protected:
	
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Health", meta = (UIMin = 0.0f))
	float MaxHealth = 100.0f;

private:
	float Health = 0.0f;
	
#if UE_BUILD_DEBUG || UE_BUILD_DEVELOPMENT

	void DebugDrawAttributes();

#endif

	UFUNCTION()
	void OnTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	TWeakObjectPtr<class ATPSBaseCharacter> InBaseCharacter;
};
