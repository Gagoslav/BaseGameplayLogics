// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Interactive/Environment/Ladder.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "BaseData/BaseEnums.h"

ALadder::ALadder()
{
	// It's obligatory to initialize RootComponent in actor derived classes
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("LadderRoot"));
	
	LeftRailMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftRail"));
	LeftRailMeshComponent->SetupAttachment(RootComponent);

	RightRailMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightRail"));
	RightRailMeshComponent->SetupAttachment(RootComponent);

	StepsInstancedMeshComponent = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Steps"));
	StepsInstancedMeshComponent->SetupAttachment(RootComponent);

	// Initialize (override) Interaction volume just declared in base class (UPrimitiveComponent)
	InteractionVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractionVolume"));
	InteractionVolume->SetupAttachment(RootComponent);
	InteractionVolume->SetCollisionProfileName(CollisionProfilePawnInteractionVolume); // get profile with corresponding FName from project settings | collision presets
	InteractionVolume->SetGenerateOverlapEvents(true);

	// Initialize (create) TopInteracionVolume that is responsible for character's interaction when he is on the top of object where the Ladder leads
	TopInteractionVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("TopInteractionVolume"));
	TopInteractionVolume->SetupAttachment(RootComponent);
	TopInteractionVolume->SetCollisionProfileName(CollisionProfilePawnInteractionVolume); // get profile with corresponding FName from project settings | collision presets
	TopInteractionVolume->SetGenerateOverlapEvents(true);

}

void ALadder::OnConstruction(const FTransform& Transform)
{
	// Locate Rails respectively on right and left side from steps and locate their bottom ends on O relative height
	LeftRailMeshComponent->SetRelativeLocation(FVector(0.0f, -LadderWidth * 0.5f, LadderHeight * 0.5f));
	RightRailMeshComponent->SetRelativeLocation(FVector(0.0f, LadderWidth * 0.5f, LadderHeight * 0.5f));

	// Change the Z value of Left rail mesh component
	UStaticMesh* LeftRailMesh = LeftRailMeshComponent->GetStaticMesh();
	if (IsValid(LeftRailMesh))
	{
		// Get Z extention of Rail mesh (height)
		float MeshHeight = LeftRailMesh->GetBoundingBox().GetSize().Z;
		if (!FMath::IsNearlyZero(MeshHeight))
		{
			LeftRailMeshComponent->SetRelativeScale3D(FVector(1.0f, 1.0f, LadderHeight / MeshHeight)); // Scale rail height on the proportion of LadderHeight and MeshHeight
		}
	}

	// Change the Z value of Right rail mesh component
	UStaticMesh* RightRailMesh = RightRailMeshComponent->GetStaticMesh();
	if (IsValid(RightRailMesh))
	{
		// Get Z extention of Rail mesh (height)
		float MeshHeight = RightRailMesh->GetBoundingBox().GetSize().Z;
		if (!FMath::IsNearlyZero(MeshHeight))
		{
			RightRailMeshComponent->SetRelativeScale3D(FVector(1.0f, 1.0f, LadderHeight / MeshHeight)); // Scale rail height on the proportion of LadderHeight and MeshHeight
		}
	}

	// Change Y value of step mesh
	UStaticMesh* StepsMesh = StepsInstancedMeshComponent->GetStaticMesh();
	if (IsValid(StepsMesh))
	{
		float StepsWidth = StepsMesh->GetBoundingBox().GetSize().Y;
		if (!FMath::IsNearlyZero(StepsWidth))
		{
			StepsInstancedMeshComponent->SetRelativeScale3D(FVector(1.0f, LadderWidth / StepsWidth, 1.0f));
		}
	}

	// Set necessary step number
	StepsInstancedMeshComponent->ClearInstances();
	uint32 StepsCount = FMath::FloorToInt(LadderHeight - BottomStepOffset) / StepsInterwal;
	for (uint32 i = 0; i < StepsCount; ++i)
	{
		FTransform InstanceTransform(FVector(0.0f, 0.0f, BottomStepOffset + i * StepsInterwal));
		StepsInstancedMeshComponent->AddInstance(InstanceTransform);
	}

	float BoxDepthExtent = GetLadderInteractionBox()->GetUnscaledBoxExtent().X;
	// Box extention is set as radius (half value) not diameter
	GetLadderInteractionBox()->SetBoxExtent(FVector(BoxDepthExtent, LadderWidth*0.5f, LadderHeight*0.5f));
	GetLadderInteractionBox()->SetRelativeLocation(FVector(BoxDepthExtent, 0.0f, LadderHeight * 0.5));

	// Set the extent for TopInteractionVolume as Y same as LadderInteractionVolume
	FVector TopBoxExtent = TopInteractionVolume->GetUnscaledBoxExtent();
	TopInteractionVolume->SetBoxExtent(FVector(TopBoxExtent.X, LadderWidth * 0.5f, TopBoxExtent.Z));
	// Set relative location for TopInteractionVolume as if it locates on the top of object the ladder is leading to
	TopInteractionVolume->SetRelativeLocation(FVector(-TopBoxExtent.X, 0.0f, LadderHeight + TopBoxExtent.Z));
}

void ALadder::BeginPlay()
{
	Super::BeginPlay();
	// Bind base classe's UFunctions (registrating and unregistrating this ladder into character) 
	// also override them to check if character is on object's top or not
	TopInteractionVolume->OnComponentBeginOverlap.AddDynamic(this, &ALadder::OnInteractionVolumeBeginOverlap);
	TopInteractionVolume->OnComponentEndOverlap.AddDynamic(this, &ALadder::OnInteractionVolumeEndOverlap);
}

FVector ALadder::GetAttachFromTopAnimMontageStartingLocation() const
{
	// Get offset's direction from ladder 
	FRotator OrientationRotation = GetActorForwardVector().ToOrientationRotator();
	FVector Offset = OrientationRotation.RotateVector(AttachFromTopAnimMontageInitialOffset); // AttachFromTopAnimMontageInitialOffset is UPROPERTY and is set from editor
	
	// Get offset's height (upper part of ladder) and add direction
	FVector LadderTop = GetActorLocation() + GetActorUpVector() * LadderHeight; 

	// Returns the world location of offset
	return LadderTop + Offset;
}

UBoxComponent* ALadder::GetLadderInteractionBox() const
{
	// Before Ladder object is created InteractionVolume is just UPrimitiveComponent declared in base (AInteractiveActor) class
	return StaticCast<UBoxComponent*>(InteractionVolume);
}

void ALadder::OnInteractionVolumeBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Call base implementation declared in AInteractiveActor class (registers this ladder into character's interactive objects array)
	Super::OnInteractionVolumeBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	// Check that Overlapping actor is character
	if (!IsOverlappingCharacterCapsule(OtherActor, OtherComp))
	{
		return;
	}

	// Check that if ladder's interactive volume is the TopInteractionVolume, then suppose character is on objet's top
	if (OverlappedComponent == TopInteractionVolume)
	{
		bIsOnTop = true;
	}
	
}

void ALadder::OnInteractionVolumeEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// Call base implementation declared in AInteractiveActor class (unregisters this ladder from character's interactive objects array)
	Super::OnInteractionVolumeEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);

	// Check that Overlapping actor is character
	if (!IsOverlappingCharacterCapsule(OtherActor, OtherComp))
	{
		return;
	}

	// Check that if ladder's interactive volume is the TopInteractionVolume, then suppose character has moved from object's top
	if (OverlappedComponent == TopInteractionVolume)
	{
		bIsOnTop = false;
	}
	
}
