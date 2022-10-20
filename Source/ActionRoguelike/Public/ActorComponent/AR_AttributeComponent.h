// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AR_AttributeComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(FOnHealthChanged, AActor*, InstigatingActor, UAR_AttributeComponent*,
                                              OwningAttribute, float, NewHealthValue, float, DeltaValue, float,
                                              NewHealthPercentage);

UCLASS(ClassGroup=(ActionRoguelike), meta=(BlueprintSpawnableComponent))
class ACTIONROGUELIKE_API UAR_AttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UAR_AttributeComponent();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
	float Health;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
	float HealthMax;

public:
	UPROPERTY(BlueprintAssignable)
	FOnHealthChanged OnHealthChanged;

	UFUNCTION(BlueprintCallable)
	bool ApplyHealthChange(float Delta);
};
