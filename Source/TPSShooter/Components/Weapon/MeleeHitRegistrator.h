// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "MeleeHitRegistrator.generated.h"

// As melee weapons use completely different logics for hit registration we create a distinct component for it 

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnMeleeHitRegistered, const FHitResult&, const FVector&);

UCLASS(meta = (BlueprintSpawnableComponent)) // To be able to add this component in blueprints
class TPSSHOOTER_API UMeleeHitRegistrator : public USphereComponent
{
	GENERATED_BODY()
public:

	UMeleeHitRegistrator();

	// This component can tick and implement and get it's location
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void ProcessHitRegistration();
	void SetIsHitRegistrationEnabled(bool bIsEnabled);

	FOnMeleeHitRegistered OnMeleeHitRegistered;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MeleeHitRegistration")
	bool bIsHitregistrationEnabled = false;

private:
	FVector PreviousComponentHitLocation = FVector::ZeroVector;
	
};
