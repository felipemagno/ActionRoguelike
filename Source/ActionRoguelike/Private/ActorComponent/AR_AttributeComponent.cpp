// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponent/AR_AttributeComponent.h"

#include "Core/AR_GameMode.h"
#include "Net/UnrealNetwork.h"

static TAutoConsoleVariable<float> CVarDamageMultiplier(
	TEXT("ar_DamageMultiplier"), 1.0f,TEXT("Multiplies all damage in game"),
	ECVF_Cheat);

static TAutoConsoleVariable<float> CVarRageMultiplier(
	TEXT("ar_RageMultiplier"), 1.0f,TEXT("Multiplies all rage gained or lost in game"),
	ECVF_Cheat);

// Sets default values for this component's properties
UAR_AttributeComponent::UAR_AttributeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
	HealthMax = Health = 100;
	bGodMode = false;
	Rage = 0;
	RageMax = 120;

	SetIsReplicatedByDefault(true);
}

void UAR_AttributeComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UAR_AttributeComponent, Health);
	DOREPLIFETIME(UAR_AttributeComponent, HealthMax);
	DOREPLIFETIME(UAR_AttributeComponent, Rage);
	DOREPLIFETIME(UAR_AttributeComponent, RageMax);
}

bool UAR_AttributeComponent::IsAlive() const
{
	return Health > 0;
}

bool UAR_AttributeComponent::ApplyHealthChange(AActor* InstigatingActor, float Delta)
{
	if (bGodMode) return false;
	// if (!GetOwner()->CanBeDamaged())
	// {
	// 	// "God" cheat sets "can be damaged" to false
	// 	UE_LOG(LogTemp,Log,TEXT("%s cannot be damaged"),GetOwner());
	// }

	if (Health <= 0 || FMath::IsNearlyZero(Delta))
	{
		return false;
	}

	float OldHealth = Health;
	float NewHealth = FMath::Clamp(Health + Delta, 0, HealthMax); 
	float ActualDelta = NewHealth - OldHealth;
	Delta *= CVarDamageMultiplier.GetValueOnGameThread();
	
	if (!GetOwner()->HasAuthority())
	{
		if (ActualDelta < 0)
		{
			ApplyRageChange(InstigatingActor, -ActualDelta);
		}

		if (!FMath::IsNearlyZero(ActualDelta))
		Health = NewHealth;
		MulticastHealthChanged(InstigatingActor, Health, ActualDelta, Health / HealthMax);

		if (ActualDelta < 0 && Health == 0)
		{
			MulticastOnDeath(InstigatingActor);
			auto* GM = GetWorld()->GetAuthGameMode<AAR_GameMode>();
			if (GM)
				GM->ActorKilledEvent(GetOwner(), InstigatingActor);
		}
	}
	return !FMath::IsNearlyZero(ActualDelta);
}

bool UAR_AttributeComponent::ApplyMaxHeal(AActor* InstigatingActor)
{
	if (IsFullHealth() || Health <= 0) return false;
	float Delta = HealthMax - Health;
	Health = HealthMax;
	MulticastHealthChanged(InstigatingActor, Health, Delta, Health / HealthMax);
	return true;
}

bool UAR_AttributeComponent::ApplyRageChange(AActor* InstigatingActor, float Delta)
{
	if (FMath::IsNearlyZero(Delta))
	{
		return false;
	}

	Delta *= CVarRageMultiplier.GetValueOnGameThread();

	Rage = FMath::Clamp(Rage + Delta, 0, RageMax);

	MulticastRageChange(InstigatingActor, Rage, Delta, Rage / RageMax);
	return true;
}

void UAR_AttributeComponent::ToogleGodMode()
{
	bGodMode = !bGodMode;
}

bool UAR_AttributeComponent::IsFullHealth() const
{
	return !(Health < HealthMax);
}

float UAR_AttributeComponent::GetHealthPercentage() const
{
	return Health / HealthMax;
}

void UAR_AttributeComponent::MulticastHealthChanged_Implementation(AActor* InstigatingActor, float NewHealthValue,
                                                                   float DeltaValue, float NewHealthPercentage)
{
	OnHealthChanged.Broadcast(InstigatingActor, this, NewHealthValue, DeltaValue, NewHealthPercentage);
}

void UAR_AttributeComponent::MulticastOnDeath_Implementation(AActor* InstigatingActor)
{
	OnDeath.Broadcast(InstigatingActor, this);
}

void UAR_AttributeComponent::MulticastRageChange_Implementation(AActor* InstigatingActor, float NewRageValue,
                                                                float DeltaValue, float NewRagePercentage)
{
	OnRageChanged.Broadcast(InstigatingActor, this, NewRageValue, DeltaValue, NewRagePercentage);
}
