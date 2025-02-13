// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TPSPlayerController.generated.h"

/**
 * 
 */
class UPlayerHUDWidget;

UCLASS()
class TPSSHOOTER_API ATPSPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	//Override base method of AController to set it responsible for the new pawn (InBaseCharacter)
	virtual void SetPawn(APawn* InPawn) override;
	inline bool GetIgnoreCameraPitch() const { return bIgnoreCameraPitch; }
	void SetIgnoreCameraPitch(bool NewIgnore) { bIgnoreCameraPitch = NewIgnore; }

protected:

	//Implement player input
	virtual void SetupInputComponent() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widgets")
	TSubclassOf<UPlayerHUDWidget> PlayerHUDWidgetClass;

private:
	//Declaring our InBaseCharacter as Soft object pointer as we may not want to load it in memory immidiately
	TSoftObjectPtr <class ATPSBaseCharacter> InBaseCharacter;

	//Declare input methods that will invoke the appropriate methods in our base character class
	void MoveForward(float Value);
	void MoveRight(float Value);
	void Turn(float Value);
	void LookUp(float Value);
	void Mantle();
	void Jump();
	void ChangeCrouchState();
	void StartSprint();
	void EndSprint();

	void SwimForward(float Value);
	void SwimRight(float Value);
	void SwimUp(float Value);

	void ClimbLadderUp(float Value);
	void InteractWithLadder();

	void PlayerStartFire();
	void PlayerStopFire();

	void StartAiming();
	void StopAiming();

	void Reload();
	void NextItem();
	void PreviousItem();
	void EquipPrimaryItem();

	void PrimaryMeleeAttack();
	void SecondaryMeleeAttack();
private:
	void CreateAndInitializeWidgets();
	UPlayerHUDWidget* PlayerHUDWidget = nullptr;
	bool bIgnoreCameraPitch = false;
};
