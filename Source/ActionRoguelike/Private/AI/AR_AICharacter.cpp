// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AR_AICharacter.h"

#include "ActorComponent/AR_AttributeComponent.h"
#include "AI/AR_AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/PawnSensingComponent.h"

// Sets default values
AAR_AICharacter::AAR_AICharacter()
{
	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensing");
	AttributeComponent = CreateDefaultSubobject<UAR_AttributeComponent>("Attributes");


	TargetActorKey = "TargetActor";
}

void AAR_AICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	PawnSensingComponent->OnSeePawn.AddDynamic(this, &AAR_AICharacter::SightResponse);
}

void AAR_AICharacter::SightResponse(APawn* Pawn)
{
	AAR_AIController* AiController = GetController<AAR_AIController>();
	if (AiController)
	{
		UBlackboardComponent* BlackboardComponent = AiController->GetBlackboardComponent();

		BlackboardComponent->SetValueAsObject(TargetActorKey, Pawn);

		DrawDebugString(GetWorld(), GetActorLocation(), "PLAYER SPOTTED", nullptr, FColor::Black, 4.0f, true);
	}
}
