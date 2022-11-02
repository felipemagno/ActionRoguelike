// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponent/AR_AttributeComponent.h"


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

bool UAR_AttributeComponent::ApplyHealthChange(float Delta)
{
	if (bGodMode) return false;
	if (Health <= 0)
	{
		return false;
	}

	Health = FMath::Clamp(Health + Delta, 0, HealthMax);

	OnHealthChanged.Broadcast(nullptr, this, Health, Delta, Health / HealthMax);
	if (Health == 0)
	{
		OnDeath.Broadcast(nullptr, this);
	}

	return true;
}

bool UAR_AttributeComponent::ApplyMaxHeal()
{
	if (IsFullHealth() || Health <= 0) return false;
	float Delta = HealthMax - Health;
	Health = HealthMax;
	OnHealthChanged.Broadcast(nullptr, this, Health, Delta, Health / HealthMax);
	return true;
}

bool UAR_AttributeComponent::IsFullHealth() const
{
	return !(Health < HealthMax);
}

float UAR_AttributeComponent::GetHealthPercentage() const
{
	return Health / HealthMax;
}
