// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AR_ActionObject.generated.h"


/**
 * 
 */
UCLASS(Abstract,Blueprintable)
class ACTIONROGUELIKE_API UAR_ActionObject : public UObject
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	void StartAction(AActor* Instigator);

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	void StopAction(AActor* Instigator);

	// Action nick name to start without object reference
	UPROPERTY(EditDefaultsOnly, Category = "Action")
	FName ActionName;
};
