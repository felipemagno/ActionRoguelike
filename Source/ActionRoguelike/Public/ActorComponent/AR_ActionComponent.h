// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "AR_ActionComponent.generated.h"

class UAR_BaseAction;

UCLASS(ClassGroup=(ActionRoguelike), meta=(BlueprintSpawnableComponent))
class ACTIONROGUELIKE_API UAR_ActionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UAR_ActionComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Tags")
	FGameplayTagContainer ActiveGameplayTags;

	UFUNCTION(BlueprintCallable, Category="Action")
	void AddAction(TSubclassOf<UAR_BaseAction> NewAction);

	UFUNCTION(BlueprintCallable, Category="Action")
	bool StartAction(AActor* Instigator, FGameplayTag ActionTag);

	UFUNCTION(BlueprintCallable, Category="Action")
	bool StopAction(AActor* Instigator, FGameplayTag ActionTag);

protected:
	UPROPERTY()
	TArray<UAR_BaseAction*> Actions;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Action")
	TArray<TSubclassOf<UAR_BaseAction>> DefaultActions;

	// Called when the game starts
	virtual void BeginPlay() override;

	FGameplayTag CurrentAction;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintGetter)
	FGameplayTag GetCurrentAction();
};
