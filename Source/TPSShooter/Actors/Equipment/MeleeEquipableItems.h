// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Equipment/EquipableItems.h"
#include "MeleeEquipableItems.generated.h"

class UAnimMontage;
enum class EMeleeAttackTypes :uint8;

USTRUCT(BlueprintType)
struct FMeleeAttackDescription
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Melee Attack")
	TSubclassOf<class UDamageType> DamageTypeClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Melee Attack", meta =(ClampMin = 0.0f, UIMin = 0.0f))
	float DamageAmount = 50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Melee Attack")
	UAnimMontage* AttackMontage;


};


UCLASS(Blueprintable)
class TPSSHOOTER_API AMeleeEquipableItems : public AEquipableItems
{
	GENERATED_BODY()
public:
	AMeleeEquipableItems();

	void StartAttack(EMeleeAttackTypes AttackType);
	
protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Melee Attack")
	TMap<EMeleeAttackTypes, FMeleeAttackDescription> Attacks;

private:
	void OnAttackTimerElapsed();
	FTimerHandle AttackTimer;
	FMeleeAttackDescription* CurrentAttack;
};
