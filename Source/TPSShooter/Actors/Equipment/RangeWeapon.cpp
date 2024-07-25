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

	EquippedSocketName = SocketCharacterWeapon;

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

void ARangeWeapon::StartAim()
{
	bIsAiming = true;
}

void ARangeWeapon::StopAim()
{
	bIsAiming = false;
}

void ARangeWeapon::StartReload()
{
	// Get an owner character
	checkf(GetOwner()->IsA<ATPSBaseCharacter>(), TEXT("ARangeWeapon::StartReload() Only character can use the weapon and reload"));
	ATPSBaseCharacter* CharacterOwner = StaticCast<ATPSBaseCharacter*>(GetOwner());
	bIsReloading = true;

	// If the corresponding montages are set then play them and after they are complete callback EndReload(true)
	if (IsValid(CharacterReloadMontage))
	{
		float MontageDuration = CharacterOwner->PlayAnimMontage(CharacterReloadMontage);
		PlayAnimMontage(WeaponReloadMontage);
		GetWorld()->GetTimerManager().SetTimer(ReloadTimer,  [this](){EndReload(true); }, MontageDuration, false);

	}
	else
	{
		EndReload(true);
	}
}

void ARangeWeapon::EndReload(bool bIsSuccess)
{
	// Make sure we were reloading
	if (!bIsReloading)
	{
		return;
	}
	// If reload was unsuccessfull then stop playing montage
	if (!bIsSuccess)
	{
		checkf(GetOwner()->IsA<ATPSBaseCharacter>(), TEXT("ARangeWeapon::StartReload() Only character can use the weapon and reload"));
		ATPSBaseCharacter* CharacterOwner = StaticCast<ATPSBaseCharacter*>(GetOwner());
		CharacterOwner->StopAnimMontage(CharacterReloadMontage);
		StopAnimMontage(WeaponReloadMontage);

	}

	GetWorld()->GetTimerManager().ClearTimer(ReloadTimer);

	bIsReloading = false;
	if (OnReloadCompleteEvent.IsBound() && bIsSuccess)
	{
		OnReloadCompleteEvent.Broadcast();
	}
}

void ARangeWeapon::MakeShot()
{
	// Assert that only character can fire, otherwise crash an editor
	// We have already initialize this actor's owner in UCharacterEquipmentComponent
	checkf(GetOwner()->IsA<ATPSBaseCharacter>(), TEXT("ARangeWeapon::Fire() Only character can use the weapon and fire"));
	ATPSBaseCharacter* CharacterOwner = StaticCast<ATPSBaseCharacter*>(GetOwner());

	if (!CanShoot())
	{
		if (CurrentAmmo == 0 && bAutoReload)
		{
			CharacterOwner->Reload();
		}
		StopFire();
		return;
	}
	EndReload(false);

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
	ViewDirection += GetBulletSpreadOffset(FMath::RandRange(0.0f, GetCurrentBulletSpreadAngle()), PlayerViewRotation);

	// Decrease bullets number
	SetAmmo(CurrentAmmo-1);
	WeaponMuzle->Shot(PlayerViewPoint, ViewDirection, Controller); // make a shot
}

FVector ARangeWeapon::GetBulletSpreadOffset(float Angle, FRotator ShotRotation) const
{
	float SpreadSize = FMath::Tan(Angle); // Get normalized length of Spread vector (counter located cathetus || tangent)
	float RotationAngle = FMath::RandRange(0.0f, 2 * PI); // random number from 0 to 360, like pick random point on circumference

	float SpreadY = FMath::Cos(RotationAngle);
	float SpreadZ = FMath::Sin(RotationAngle);
	FVector Result = (ShotRotation.RotateVector(FVector::RightVector) * SpreadY + ShotRotation.RotateVector(FVector::UpVector) * SpreadZ) *SpreadSize; // Find offset Direction and time it by ratio 
	//UE_LOG(LogTemp, Warning, TEXT("%f %f %f"),Result.X, Result.Y, Result.Z );

	return Result;

}

float ARangeWeapon::GetCurrentBulletSpreadAngle() const
{
	// As we have different spreads of firing depending if the character is aiming or just shooting
	float AngleInDegrees = bIsAiming ? AimSpreadAngle : SpreadAngle;
	return FMath::DegreesToRadians(AngleInDegrees);
}

FTransform ARangeWeapon::GetForegripTransform() const
{
	return WeaponMesh->GetSocketTransform(SocketForegrip);
}

void ARangeWeapon::SetAmmo(int32 NewAmmo)
{
	CurrentAmmo = NewAmmo;
	// Execute OnAmmoChangedEvent delegate that calls back UCharacterEquipmentComponent::OnCurrentWeaponUpdatedAmmo(int32 Ammo)
	if (OnAmmoChangedEvent.IsBound())
	{
		OnAmmoChangedEvent.Broadcast(CurrentAmmo); // execute bound functions
	}
}

bool ARangeWeapon::CanShoot() const
{
	return CurrentAmmo>0;
}

EAmunitionType ARangeWeapon::GetAmmoType() const
{
	return AmmoType;
}

void ARangeWeapon::BeginPlay()
{
	Super::BeginPlay();
	SetAmmo(MaxAmmo);
}



float ARangeWeapon::PlayAnimMontage(UAnimMontage* AnimMontage)
{
	// Wrapper function designed to run the Montage_Play method of anim instance if we have one

	UAnimInstance* WeaponAnimInstance = WeaponMesh->GetAnimInstance();
	float Result = 0.0f;
	if (IsValid(WeaponAnimInstance))
	{
		return WeaponAnimInstance->Montage_Play(AnimMontage);
	}
	return Result;
}

void ARangeWeapon::StopAnimMontage(UAnimMontage* AnimMontage, float BlendOutTime)
{
	// Wrapper function designed to run the Montage_Stop method of anim instance if we have one
	UAnimInstance* WeaponAnimInstance = WeaponMesh->GetAnimInstance();
	if (IsValid(WeaponAnimInstance))
	{
		WeaponAnimInstance->Montage_Stop(BlendOutTime, AnimMontage);
	}
}

float ARangeWeapon::GetShotTimerInterval()
{
	return 60.0f / RateOfFire;
}

