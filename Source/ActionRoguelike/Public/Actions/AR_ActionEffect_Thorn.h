// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actions/AR_BaseActionEffect.h"
#include "AR_ActionEffect_Thorn.generated.h"

class UAR_AttributeComponent;

/**
 * 
 */
UCLASS(Abstract)
class ACTIONROGUELIKE_API UAR_ActionEffect_Thorn : public UAR_BaseActionEffect
{
	GENERATED_BODY()

	UAR_ActionEffect_Thorn();
protected:
	UFUNCTION()
	void ThornDamage(AActor* InstigatingActor, UAR_AttributeComponent* OwningAttribute, float NewHealthValue,
	                 float DeltaValue, float NewHealthPercentage);
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Effect")
	float DamageReflectedPercentage;


	virtual void StartAction_Implementation(AActor* Instigator) override;
	virtual void StopAction_Implementation(AActor* Instigator) override;
};
