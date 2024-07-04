// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "TPSDebugSubsystem.generated.h"

/**
 * GameInstanceSubsystem is a place for encapsulation of some logics
 * related to game instance, it's like actor component for actor
 */
UCLASS()
class TPSSHOOTER_API UTPSDebugSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	// Getter, returning if particular debug category is enabled or not, by its name
	bool IsCategoryEnabled(const FName& CategoryName) const;

private:

	UFUNCTION(Exec) 
	// Console command finding or adding particular drawing debug category and activating/disactivating it
	void EnableDebugCategory(const FName& CategoryName, bool bIsEnabled);

	// Map that has Debug category name as key and its state as value
	TMap<FName, bool> EnabledDebugCategories;
};
