// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTasks/AR_BTT_MaxHeal.h"

#include "AIController.h"
#include "ActorComponent/AR_AttributeComponent.h"
#include "GameFramework/Character.h"

EBTNodeResult::Type UAR_BTT_MaxHeal::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ACharacter* Character = Cast<ACharacter>(OwnerComp.GetAIOwner()->GetPawn());

	UAR_AttributeComponent* Attribute = Cast<UAR_AttributeComponent>(
		Character->GetComponentByClass(UAR_AttributeComponent::StaticClass()));
	if (!Attribute) return EBTNodeResult::Failed;

	return Attribute->ApplyMaxHeal(Character) ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
}
