// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animations/TPSBaseCharacterAnimInstance.h"
#include "FPSCharacterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class TPSSHOOTER_API UFPSCharacterAnimInstance : public UTPSBaseCharacterAnimInstance
{
	GENERATED_BODY()
public:
	virtual void NativeBeginPlay()override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;


protected:
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Character Animation | First person")
	float PlayerCameraPitchAngle = 0.0f;

	TWeakObjectPtr<class AFPPlayerCharacter> InFPPlayerCharacter;
private:
	float CalculateCameraPitchAngle() const;
};
