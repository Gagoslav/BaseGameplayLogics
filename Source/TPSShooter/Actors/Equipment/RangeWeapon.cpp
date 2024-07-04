// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Equipment/RangeWeapon.h"
#include "Components/Weapon/WeaponFusilComponent.h"
#include "Character/TPSBaseCharacter.h"
#include "Controllers/TPSPlayerController.h"

ARangeWeapon::ARangeWeapon()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("WeaponRoot"));
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(RootComponent);

	WeaponMuzle = CreateDefaultSubobject<UWeaponFusilComponent>(TEXT("WeaponFusil"));
	WeaponMuzle->SetupAttachment(WeaponMesh, FName("SocketMuzzle"));

}

void ARangeWeapon::Fire()
{
	checkf(GetOwner()->IsA<ATPSBaseCharacter>(), TEXT("Only character can use the weapon and fire"));
	ATPSBaseCharacter* CharacterOwner = StaticCast<ATPSBaseCharacter*>(GetOwner());
	APlayerController* Controller = CharacterOwner->GetController<ATPSPlayerController>();
	if (!IsValid(Controller))
	{
		return;
	}

	FVector PlayerViewPoint;
	FRotator PlayerViewRotation;
	Controller->GetPlayerViewPoint(PlayerViewPoint, PlayerViewRotation);

	FVector ViewDirection = PlayerViewRotation.RotateVector(FVector::ForwardVector);
	WeaponMuzle->Shot(PlayerViewPoint, ViewDirection, Controller);
}
