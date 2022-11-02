// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTServices/AR_BTS_CheckAttackRange.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

void UAR_BTS_CheckAttackRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	// Check distance between AI Pawn and target Actor

	auto* BlackBoard = OwnerComp.GetBlackboardComponent();
	if (ensure(BlackBoard))
	{
		AActor* TargetActor = Cast<AActor>(BlackBoard->GetValueAsObject(("TargetActor")));
		if (TargetActor)
		{
			auto* AIController = OwnerComp.GetAIOwner();
			if (ensure(AIController))
			{
				APawn* AIPawn = AIController->GetPawn();
				if (ensure(AIPawn))
				{
					float DistanceTo = FVector::DistSquared(TargetActor->GetActorLocation(),
					                                        AIPawn->GetActorLocation());

					bool bIsWithinRange = DistanceTo < DistanceCheck * DistanceCheck;
					if (bIsWithinRange)
					{
						// The check is already true for being within range, now check if it is also within sight
						bool bIsInSight = AIController->LineOfSightTo(TargetActor);
						BlackBoard->SetValueAsBool(WithinRangeKey.SelectedKeyName, bIsInSight);
					}
					else
					{
						BlackBoard->SetValueAsBool(WithinRangeKey.SelectedKeyName, false);
					}
				}
			}
		}
	}
}
