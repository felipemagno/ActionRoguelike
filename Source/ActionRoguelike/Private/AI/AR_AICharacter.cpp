// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AR_AICharacter.h"

#include "AR_Character.h"
#include "ActorComponent/AR_AttributeComponent.h"
#include "AI/AR_AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/PawnSensingComponent.h"

// Sets default values
AAR_AICharacter::AAR_AICharacter()
{
	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensing");
	AttributeComponent = CreateDefaultSubobject<UAR_AttributeComponent>("Attributes");

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	TargetActorKey = "TargetActor";
	HitFlashtime_ParameterName = "HitFlashTime";
}

void AAR_AICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	PawnSensingComponent->OnSeePawn.AddDynamic(this, &AAR_AICharacter::SightResponse);
	AttributeComponent->OnHealthChanged.AddDynamic(this, &AAR_AICharacter::HealthChangedResponse);
}

void AAR_AICharacter::HealthChangedResponse(AActor* InstigatingActor, UAR_AttributeComponent* OwningAttribute,
                                            float NewHealthValue, float DeltaValue, float NewHealthPercentage)
{
	// make sure to not target self and that whatever hit you has a controller to be targeted
	if (InstigatingActor != this)
	{
		const APawn* InstigatingPawn = Cast<APawn>(InstigatingActor);

		HitFlashtime_ParameterName = "HitFlashTime";
		GetMesh()->SetScalarParameterValueOnMaterials(HitFlashtime_ParameterName, GetWorld()->TimeSeconds);
		if (InstigatingPawn && InstigatingPawn->Controller && Cast<AAR_Character>(InstigatingPawn))
		{
			SetTargetActor(InstigatingActor);
		}
	}
}


void AAR_AICharacter::SetTargetActor(AActor* Actor)
{
	AAR_AIController* AiController = GetController<AAR_AIController>();
	if (AiController)
	{
		UBlackboardComponent* BlackboardComponent = AiController->GetBlackboardComponent();

		BlackboardComponent->SetValueAsObject(TargetActorKey, Actor);
	}
}

void AAR_AICharacter::SightResponse(APawn* Pawn)
{
	SetTargetActor(Pawn);
	DrawDebugString(GetWorld(), GetActorLocation(), "PLAYER SPOTTED", nullptr, FColor::Black, 4.0f, true);
}
