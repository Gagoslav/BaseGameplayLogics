// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


// As user defined collision channels and collision objects are by default assigned to ECC_GameTraceChannel[number] in EngineTypes.h
// we want to just simplify their call by using preprocessor #define to replace their names by more
// user friendly names

#define ECC_Bullet ECC_GameTraceChannel1
#define ECC_Climbing ECC_GameTraceChannel2
#define ECC_InteractionVolume ECC_GameTraceChannel3


UENUM(BlueprintType)
enum class EEquipedItemType : uint8
{
	None,
	Pistol,
	AssaultRifle
};

UENUM(BlueprintType)
enum class ECustomMovementMode : uint8
{
	CMOVE_None = 0 UMETA(DisplayName = "None"),
	CMOVE_Mantling UMETA(DisplayName = "Mantling"),
	CMOVE_Ladder UMETA(DisplayName = "Ladder"),
	CMOVE_Max UMETA(Hidden)
};

// Set Collision profile names to get them when we want to change collision profile through code

const FName CollisionProfilePawn = FName("Pawn");
const FName CollisionProfilePawnInteractionVolume = FName("PawnInteractionVolume");
const FName CollisionProfileRagdoll = FName("Ragdoll");

const FName SocketFPCamera = FName("CameraSocket");
const FName SocketCharacterWeapon = FName("SocketWeapon");
const FName SocketWeaponMuzzle = FName("SocketMuzzle");
const FName SocketForegrip = FName("SocketForegrip");

const FName DebugCategoryLedgeDetection = FName("LedgeDetection");
const FName DebugCategoryCharacterAttributes = FName("CharacterAttributes");
const FName DebugCategoryRangeWeapon = FName("RangeWeapon");

const FName FXParamTraceEnd = FName("TraceEnd");
