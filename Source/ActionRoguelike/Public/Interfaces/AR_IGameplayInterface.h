// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AR_IGameplayInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UAR_IGameplayInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ACTIONROGUELIKE_API IAR_IGameplayInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Interact(APawn* InstigatorPawn);

	UFUNCTION(BlueprintNativeEvent)
	void OnActorLoaded();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool ReceiveCredits(int32 CreditsReceived);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool SpendCredits(int32 CreditsCost);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	int32 GetCreditsValue();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool IsActive(); 
};
