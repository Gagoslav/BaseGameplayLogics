// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Equipment/RangeWeapon.h"
#include "Components/Weapon/WeaponFusilComponent.h"
#include "Character/TPSBaseCharacter.h"
#include "Controllers/TPSPlayerController.h"
#include "BaseData/BaseEnums.h"

ARangeWeapon::ARangeWeapon()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("WeaponRoot"));
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(RootComponent);

	WeaponMuzle = CreateDefaultSubobject<UWeaponFusilComponent>(TEXT("WeaponMuzzle"));
	WeaponMuzle->SetupAttachment(WeaponMesh, SocketWeaponMuzzle);

}

void ARangeWeapon::StartFire()
{
	MakeShot();
	// If the fire mode of our current weapon is full automate we will fire automatic 
	if (WeaponFireMode == EWeaponFireMode::FullAutomate)
	{
		GetWorld()->GetTimerManager().ClearTimer(ShotTimer);
		GetWorld()->GetTimerManager().SetTimer(ShotTimer, this, &ARangeWeapon::MakeShot, GetShotTimerInterval(), true);
	}
	
}

void ARangeWeapon::StopFire()
{
	GetWorld()->GetTimerManager().ClearTimer(ShotTimer);
}

void ARangeWeapon::MakeShot()
{
	// Assert that only character can fire, otherwise crash an editor
	// We have already initialize this actor's owner in UCharacterEquipmentComponent
	checkf(GetOwner()->IsA<ATPSBaseCharacter>(), TEXT("ARangeWeapon::Fire() Only character can use the weapon and fire"));
	ATPSBaseCharacter* CharacterOwner = StaticCast<ATPSBaseCharacter*>(GetOwner());

	// Pass Anim montage for character that is located in weapon class to be played by character
	CharacterOwner->PlayAnimMontage(CharacterFireMontage);
	PlayAnimMontage(WeaponFireMontage);

	// We need to get our player controller to be able to find PlayerViewPoint (Centre of screen)
	APlayerController* Controller = CharacterOwner->GetController<ATPSPlayerController>();
	if (!IsValid(Controller))
	{
		return;
	}

	FVector PlayerViewPoint;
	FRotator PlayerViewRotation;
	// For this we got APlayerController
	Controller->GetPlayerViewPoint(PlayerViewPoint, PlayerViewRotation); // GetPlayerViewPoint takes OUT parameters, makes them the centre of screen

	FVector ViewDirection = PlayerViewRotation.RotateVector(FVector::ForwardVector);
	// Add spread offset from screen's sentral point
	ViewDirection += GetBulletSpreadOffset(FMath::RandRange(0.0f, FMath::DegreesToRadians(SpreadAngle)), PlayerViewRotation);

	WeaponMuzle->Shot(PlayerViewPoint, ViewDirection, Controller); // make a shot
}

FVector ARangeWeapon::GetBulletSpreadOffset(float Angle, FRotator ShotRotation)
{
	float SpreadSize = FMath::Tan(Angle); // Get normalized length of Spread vector (counter located cathetus || tangent)
	float RotationAngle = FMath::RandRange(0.0f, 2 * PI);

	float SpreadY = FMath::Cos(RotationAngle);
	float SpreadZ = FMath::Sin(RotationAngle);
	FVector Result = (ShotRotation.RotateVector(FVector::UpVector) * SpreadZ
		+ ShotRotation.RotateVector(FVector::RightVector) * SpreadY) * SpreadSize;

	return Result;

}

FTransform ARangeWeapon::GetForegripTransform() const
{
	return WeaponMesh->GetSocketTransform(SocketForegrip);
}

float ARangeWeapon::PlayAnimMontage(UAnimMontage* AnimMontage)
{
	UAnimInstance* WeaponAnimInstance = WeaponMesh->GetAnimInstance();
	float Result = 0.0f;
	if (IsValid(WeaponAnimInstance))
	{
		return WeaponAnimInstance->Montage_Play(AnimMontage);
	}
	return Result;
}

float ARangeWeapon::GetShotTimerInterval()
{
	return 60.0f / RateOfFire;
}
