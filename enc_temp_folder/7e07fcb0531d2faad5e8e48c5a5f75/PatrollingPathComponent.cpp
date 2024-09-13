// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/PatrollingPathComponent.h"

// Sets default values for this component's properties
UPatrollingPathComponent::UPatrollingPathComponent()
{
	
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPatrollingPathComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UPatrollingPathComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

