// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Projectiles/TPSProjectile.h"
#include "TPSProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
ATPSProjectile::ATPSProjectile()
{
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	CollisionComponent->InitSphereRadius(5.0f);
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block); // I should make my own collision preset for projectile
	SetRootComponent(CollisionComponent);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComponent"));
	ProjectileMovementComponent->InitialSpeed = 2000.0f;
}

void ATPSProjectile::LaunchProjectile(FVector Direction)
{
	// Set Direction and Velocity
	ProjectileMovementComponent->Velocity = Direction * ProjectileMovementComponent->InitialSpeed;
	CollisionComponent->IgnoreActorWhenMoving(GetOwner(), true); // To ignore a character who will throw a projectile
	OnProjectileLaunched(); // Callback function
}

void ATPSProjectile::OnProjectileLaunched()
{
	// Still pure virtual function that will be overriden
}





