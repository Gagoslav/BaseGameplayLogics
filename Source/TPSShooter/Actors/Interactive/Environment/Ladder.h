// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Interactive/InteractiveActor.h"
#include "Ladder.generated.h"

/**
 *  Ladder class dreived from Interactive actor, blueprintable, here we have all necessary methods and properties 
 *  to construct the ladder itself, interact with character and play character's animations
 */
class UStaticMeshComponent;
class UAnimMontage;
class UBoxComponent;

UCLASS(Blueprintable)
class TPSSHOOTER_API ALadder : public AInteractiveActor
{
	GENERATED_BODY()
public:
	ALadder();


	// This method is like a constructor and is called when an object is constructed
	// But unlike Constructor it is called AFTER the object has been fully constructed and placed in the world and AFTER the object has been serialized
	// and already contains its fields and methods initialized (like construction script in blueprints), so we already can use the values of properties
	// it's called between Constructor and BeginPlay
	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void BeginPlay() override;

	inline float GetLadderHeight() const { return LadderHeight; }

	// Returns if the character is on the top of the object our ladder is leading to
	inline bool GetIsOnTop() const { return bIsOnTop; }

	inline UAnimMontage* GetAttachFromTopAnimMontage() const { return AttachFromTopAnimMontage; }

	// Returns an offset for character from the upper part of ladder when he tries to descend from object's top
	FVector GetAttachFromTopAnimMontageStartingLocation() const;

protected:
	// Basic properties that will make the physical part of ladder
	//================================================================================================================//
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = " Ladder parameters")
	float LadderHeight = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = " Ladder parameters")
	float LadderWidth = 50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = " Ladder parameters")
	float StepsInterwal = 25.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = " Ladder parameters")
	float BottomStepOffset = 25.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* RightRailMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* LeftRailMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UInstancedStaticMeshComponent* StepsInstancedMeshComponent;

	//================================================================================================================//

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	// Interaction volume on the top of object the ladder leads to, is used to understand if the character is on object's ladder is leading to and can descend
	// UFunctions of base interaction volume are bound to this object OnBeginPlay 
	UBoxComponent* TopInteractionVolume;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = " Ladder parameters")
	// We hold the pointer to animation montage in this class as we may have different geometric figures as ladders
	UAnimMontage* AttachFromTopAnimMontage;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = " Ladder parameters")
	// Place from where we start playing AttachFromTop animation montage, also hold this property in Ladder class as we may have different geometric figures as ladders
	FVector AttachFromTopAnimMontageInitialOffset = FVector::ZeroVector;

	// Just caster funtion that convert UPrimitive component of base class into UBoxComponent to initialize ladder's interaction volume OnConstruction
	UBoxComponent* GetLadderInteractionBox() const;

	// Override the base AInteractiveActor's OnInteractionVolumeBeginOverlap the way it can also check if the character is on object's top
	virtual void OnInteractionVolumeBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	// Override the base AInteractiveActor's OnInteractionVolumeEndOverlap the way it can also check if the character is on object's top
	virtual void OnInteractionVolumeEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

private:
	// Field for understanding if character is on the top of an object the ladder is leading to
	bool bIsOnTop = false;
};
