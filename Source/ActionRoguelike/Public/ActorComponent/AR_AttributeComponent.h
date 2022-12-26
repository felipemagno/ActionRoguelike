// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AR_AttributeComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(FOnHealthChanged, AActor*, InstigatingActor, UAR_AttributeComponent*,
                                              OwningAttribute, float, NewHealthValue, float, DeltaValue, float,
                                              NewHealthPercentage);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(FOnRageChanged, AActor*, InstigatingActor, UAR_AttributeComponent*,
											  OwningAttribute, float, NewRageValue, float, DeltaValue, float,
											  NewRagePercentage);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDeath, AActor*, InstigatingActor, UAR_AttributeComponent*,
                                             OwningAttribute);

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes")
	bool bGodMode;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
	float Rage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
	float RageMax;
	
public:
	UPROPERTY(BlueprintAssignable)
	FOnHealthChanged OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnDeath OnDeath;

	UPROPERTY(BlueprintAssignable)
	FOnRageChanged OnRageChanged;

	UFUNCTION(BlueprintCallable)
	bool IsAlive() const;

	UFUNCTION(BlueprintCallable)
	bool IsFullHealth() const;
	
	UFUNCTION(BlueprintCallable)
	float GetHealthPercentage() const;

	UFUNCTION(BlueprintCallable)
	bool ApplyHealthChange(AActor* InstigatingActor, float Delta);

	UFUNCTION(BlueprintCallable)
	bool ApplyMaxHeal(AActor* InstigatingActor);

	UFUNCTION(BlueprintCallable)
	bool ApplyRageChange(AActor* InstigatingActor, float Delta);

	UFUNCTION()
	void ToogleGodMode();

};
