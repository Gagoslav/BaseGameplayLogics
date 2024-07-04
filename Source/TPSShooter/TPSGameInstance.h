// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "TPSGameInstance.generated.h"

/**
 * In this game instance we place some basic debug methods, that we can call from engine console
 */
UCLASS()
class TPSSHOOTER_API UTPSGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	// Virtual Method to enable the call of console functions (UFUNCTION(Exec))
	// from this classe's subsystems
	virtual bool ProcessConsoleExec(const TCHAR* Cmd, FOutputDevice& Ar, UObject* Executor) override;

};
