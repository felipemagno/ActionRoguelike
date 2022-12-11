// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponent/AR_AttributeComponent.h"

#include "Core/AR_GameMode.h"

static TAutoConsoleVariable<float> CVarDamageMultiplier(
	TEXT("ar_DamageMultiplier"), 1.0f,TEXT("Multiplies all damage in game"),
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

	if (Delta < 0)
	{
		Delta *= CVarDamageMultiplier.GetValueOnGameThread();
	}

	Health = FMath::Clamp(Health + Delta, 0, HealthMax);

	OnHealthChanged.Broadcast(InstigatingActor, this, Health, Delta, Health / HealthMax);
	if (Health == 0)
	{
		OnDeath.Broadcast(InstigatingActor, this);
		auto* GM = GetWorld()->GetAuthGameMode<AAR_GameMode>();
		if (GM)
			GM->ActorKilledEvent(GetOwner(), InstigatingActor);
	}

	return true;
}

bool UAR_AttributeComponent::ApplyMaxHeal(AActor* InstigatingActor)
{
	if (IsFullHealth() || Health <= 0) return false;
	float Delta = HealthMax - Health;
	Health = HealthMax;
	OnHealthChanged.Broadcast(InstigatingActor, this, Health, Delta, Health / HealthMax);
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
