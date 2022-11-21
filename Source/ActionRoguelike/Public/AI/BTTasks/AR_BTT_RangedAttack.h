// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Projectile/AR_BaseProjectile.h"
#include "AR_BTT_RangedAttack.generated.h"

/**
 * 
 */
UCLASS(DisplayName= "RangedAttack")
class ACTIONROGUELIKE_API UAR_BTT_RangedAttack : public UBTTaskNode
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category = "AI")
	FBlackboardKeySelector TargetActorKey;

	UPROPERTY(EditAnywhere, Category = "AI")
	TSubclassOf<AAR_BaseProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, Category = "AI")
	FName SpawnSocketName = "Muzzle_01";

	// How much random innacuracy to add to the shot being fired, this value is added in a random range from [-FireSpread_Yaw,FireSpread_Yaw]
	UPROPERTY(EditAnywhere, Category = "AI")
	float FireSpread_Yaw;

	// How much random innacuracy to add to the shot being fired, this value is added in a random range from [-FireSpread_Pitch,FireSpread_Pitch]
	UPROPERTY(EditAnywhere, Category = "AI")
	float FireSpread_Pitch;
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

public:
	UAR_BTT_RangedAttack();
};
