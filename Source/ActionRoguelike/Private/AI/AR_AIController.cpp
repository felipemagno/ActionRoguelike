// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AR_AIController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

void AAR_AIController::BeginPlay()
{
	Super::BeginPlay();

	RunBehaviorTree(BehaviorTree);

	auto* Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (Player)
	{
		GetBlackboardComponent()->SetValueAsVector("MoveToLocation", Player->GetActorLocation());

		GetBlackboardComponent()->SetValueAsObject("TargetActor", Player);
	}
}
