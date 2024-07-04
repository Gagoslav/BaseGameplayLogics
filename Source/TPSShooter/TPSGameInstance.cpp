// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSGameInstance.h"

bool UTPSGameInstance::ProcessConsoleExec(const TCHAR* Cmd, FOutputDevice& Ar, UObject* Executor)
{
	bool bResult = Super::ProcessConsoleExec(Cmd, Ar, Executor);

	// Get all subsystems of game instance class
	TArray<UGameInstanceSubsystem*>Subsystems = GetSubsystemArray<UGameInstanceSubsystem>();
	for (auto Subsystem : Subsystems)
	{
		// Here the purpose is call the virtual method ProcessConsoleExec(Cmd, Ar, Executor)
		// for all Subsystems of game instance, to avoid its manually call from each separate GameInstanceSubsystem
		bResult |= Subsystem->ProcessConsoleExec(Cmd, Ar, Executor);
	}
	return bResult;
}
