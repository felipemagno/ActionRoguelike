// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/NoExportTypes.h"
#include "AR_BaseAction.generated.h"

class UAR_ActionComponent;
/**
 * 
 */
UCLASS(Abstract, Blueprintable)
class ACTIONROGUELIKE_API UAR_BaseAction : public UObject
{
	GENERATED_BODY()
public:
	UAR_BaseAction();
protected:
	// Tags granted to owning actor while this Action is being executed
	UPROPERTY(EditDefaultsOnly, Category="Tags")
	FGameplayTagContainer GrantedTags;

	// Action can only start if owning actor has none of these tags applied
	UPROPERTY(EditDefaultsOnly, Category="Tags")
	FGameplayTagContainer BlockedTags;

	// Return owning Action Component
	UFUNCTION(BlueprintCallable, Category = "Action")
	UAR_ActionComponent* GetOwningComponent() const;

	UPROPERTY(ReplicatedUsing="OnRep_IsRunning")
	bool bIsRunning;

	UFUNCTION()
	void OnRep_IsRunning();

public:
	// Starts automatically when added 
	UPROPERTY(EditDefaultsOnly, Category="Action")
	bool AutoStart;

	// Action tag to start without object reference
	UPROPERTY(EditDefaultsOnly, Category="Tags")
	FGameplayTag ActionTag;

	UFUNCTION(BlueprintGetter, Category= "Action")
	bool IsRunning() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Action")
	bool CanStartAction(AActor* Instigator);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Action")
	void StartAction(AActor* Instigator);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Action")
	void StopAction(AActor* Instigator);

	virtual bool IsSupportedForNetworking() const override { return true; };

	virtual UWorld* GetWorld() const override;
};
