// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actions/AR_BaseAction.h"
#include "AR_BaseActionEffect.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class ACTIONROGUELIKE_API UAR_BaseActionEffect : public UAR_BaseAction
{
	GENERATED_BODY()
public:
	UAR_BaseActionEffect();
	virtual void StartAction_Implementation(AActor* Instigator) override;
	virtual void StopAction_Implementation(AActor* Instigator) override;

protected:
	// Duration of given effect
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Effect")
	float Duration;

	// Period interval for its "Tick" activation
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Effect")
	float Period;

	FTimerHandle DurationTimer;
	FTimerHandle PeriodTimer;

	UFUNCTION(BlueprintNativeEvent, Category="Effect")
	void ExecutePeriodicEffect(AActor* Instigator);
};
