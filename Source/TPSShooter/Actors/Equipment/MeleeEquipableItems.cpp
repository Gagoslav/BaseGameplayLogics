// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Equipment/MeleeEquipableItems.h"
#include "BaseData/BaseEnums.h"
#include "Character/TPSBaseCharacter.h"
#include "Actors/Equipment/MeleeEquipableItems.h"
#include "Animation/AnimMontage.h"

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

void AMeleeEquipableItems::OnAttackTimerElapsed()
{
	CurrentAttack = nullptr;
}
