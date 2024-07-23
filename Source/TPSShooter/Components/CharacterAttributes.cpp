// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/CharacterAttributes.h"
#include "Character/TPSBaseCharacter.h"
#include "Subsystems/TPSDebugSubsystem.h"
#include "BaseData/BaseEnums.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"


UCharacterAttributes::UCharacterAttributes()
{
	
	PrimaryComponentTick.bCanEverTick = true;

	
}

void UCharacterAttributes::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


#if UE_BUILD_DEBUG || UE_BUILD_DEVELOPMENT

	DebugDrawAttributes();

#endif

}


void UCharacterAttributes::BeginPlay()
{
	Super::BeginPlay();
	checkf(MaxHealth > 0.0f, TEXT("UCharacterAttributes::BeginPlay() max health cannot be equal to 0"));
	checkf(GetOwner()->IsA<ATPSBaseCharacter>(), TEXT("UCharacterAttributes::BeginPlay(), UCharacterAttributes can be used only with ATPSBaseCharacter"));
	InBaseCharacter = StaticCast<ATPSBaseCharacter*>(GetOwner());

	Health = MaxHealth;
	InBaseCharacter->OnTakeAnyDamage.AddDynamic(this, &UCharacterAttributes::OnTakeAnyDamage);
}


#if UE_BUILD_DEBUG || UE_BUILD_DEVELOPMENT
void UCharacterAttributes::DebugDrawAttributes()
{
	UTPSDebugSubsystem* DebugSubsystem = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<UTPSDebugSubsystem>();
	if (!DebugSubsystem->IsCategoryEnabled(DebugCategoryCharacterAttributes))
	{
		return;
	}

	FVector TextLocation = InBaseCharacter->GetActorLocation() + (InBaseCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() + 5.0f) * FVector::UpVector;
	DrawDebugString(GetWorld(), TextLocation, FString::Printf(TEXT("Health: %.2f"), Health), nullptr, FColor::Green, 0.0f, true);

}
#endif


void UCharacterAttributes::OnTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (!IsAlive())
	{
		return;
	}
	
	
	Health = FMath::Clamp(Health - Damage, 0.0f, MaxHealth); // Clamp sets minimal and maximal border 
	UE_LOG(LogDamage, Warning, TEXT("UCharacterAttributes::OnTakeAnyDamage %s recieved %.2f amount of damage from %s"), *InBaseCharacter->GetName(), Damage, *DamageCauser->GetName());

	if (Health <= 0.0f)
	{
		UE_LOG(LogDamage, Warning, TEXT("UCharacterAttributes::OnTakeAnyDamage character %s is killed by an actor %s"), *InBaseCharacter->GetName(), *DamageCauser->GetName());
		if (OnDeathEvent.IsBound())
		{
			OnDeathEvent.Broadcast();
		}
	}
}




