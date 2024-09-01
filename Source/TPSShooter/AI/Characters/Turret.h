// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BaseData/BaseEnums.h"
#include "Turret.generated.h"


class UWeaponFusilComponent;

UENUM(BlueprintType)
enum class ETurretState : uint8
{
	Searching,
	Firing
};


UCLASS()
class TPSSHOOTER_API ATurret : public APawn
{
	GENERATED_BODY()

public:
	
	ATurret();
	
	virtual void Tick(float DeltaTime) override;

	void SetCurrentTarget(AActor* NewTarget);

	virtual void PossessedBy(AController* NewController) override;

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* TurretBaseComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* TurretBarellComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UWeaponFusilComponent* WeaponBarell;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Turret parametrs", meta = (ClampMin = 0.0f, UIMin =0.0f))
	float BaseSearchingRotationRate = 60.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Turret parametrs", meta = (ClampMin = 0.0f, UIMin = 0.0f))
	float BaseFiringInterpSpeed = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Turret parametrs", meta = (ClampMin = 0.0f, UIMin = 0.0f))
	float BarellPitchRotationRate = 60.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Turret parametrs", meta = (ClampMin = 0.0f, UIMin = 0.0f))
	float MaxBarellPitchAngle = 60.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Turret parametrs", meta = (ClampMin = -30.0f, UIMin = 30.0f))
	float MinBarellPitchAngle = -30.0f;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Turret parametrs | Fire", meta = (ClampMin = 30.0f, UIMin = 30.0f))
	float RateOfFire = 300.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Turret parametrs | Fire", meta = (ClampMin = 0.0f, UIMin = 0.0f))
	float BulletSpreadAngle = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Turret parametrs | Fire", meta = (ClampMin = 0.0f, UIMin = 0.0f))
	float FireDelayTime = 3.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Turret parametrs | Team")
	ETeams Team = ETeams::Enemy;

private:

	void SearchingMovement(float DeltaTime);
	void FiringMovement(float DeltaTime);

	void SetCurrentTurretState(ETurretState NewState);
	void MakeShot();

	// virtual methods of APawn class 
	// Set View location and rotation of turret at its barell
	virtual FVector GetPawnViewLocation() const override;
	virtual FRotator GetViewRotation()const override;

	ETurretState CurrentTurretState = ETurretState::Searching;
	AActor* CurrentTarget = nullptr;

	float GetFireInterval() const;
	FTimerHandle FiringTimer;

};
