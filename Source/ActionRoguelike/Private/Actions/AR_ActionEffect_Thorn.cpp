// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions/AR_ActionEffect_Thorn.h"

#include "ActorComponent/AR_ActionComponent.h"
#include "ActorComponent/AR_AttributeComponent.h"
#include "Core/AR_GameplayFunctionLibrary.h"

UAR_ActionEffect_Thorn::UAR_ActionEffect_Thorn()
{
	DamageReflectedPercentage = 0.6f;
}

void UAR_ActionEffect_Thorn::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	UAR_AttributeComponent* AttributeComponent = Cast<UAR_AttributeComponent>(
		GetOwningComponent()->GetOwner()->GetComponentByClass(UAR_AttributeComponent::StaticClass()));
	if (IsValid(AttributeComponent))
	{
		AttributeComponent->OnHealthChanged.AddDynamic(this, &UAR_ActionEffect_Thorn::ThornDamage);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(0, 2, FColor::Red, "Thorn Action cannot be used without Attribute Component");
		StopAction(Instigator);
	}
}

void UAR_ActionEffect_Thorn::StopAction_Implementation(AActor* Instigator)
{
	Super::StopAction_Implementation(Instigator);

	UAR_AttributeComponent* AttributeComponent = Cast<UAR_AttributeComponent>(
	GetOwningComponent()->GetOwner()->GetComponentByClass(UAR_AttributeComponent::StaticClass()));
	if (IsValid(AttributeComponent))
	{
		AttributeComponent->OnHealthChanged.RemoveDynamic(this, &UAR_ActionEffect_Thorn::ThornDamage);
		AttributeComponent = nullptr;
	}	
}

void UAR_ActionEffect_Thorn::ThornDamage(AActor* InstigatingActor, UAR_AttributeComponent* OwningAttribute,
	float NewHealthValue, float DeltaValue, float NewHealthPercentage)
{
	// DO NOT DAMAGE SELF OR REFLECT HEALING
	AActor* Owner = GetOwningComponent()->GetOwner();
	if (Owner == InstigatingActor || DeltaValue >= 0)
	{
		return;
	}

	// REFLECT DAMAGE BACK
	const float DamageAmount = FMath::RoundToFloat(-DeltaValue * DamageReflectedPercentage);
	UAR_GameplayFunctionLibrary::ApplyDamage(Owner,InstigatingActor,DamageAmount);
}