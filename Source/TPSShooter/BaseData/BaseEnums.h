// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


// As user defined collision channels and collision objects are by default assigned to ECC_GameTraceChannel[number] in EngineTypes.h
// we want to just simplify their call by using preprocessor #define to replace their names by more
// user friendly names

#define ECC_Bullet ECC_GameTraceChannel1
#define ECC_Climbing ECC_GameTraceChannel2
#define ECC_InteractionVolume ECC_GameTraceChannel3
#define ECC_Melee ECC_GameTraceChannel4

UENUM(BlueprintType)
enum class EEquipedItemType : uint8
{
	None,
	Pistol,
	AssaultRifle,
	Throwable,
	Melee,
	MAX UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EAmunitionType : uint8
{
	None,
	Pistol,
	AssaultRifle,
	ShotgunShells,
	MAX UMETA(Hidden)
};

UENUM(BlueprintType)
enum class ECustomMovementMode : uint8
{
	CMOVE_None = 0 UMETA(DisplayName = "None"),
	CMOVE_Mantling UMETA(DisplayName = "Mantling"),
	CMOVE_Ladder UMETA(DisplayName = "Ladder"),
	CMOVE_Max UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EEquipmentSlots :uint8
{
	None,
	SideArm,
	PrimaryWeapon,
	SecondaryWeapon,
	PrimaryItemSlot,
	Melee,
	MAX UMETA(hidden)
};

UENUM(BlueprintType)
enum class EReticleType : uint8
{
	None,
	Default,
	SniperRifle,
	MAX UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EMeleeAttackTypes: uint8
{
	None,
	PrimaryAttack,
	SecondaryAttack,
	MAX UMETA(Hidden)
};

UENUM(BlueprintType)
enum class ETeams : uint8
{
	Player,
	Enemy
};

// Set Collision profile names to get them when we want to change collision profile through code

const FName CollisionProfilePawn = FName("Pawn");
const FName CollisionProfilePawnInteractionVolume = FName("PawnInteractionVolume");
const FName CollisionProfileRagdoll = FName("Ragdoll");
const FName CollisionProfileNoCollision = FName("NoCollision");

const FName SocketFPCamera = FName("CameraSocket");
const FName SocketCharacterWeapon = FName("SocketWeapon");
const FName SocketWeaponMuzzle = FName("SocketMuzzle");
const FName SocketForegrip = FName("SocketForegrip");
const FName SocketThrowableItem = FName("SocketThrowable");

const FName DebugCategoryLedgeDetection = FName("LedgeDetection");
const FName DebugCategoryCharacterAttributes = FName("CharacterAttributes");
const FName DebugCategoryRangeWeapon = FName("RangeWeapon");
const FName DebugCategoryMelee = FName("Melee");


const FName FXParamTraceEnd = FName("TraceEnd");

const FName SectionMontageReloadEnd = FName("EndReload");

const FName BB_CurrentTarget = FName("CurrentTarget");
const FName BB_NextLocation = FName("NextLocation");