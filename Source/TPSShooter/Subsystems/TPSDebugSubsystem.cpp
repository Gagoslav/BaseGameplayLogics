// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystems/TPSDebugSubsystem.h"


void UTPSDebugSubsystem::EnableDebugCategory(const FName& CategoryName, bool bIsEnabled)
{
	// This is a setter for EnabledDebugCategories map that sets the value for key defined as name
	EnabledDebugCategories.FindOrAdd(CategoryName);
	EnabledDebugCategories[CategoryName] = bIsEnabled;
}

bool UTPSDebugSubsystem::IsCategoryEnabled(const FName& CategoryName) const
{
	// This is a getter that accesses the EnabledDebugCategories map and returns bool if it
	// could find the necessary debug category by its name
	const bool* bIsEnabled = EnabledDebugCategories.Find(CategoryName);
	return bIsEnabled != nullptr && *bIsEnabled;
}