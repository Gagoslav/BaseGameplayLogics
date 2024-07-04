// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractiveActor.generated.h"

// Abstaract class for all types of intaractive actors that will be derived from this class
// contains only base property of all interactive objects: Overlapping volume, ufunctions that will
// fire as soon as characters overlap it (on begin and end) and getter defining
// if the overlapping actor is a character and has appropriate collision primitive (capsule component)
UCLASS(Abstract, NotBlueprintable)
class TPSSHOOTER_API AInteractiveActor : public AActor
{
	GENERATED_BODY()
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction")
	UPrimitiveComponent* InteractionVolume;
	
	UFUNCTION()
	virtual void OnInteractionVolumeBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	
	UFUNCTION()
	virtual void OnInteractionVolumeEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


	bool IsOverlappingCharacterCapsule(AActor * OtherActor, UPrimitiveComponent* OtherComp);

};
