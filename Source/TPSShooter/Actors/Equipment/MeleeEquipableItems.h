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

	void SetIsHitRegistrationEnabled(bool bIsRegistrationEnabled);
	
protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Melee Attack")
	TMap<EMeleeAttackTypes, FMeleeAttackDescription> Attacks;

	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void ProcessHit(const FHitResult& HitResult, const FVector& HitDirection);

	TArray<class UMeleeHitRegistrator*> HitRegistrators;
	// Set of already hit actors as they may be plural and are unique
	TSet<AActor*>HitActors;

	void OnAttackTimerElapsed();
	FTimerHandle AttackTimer;
	FMeleeAttackDescription* CurrentAttack;
};
