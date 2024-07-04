// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LedgeDetectorComponent.generated.h"


USTRUCT(BlueprintType)
struct FLedgeDescription
{
	GENERATED_BODY()

	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ledge description")
	// Location of edge where our climbing character should get
	FVector Location;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ledge description")
	// Gets X/Y normal of ledge
	FVector LedgeNormal;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ledge description")
	// Orientation of edge to correctly rotate our climbing character to edge
	FRotator Rotation;

};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TPSSHOOTER_API ULedgeDetectorComponent : public UActorComponent
{
	GENERATED_BODY()
	

	

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Detection settings", meta = (ClampMin = 0, UIMin = 0))
	// Analog of MaxStepHeight of default character movement component, when we just walk
	// Minimal height of ledge that we are able to climb on it
	float MinimumLedgeHeight = 40.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Detection settings", meta = (ClampMin = 0, UIMin = 0))
	// Maximal height of ledge that we are able to climb on it
	float MaximumLedgeHeight = 200.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Detection settings", meta = (ClampMin = 0, UIMin = 0))
	// Offset from character for forward check implementation
	float ForwardCheckDistance = 100.0f;

public:	
	// Marcoss OUT does litterally nothing just indicates that in UE5
	// we took a parameter of function by reference (not constant) just to assign its value within that function
	bool DetectLedge(OUT FLedgeDescription& LedgeDescription);
	
private:
	
	TWeakObjectPtr<ACharacter> InCharacter;

		
};
