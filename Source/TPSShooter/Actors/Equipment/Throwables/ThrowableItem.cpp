// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Equipment/Throwables/ThrowableItem.h"
#include "Character/TPSBaseCharacter.h"
#include "Controllers/TPSPlayerController.h"
#include "Actors/Projectiles/TPSProjectile.h"
#include "BaseData/BaseEnums.h"

void AThrowableItem::Throw()
{
	// First step ensure that the owner of this actor is set to be our ATPSBaseCharacter on it's spawn
	checkf(GetOwner()->IsA<ATPSBaseCharacter>(), TEXT("AThrowableItem::Throw() Only character can use the throwable Item"));
	ATPSBaseCharacter* CharacterOwner = StaticCast<ATPSBaseCharacter*>(GetOwner());

	// Here we get our ATPSPlayerController to get PlayerViewPoint later
	APlayerController* Controller = CharacterOwner->GetController<ATPSPlayerController>();
	if (!IsValid(Controller))
	{
		return;
	}

	// Variables that need to be initialized in GetPlayerViewPoint
	FVector PlayerViewLocation;
	FRotator PlayerViewRotation;
	

	Controller->GetPlayerViewPoint(PlayerViewLocation, PlayerViewRotation);
	FTransform PlayerViewTransform(PlayerViewRotation, PlayerViewLocation); // To convert Socket location from World coordinates to local coordinates
	FVector PlayerViewDirection = PlayerViewRotation.RotateVector(FVector::ForwardVector); // returns normalized vector
	FVector PlayerViewUpVector = PlayerViewRotation.RotateVector(FVector::UpVector); // returns normalized vector
	

	FVector LaunchDirection = PlayerViewDirection + FMath::Tan(FMath::RadiansToDegrees(ThrowAngle)) * PlayerViewUpVector;

	FVector ThrowableSocketLocation = CharacterOwner->GetMesh()->GetSocketLocation(SocketThrowableItem);
	FVector SocketInViewSpace = PlayerViewTransform.InverseTransformPosition(ThrowableSocketLocation);

	// Set spawn location for ATPSProjectile for that we have TSubclassOf
	FVector SpawnLocation = PlayerViewLocation + PlayerViewDirection * SocketInViewSpace.X;

	ATPSProjectile* Projectile = GetWorld()->SpawnActor<ATPSProjectile>(ProjectileClass, SpawnLocation, FRotator::ZeroRotator);
	if (IsValid(Projectile))
	{
		// Obligatory set owner of projectile as ATPSBaseCharacter as that will be checked by assert 
		Projectile->SetOwner(GetOwner());
		Projectile->LaunchProjectile(LaunchDirection.GetSafeNormal());
	}
	
}
