// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AR_AICharacter.h"

#include "AR_Player.h"
#include "BrainComponent.h"
#include "ActorComponent/AR_AttributeComponent.h"
#include "AI/AR_AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Interfaces/AR_IUserInterface.h"
#include "Perception/PawnSensingComponent.h"
#include "UserInterface/AR_WorldUserWidget.h"

// Sets default values
AAR_AICharacter::AAR_AICharacter()
{
	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensing");
	AttributeComponent = CreateDefaultSubobject<UAR_AttributeComponent>("Attributes");

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	TargetActorKey = "TargetActor";
	HealthValueKey = "CurrentHealth";
	HitFlashtime_ParameterName = "HitFlashTime";

	RagdollDuration = 3.0f;
}

void AAR_AICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	PawnSensingComponent->OnSeePawn.AddDynamic(this, &AAR_AICharacter::SightResponse);
	AttributeComponent->OnHealthChanged.AddDynamic(this, &AAR_AICharacter::HealthChangedResponse);
	AttributeComponent->OnDeath.AddDynamic(this, &AAR_AICharacter::DeathResponse);
}

void AAR_AICharacter::HealthChangedResponse(AActor* InstigatingActor, UAR_AttributeComponent* OwningAttribute,
                                            float NewHealthValue, float DeltaValue, float NewHealthPercentage)
{
	// make sure to not target self and that whatever hit you has a controller to be targeted
	if (InstigatingActor != this && DeltaValue < 0)
	{
		if (!ActiveHealthBar)
		{
			ActiveHealthBar = CreateWidget<UAR_WorldUserWidget>(GetWorld(), HealthBarWidgetClass);
			
			ActiveHealthBar->AttachedActor = this;
			ActiveHealthBar->AddToViewport();
		}

		//Update Health and apply UI & UX effects
		HitFlashtime_ParameterName = "HitFlashTime";
		GetMesh()->SetScalarParameterValueOnMaterials(HitFlashtime_ParameterName, GetWorld()->TimeSeconds);
		
		UAR_WorldUserWidget* DamageText = CreateWidget<UAR_WorldUserWidget>(GetWorld(),DamageTextWidgetClass);
		if(DamageText->Implements<UAR_IUserInterface>())
		{
			DamageText->AttachedActor = this;
			IAR_IUserInterface::Execute_UpdateHealthText(DamageText,DeltaValue);
			DamageText->AddToViewport();	
		}		

		//Update Health on AI
		AAR_AIController* AiController = GetController<AAR_AIController>();
		if (AiController)
		{
			UBlackboardComponent* BlackboardComponent = AiController->GetBlackboardComponent();

			BlackboardComponent->SetValueAsFloat(HealthValueKey, NewHealthValue);
		}

		//Update target actor if valid
		const APawn* InstigatingPawn = Cast<APawn>(InstigatingActor);
		if (InstigatingPawn && InstigatingPawn->Controller && Cast<AAR_Player>(InstigatingPawn))
		{
			SetTargetActor(InstigatingActor);
		}
	}
}

void AAR_AICharacter::DeathResponse(AActor* InstigatingActor, UAR_AttributeComponent* OwningAttribute)
{
	PlayAnimMontage(DeathMontage, 1);
	//SetActorEnableCollision(false);

	//Disable Brain and Behavior Tree
	AAIController* AIC = Cast<AAIController>(GetController());
	if (IsValid(AIC))
	{
		AIC->GetBrainComponent()->StopLogic("Enemy Killed");
	}

	//Ragdoll
	GetMesh()->SetAllBodiesSimulatePhysics(true);
	GetMesh()->SetCollisionProfileName("Ragdoll");

	//Disable capsule collision
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	//Disable character movement
	GetCharacterMovement()->DisableMovement();

	SetLifeSpan(RagdollDuration);
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
