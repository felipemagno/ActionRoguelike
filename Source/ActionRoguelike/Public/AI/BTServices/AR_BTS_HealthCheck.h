// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "AR_BTS_HealthCheck.generated.h"

/**
 * 
 */
UCLASS(DisplayName="HealthCheck")
class ACTIONROGUELIKE_API UAR_BTS_HealthCheck : public UBTService
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category = "AI")
	FBlackboardKeySelector HealthCheckResultKey;

	UPROPERTY(EditAnywhere, Category = "AI")
	float PercentageThreshold = 0.3f;

	// Is this service checking Health below PercentageThreshold?
	UPROPERTY(EditAnywhere, Category = "AI")
	bool bIsCheckingBelowPercentage = true;

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	virtual void OnSearchStart(FBehaviorTreeSearchData& SearchData) override;
};
