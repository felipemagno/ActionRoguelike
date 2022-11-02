// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "AR_BTS_CheckAttackRange.generated.h"

/**
 * 
 */
UCLASS(DisplayName = "CheckAttackRange")
class ACTIONROGUELIKE_API UAR_BTS_CheckAttackRange : public UBTService
{
	GENERATED_BODY()

protected:
	// Target to comparte distance to
	UPROPERTY(EditAnywhere, Category = "AI")
	FBlackboardKeySelector WithinRangeKey;

	// Distance used to check if target is within range
	UPROPERTY(EditAnywhere, Category = "AI")
	float DistanceCheck = 500.0f;

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
