// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Interactive/InteractiveActor.h"
#include "Character/TPSBaseCharacter.h"



// Called when the game starts or when spawned
void AInteractiveActor::BeginPlay()
{
	Super::BeginPlay();
	if (IsValid(InteractionVolume))
	{
		// Bind our UFUNCTIONS to delegates
		InteractionVolume->OnComponentBeginOverlap.AddDynamic(this, &AInteractiveActor::OnInteractionVolumeBeginOverlap);
		InteractionVolume->OnComponentEndOverlap.AddDynamic(this, &AInteractiveActor::OnInteractionVolumeEndOverlap);
	}
}

void AInteractiveActor::OnInteractionVolumeBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	if (IsOverlappingCharacterCapsule(OtherActor, OtherComp))
	{
		ATPSBaseCharacter* InBaseCharacter = StaticCast<ATPSBaseCharacter*>(OtherActor);
		// Add (register) this object into BaseCharacter's AvailableInteractiveActors array
		InBaseCharacter->RegisterInteractiveActor(this);
	}


}



void AInteractiveActor::OnInteractionVolumeEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	
	if (IsOverlappingCharacterCapsule(OtherActor, OtherComp))
	{
		ATPSBaseCharacter* InBaseCharacter = StaticCast<ATPSBaseCharacter*>(OtherActor);
		// Remove (Unregister) this object from BaseCharacter's AvailableInteractiveActors array
		InBaseCharacter->UnRegisterInteractiveActor(this);
	}
}



bool AInteractiveActor::IsOverlappingCharacterCapsule(AActor * OtherActor, UPrimitiveComponent* OtherComp)
{

	ATPSBaseCharacter* InBaseCharacter = Cast<ATPSBaseCharacter>(OtherActor);
	// Make sure the overlapped actor is our ATPSBaseCharacter othervise skip
	if (!IsValid(InBaseCharacter))
	{
		return false;
	}

	// To prevent system from firing additional overlap events, such as overlap with character's mesh and other components
	// we check our current overlap to be only with character's collision component
	if (Cast<UCapsuleComponent>(OtherComp) != InBaseCharacter->GetCapsuleComponent())
	{
		return false;
	}

	return true;
}