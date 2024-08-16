// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Equipment/MeleeEquipableItems.h"
#include "BaseData/BaseEnums.h"
#include "Character/TPSBaseCharacter.h"
#include "Actors/Equipment/MeleeEquipableItems.h"
#include "Animation/AnimMontage.h"
#include "Components/Weapon/MeleeHitRegistrator.h"
#include "Engine/DamageEvents.h"

AMeleeEquipableItems::AMeleeEquipableItems()
{
	EquippedSocketName = SocketCharacterWeapon;
}

void AMeleeEquipableItems::StartAttack(EMeleeAttackTypes AttackType)
{
	ATPSBaseCharacter* CharacterOwner = GetCharacterOwner();
	if (!IsValid(CharacterOwner))
	{
		return;
	}

	HitActors.Empty();
	CurrentAttack = Attacks.Find(AttackType);

	if (CurrentAttack && IsValid(CurrentAttack->AttackMontage))
	{
		UAnimInstance* CharacterAnimInstance = CharacterOwner->GetMesh()->GetAnimInstance();
		if (IsValid(CharacterAnimInstance))
		{
			float Duration = CharacterAnimInstance->Montage_Play(CurrentAttack->AttackMontage, 1.0f, EMontagePlayReturnType::Duration);
			GetWorld()->GetTimerManager().SetTimer(AttackTimer, this, &AMeleeEquipableItems::OnAttackTimerElapsed, Duration, false);
		}
		else
		{
			OnAttackTimerElapsed();
		}
	}
}

void AMeleeEquipableItems::SetIsHitRegistrationEnabled(bool bIsRegistrationEnabled)
{
	HitActors.Empty();
	for (UMeleeHitRegistrator* HitRegistrator : HitRegistrators)
	{
		HitRegistrator->SetIsHitRegistrationEnabled(bIsRegistrationEnabled);
	}
}

void AMeleeEquipableItems::BeginPlay()
{
	Super::BeginPlay();
	GetComponents<UMeleeHitRegistrator>(HitRegistrators); // Default method of AActor
	// Bind our UFUNCTION (ProcessHit) to all available HitRegistrators
	for (UMeleeHitRegistrator* HitRegistrator : HitRegistrators)
	{
		HitRegistrator->OnMeleeHitRegistered.AddUFunction(this, FName("ProcessHit"));
	}
}

void AMeleeEquipableItems::ProcessHit(const FHitResult& HitResult, const FVector& HitDirection)
{
	if (CurrentAttack==nullptr)
	{
		return;
	}
	AActor* HitActor = HitResult.GetActor();
	if (IsValid(HitActor))
	{
		return;
	}

	if (HitActors.Contains(HitActor))
	{
		return;
	}
	// Special type of UDamageEvent
	FPointDamageEvent DamageEvent;
	DamageEvent.HitInfo = HitResult;
	DamageEvent.ShotDirection = HitDirection;
	DamageEvent.DamageTypeClass = CurrentAttack->DamageTypeClass;

	ATPSBaseCharacter* CharacterOwner = GetCharacterOwner();
	AController* Controller = IsValid(CharacterOwner) ? CharacterOwner->GetController<AController>() : nullptr;
	// Apply damage (from FHitResult)
	HitActor->TakeDamage(CurrentAttack->DamageAmount, DamageEvent,Controller, GetOwner());
	HitActors.Add(HitActor);
}

void AMeleeEquipableItems::OnAttackTimerElapsed()
{
	CurrentAttack = nullptr;
	SetIsHitRegistrationEnabled(false);
}
