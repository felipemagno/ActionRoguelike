// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "AR_BTT_MaxHeal.generated.h"

/**
 * 
 */
UCLASS(DisplayName = "MaxHeal")
class ACTIONROGUELIKE_API UAR_BTT_MaxHeal : public UBTTaskNode
{
	GENERATED_BODY()

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
