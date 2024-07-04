// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/Notifies/AnimNotify_EnableRagdoll.h"
#include "BaseData/BaseEnums.h"

void UAnimNotify_EnableRagdoll::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	// Sets new CollisionProfileName and physic simulation
	MeshComp->SetCollisionProfileName(CollisionProfileRagdoll);
	MeshComp->SetSimulatePhysics(true);
}
