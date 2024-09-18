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

	WeaponMuzzle = CreateDefaultSubobject<UWeaponFusilComponent>(TEXT("WeaponMuzzle"));
	WeaponMuzzle->SetupAttachment(WeaponMesh, SocketWeaponMuzzle);

	EquippedSocketName = SocketCharacterWeapon;

	ReticleType = EReticleType::Default;

}

void ARangeWeapon::StartFire()
{
	// If our timer manager for shooting is active then make early return
	if (GetWorld()->GetTimerManager().IsTimerActive(ShotTimer))
	{
		return;
	}

	bIsFiring = true;
	MakeShot();


}

void ARangeWeapon::StopFire()
{
	bIsFiring = false;
	//GetWorld()->GetTimerManager().ClearTimer(ShotTimer);
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

	ATPSBaseCharacter* CharacterOwner = GetCharacterOwner();
	if (!IsValid(CharacterOwner))
	{
		return;
	}
	bIsReloading = true;

	// If the corresponding montages are set then play them and after they are complete callback EndReload(true)
	if (IsValid(CharacterReloadMontage))
	{
		float MontageDuration = CharacterOwner->PlayAnimMontage(CharacterReloadMontage);
		PlayAnimMontage(WeaponReloadMontage);
		if (ReloadType == EReloadType::FullClip)
		{
			GetWorld()->GetTimerManager().SetTimer(ReloadTimer, [this]() {EndReload(true); }, MontageDuration, false);
		}
		// In the case of ByBullte reload we will use AnimMontages devided by sections and use AnimNotifies to implement reload action

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
	ATPSBaseCharacter* CharacterOwner = GetCharacterOwner();

	if (!bIsSuccess)
	{
		if (!IsValid(CharacterOwner))
		{
			CharacterOwner->StopAnimMontage(CharacterReloadMontage);
		}

		StopAnimMontage(WeaponReloadMontage);

	}

	// Case for a shotgun (reload bullets one by one)
	if (ReloadType == EReloadType::ByBullet)
	{

		UAnimInstance* CharacterAnimInstance = IsValid(CharacterOwner) ? CharacterOwner->GetMesh()->GetAnimInstance() : nullptr;
		if (IsValid(CharacterAnimInstance))
		{
			CharacterAnimInstance->Montage_JumpToSection(SectionMontageReloadEnd, CharacterReloadMontage);
		}
		UAnimInstance* WeaponAnimInstance = WeaponMesh->GetAnimInstance();
		if (IsValid(WeaponAnimInstance))
		{
			WeaponAnimInstance->Montage_JumpToSection(SectionMontageReloadEnd, WeaponReloadMontage);
		}
	}

	GetWorld()->GetTimerManager().ClearTimer(ReloadTimer);

	bIsReloading = false;
	if (OnReloadCompleteEvent.IsBound() && bIsSuccess)
	{
		// Delegate function is added in UCharacterEquipmentComponent::EquipItemInSlot(EEquipmentSlots Slot)
		OnReloadCompleteEvent.Broadcast();
	}
}

void ARangeWeapon::MakeShot()
{

	// We have already initialize this actor's owner in UCharacterEquipmentComponent
	ATPSBaseCharacter* CharacterOwner = GetCharacterOwner();
	if (!IsValid(CharacterOwner))
	{
		return;
	}
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

	FVector ShotLocation;
	FRotator ShotRotation;

	if (CharacterOwner->IsPlayerControlled()) // checks if the pawn is player or AICharacter
	{
		// We need to get our player controller to be able to find PlayerViewPoint (Centre of screen)
		APlayerController* Controller = CharacterOwner->GetController<ATPSPlayerController>();
		Controller->GetPlayerViewPoint(ShotLocation, ShotRotation); // GetPlayerViewPoint takes OUT parameters, make
	}
	else
	{
		// if our character is controlled by AI controller and is a bot
		ShotLocation = WeaponMuzzle->GetComponentLocation();
		ShotRotation = CharacterOwner->GetBaseAimRotation(); // Default method of APawn class
	}


	FVector ShotDirection = ShotRotation.RotateVector(FVector::ForwardVector);

	// Decrease bullets number
	SetAmmo(CurrentAmmo - 1);
	WeaponMuzzle->Shot(ShotLocation, ShotDirection, GetCurrentBulletSpreadAngle()); // make a shot

	// If the fire mode of our current weapon is full automate we will fire automatic 

	GetWorld()->GetTimerManager().SetTimer(ShotTimer, this, &ARangeWeapon::OnShotTimerElapsed, GetShotTimerInterval(), false);

}

void ARangeWeapon::OnShotTimerElapsed()
{
	if (!bIsFiring)
	{
		return;
	}

	switch (WeaponFireMode)
	{
	case EWeaponFireMode::Single:
	{
		StopFire();
		break;
	}
	case EWeaponFireMode::FullAutomate:
	{
		MakeShot();
		break;
	}

	}

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
	return CurrentAmmo > 0;
}

EAmunitionType ARangeWeapon::GetAmmoType() const
{
	return AmmoType;
}

EReticleType ARangeWeapon::GetReticleType() const
{
	return bIsAiming ? AimReticleType : Super::GetReticleType();
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

