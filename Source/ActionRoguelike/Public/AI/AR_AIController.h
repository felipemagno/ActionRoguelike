// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AR_AIController.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API AAR_AIController : public AAIController
{
	GENERATED_BODY()

protected:
	
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="AI")
	UBehaviorTree* BehaviorTree;
};
