// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GenericTeamAgentInterface.h"
#include "BaseData/BaseEnums.h"
#include "TPSBaseCharacter.generated.h"

class UBaseCharacterMovementComponent; // Custom character movement component for Sprint implementation
class UCharacterEquipmentComponent;
class UAnimMontage;
class AInteractiveActor;
class ALadder;
class UCharacterAttributes;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnAimingStateChanged, bool)

//Declare a structure that will hold parameters for mantling animation correct play
USTRUCT(BlueprintType)
struct FMantlingSettings
{
	GENERATED_BODY();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	// Animation montage we are going to play
	UAnimMontage* MantlingMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	// Animation montage we are going to play for First Person Player
	UAnimMontage* FirstPersonMantlingMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	// Curve responsible for correct synchronization of playing animation and character's physical movement
	class UCurveVector* MantlingCurve; 

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = 0.0f, UIMin = 0.0f))
	float AnimationCorrectionXY = 65.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = 0.0f, UIMin = 0.0f))
	float AnimationCorrectionZ = 200.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = 0.0f, UIMin = 0.0f))
	// Maximal height that our character can visually reach using appropriate mantling animation
	float MaxHeight = 200.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = 0.0f, UIMin = 0.0f))
	// Minimal height that our character can visually reach using appropriate mantling animation
	float MinHeight = 100.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = 0.0f, UIMin = 0.0f))
	// If the height of potential ledge is MaxHeight, Start time within MantlingCurve is 0, as we move the character's collision to more distant location and animation takes longer
	float MaxHeightStartTime = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = 0.0f, UIMin = 0.0f))
	// If the height of potential ledge is MinHeight, Start time within MantlingCurve is 0.5, as we move the character's collision to nearer location and animation takes shorter
	float MinHeightStartTime = 0.5f;

};

// TInlineAllocator<size> works like Reserve, but hold necessary memory not in OZU but directly in class, works faster from cach perspective
typedef TArray<AInteractiveActor*, TInlineAllocator<10>> TInteractiveActorsArray; 

// This class is abstract because we don't want to instantiate from base character
// and here we will declare mostly pure virtual methods that we will override in
// derived classes, it's just a level of abstraction

UCLASS(Abstract, NotBlueprintable)
class TPSSHOOTER_API ATPSBaseCharacter : public ACharacter, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	 
	// As we want to change the base component with the customly created one, we should use FObjectInitializer (SetDefaultSubobjectClass)
	ATPSBaseCharacter(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	virtual void PossessedBy(AController* NewController) override;

	//declaring pure virtual methods in an abstract base class to implement them in derived classes
	virtual void MoveForward(float Value) {};
	virtual void MoveRight(float Value) {};
	virtual void Turn(float Value) {};
	virtual void LookUp(float Value) {};

	virtual void SwimForward(float Value) {};
	virtual void SwimRight(float Value) {};
	virtual void SwimUp(float Value) {};
	
	virtual void StartSprint();
	virtual void EndSprint();
	virtual void ChangeCrouchState();

	UFUNCTION(BlueprintCallable)
	void Mantle(bool bForce = false);

	virtual bool CanJumpInternal_Implementation() const override;

	// Methods responsible for movement on ladder

	void ClimbLadderUp(float Value);
	void InteractWithLadder();
	const ALadder* GetAvailableLadder() const;

	// pure virtual method of ACharacter, is called when a character is falling
	virtual void Falling() override; 

	// virtual method of ACharacter and UCharacterMovementComponent, triggers when a character reaches the peak of jumping (bNotifyApex is TRUE)
	virtual void NotifyJumpApex() override; 

	// virtual method of ACharacter, triggers when a character has landed on some surface
	virtual void Landed(const FHitResult& Hit) override; 


	void StartFire();
	void StopFire();

	void StartAiming();
	void StopAiming();

	void Reload();

	void NextItem();
	void PreviousItem();

	void EquipPrimaryItem();

	void PrimaryMeleeAttack();
	void SecondaryMeleeAttack();

	/* IGenericTeamAgentInterface */
	virtual FGenericTeamId GetGenericTeamId() const override;
	/* IGenericTeamAgentInterface */


	// All UFUNCTION(BlueprintNativeEvent) s implementation in .cpp files should be augmented with _Implementation suffix

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Character")
	void OnStartAiming();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Character")
	void OnStopAiming();

	float GetAimingMovementSpeed() const;

	inline bool GetIsAiming() const { return bIsAiming; }

	FOnAimingStateChanged OnAimingStateChanged;

	// Getter of new customly created movement component
	inline UBaseCharacterMovementComponent* GetBaseCharacterMovement() const { return BaseCharacterMovementComponent; }

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character | Movement")
	class ULedgeDetectorComponent* LedgeDetectorComponent;


	inline const UCharacterEquipmentComponent* GetCharacterEquipmentComponent()const { return CharacterEquipmentComponent; }
	inline UCharacterEquipmentComponent* GetCharacterEquipmentComponent_Mutable()const { return CharacterEquipmentComponent; }
	inline const UCharacterAttributes* GetCharacterAttributesComponent() const { return CharacterAttributesComponent; }

	void RegisterInteractiveActor(AInteractiveActor* InInteractiveActor);
	void UnRegisterInteractiveActor(AInteractiveActor* InInteractiveActor);

	


protected:

	bool CanMantle() const;

	virtual void OnMantle(const FMantlingSettings& MantlingSettings, float MantlingAnimationStart);

	// Ensure communicatino between C++ and Blueprints, make the function's implementation in blueprints
	UFUNCTION(BlueprintImplementableEvent, Category = "Character | Movement")
	void OnStartSprint();

	UFUNCTION(BlueprintImplementableEvent, Category = "Character | Movement")
	void OnEndSprint();


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character | Movement | Mantling")
	FMantlingSettings HighMantleSettings;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character | Movement | Mantling")
	FMantlingSettings LowMantleSettings;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = 0, UIMin = 0))
	float LowMantleMaxHeight = 125.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character | Components")
	// Component responsible for holding equipable items 
	UCharacterEquipmentComponent* CharacterEquipmentComponent;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character | Components")
	// Component responsible for health and its debugging
	UCharacterAttributes* CharacterAttributesComponent;

	virtual void OnDeath();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character | Animations")
	UAnimMontage* OnDeathAnimMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character | Attributes")
	class UCurveFloat* FallDamageCurve;


	// Methods designed to encapsulate base logics of BlueprintNativeEvents
	virtual void OnStartAimingInternal();
	virtual void OnStopAimingInternal();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character | Team")
	ETeams Team = ETeams::Enemy;

private:
	// Declare customly created movement component
	 UBaseCharacterMovementComponent* BaseCharacterMovementComponent;
	 void TryChangeSprintState();

	 // Bool variable that is triggered by Start/End sprint functions in this class called from player controller class
	 // is used to check every frame if the character can sprint
	 bool bShouldSprint = false;

	 bool bIsAiming = false;

	 // Select appropriate mantling settings for particular ledge (low mantle setting or high mantle settings)
	 const FMantlingSettings& GetMantlingSettings(float LedgeHeight) const;

	 // Array containing all available interactive actors, like ladders etc
	TInteractiveActorsArray AvailableInteractiveActors; // is typedef type, see initialization before class
	 // TInlineAllocator<size> works like Reserve, but hold necessary memory not in OZU but directly in class, works faster from cach perspective

	void EnableRagdoll();

	FVector CurrentFallApex;
	float CurrentAimingMovementSpeed;
};
