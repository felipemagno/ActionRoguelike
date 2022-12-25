// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/AR_GameplayFunctionLibrary.h"

#include "ActorComponent/AR_AttributeComponent.h"

bool UAR_GameplayFunctionLibrary::ApplyDamage(AActor* DamageInstigator, AActor* TargetActor, float DamageAmount)
{
	UAR_AttributeComponent* Attribute = Cast<UAR_AttributeComponent>(
		TargetActor->GetComponentByClass(UAR_AttributeComponent::StaticClass()));
	if (Attribute)
	{
		return Attribute->ApplyHealthChange(DamageInstigator, -DamageAmount);
	}
	else
	{
		return false;
	}
}

bool UAR_GameplayFunctionLibrary::ApplyDirectionalDamage(AActor* DamageInstigator, AActor* TargetActor,
                                                         float DamageAmount, const FHitResult& HitResult, float ImpulseStrength)
{
	if (ApplyDamage(DamageInstigator, TargetActor, DamageAmount))
	{
		UPrimitiveComponent* HitComponent = HitResult.GetComponent();
		if(HitComponent && HitComponent->IsSimulatingPhysics(HitResult.BoneName))
		{
			FVector Direction = HitResult.ImpactPoint - HitResult.TraceStart;
			Direction.Normalize();
			HitComponent->AddImpulseAtLocation(Direction * ImpulseStrength,HitResult.ImpactPoint,HitResult.BoneName);
		}
		return true;
	}
	else
	{
		return false;
	}
}
