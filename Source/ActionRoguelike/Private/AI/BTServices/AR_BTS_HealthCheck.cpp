// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTServices/AR_BTS_HealthCheck.h"

#include "AIController.h"
#include "ActorComponent/AR_AttributeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

UAR_AttributeComponent* AttributeComponent;
UBlackboardComponent* BlackBoard;

void UAR_BTS_HealthCheck::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	if (!AttributeComponent)
	{
		AttributeComponent = Cast<UAR_AttributeComponent>(
			OwnerComp.GetAIOwner()->GetPawn()->GetComponentByClass(UAR_AttributeComponent::StaticClass()));
	}

	if (ensure(AttributeComponent) && ensure(BlackBoard))
	{
		bool HealthCheckResult = AttributeComponent->GetHealthPercentage() < PercentageThreshold;
		if (!bIsCheckingBelowPercentage)
			HealthCheckResult = !HealthCheckResult;
		BlackBoard->SetValueAsBool(HealthCheckResultKey.SelectedKeyName, HealthCheckResult);
	}
}


void UAR_BTS_HealthCheck::OnSearchStart(FBehaviorTreeSearchData& SearchData)
{
	Super::OnSearchStart(SearchData);

	BlackBoard = SearchData.OwnerComp.GetBlackboardComponent();
}
