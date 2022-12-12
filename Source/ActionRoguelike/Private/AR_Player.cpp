// Fill out your copyright notice in the Description page of Project Settings.


#include "AR_Player.h"

#include "ActorComponent/AR_InteractionComponent.h"
#include "Projects.h"
#include "ActorComponent/AR_ActionComponent.h"
#include "ActorComponent/AR_AttributeComponent.h"
#include "Camera/CameraComponent.h"
#include "Core/AR_PlayerState.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
/////////////////////
// SETUP FUNCTIONS

// Sets default values
AAR_Player::AAR_Player()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->bUsePawnControlRotation = true;

	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(SpringArmComp);

	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;

	InteractionComp = CreateDefaultSubobject<UAR_InteractionComponent>("InteractionComponent");
	AttributeComp = CreateDefaultSubobject<UAR_AttributeComponent>("AttributeComponent");
	ActionComponent = CreateDefaultSubobject<UAR_ActionComponent>("ActionComponent");

	HitFlashTime_ParameterName = "HitTime";
}

// Called to bind functionality to input
void AAR_Player::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AAR_Player::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AAR_Player::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("PrimaryAttack", IE_Pressed, this, &AAR_Player::PrimaryAttack);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AAR_Player::Jump);
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AAR_Player::PrimaryInteract);
	PlayerInputComponent->BindAction("SpecialAttack", IE_Pressed, this, &AAR_Player::SpecialAttack);
	PlayerInputComponent->BindAction("SpecialAbility", IE_Pressed, this, &AAR_Player::SpecialAbility);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AAR_Player::SprintStart);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AAR_Player::SprintStop);
}

/////////////////////
// UNREAL FUNCTIONS

void AAR_Player::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AttributeComp->OnDeath.AddDynamic(this, &AAR_Player::Death);
	AttributeComp->OnHealthChanged.AddDynamic(this, &AAR_Player::HealthChanged);
}

void AAR_Player::GetActorEyesViewPoint(FVector& OutLocation, FRotator& OutRotation) const
{
	OutLocation = CameraComp->GetComponentLocation();
	OutRotation = CameraComp->GetComponentRotation();
}

/////////////////////
// EVENTS

void AAR_Player::Death(AActor* InstigatingActor, UAR_AttributeComponent* OwningAttribute)
{
	auto* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		DisableInput(PlayerController);
	}
	SetActorEnableCollision(false);
}

void AAR_Player::HealthChanged(AActor* InstigatingActor, UAR_AttributeComponent* OwningAttribute,
                               float NewHealthValue, float DeltaValue, float NewHealthPercentage)
{
	if (!OwningAttribute->IsAlive())
	{
		return;
	}
	GetMesh()->SetScalarParameterValueOnMaterials(HitFlashTime_ParameterName, GetWorld()->TimeSeconds);
}

// CONSOLE COMMANDS

void AAR_Player::AR_HealSelf(float Ammount /* = 100 */)
{
	if (GIsEditor)
		AttributeComp->ApplyHealthChange(this, Ammount);
}

void AAR_Player::AR_ToggleGodMode()
{
	if (GIsEditor)
		AttributeComp->ToogleGodMode();
}

/////////////////////
// GAMEPLAY INTERFACE 
bool AAR_Player::SpendCredits_Implementation(int32 CreditsCost)
{
	AAR_PlayerState* MyPlayerState = Cast<AAR_PlayerState>(GetPlayerState());
	if (ensure(MyPlayerState))
	{
		return MyPlayerState->UpdateCredits(-CreditsCost);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("%s is not subclass of AAR_PlayerState"), GetPlayerState());
		return false;
	}
}

bool AAR_Player::ReceiveCredits_Implementation(int32 CreditsReceived)
{
	AAR_PlayerState* MyPlayerState = Cast<AAR_PlayerState>(GetPlayerState());
	if (ensure(MyPlayerState))
	{
		return MyPlayerState->UpdateCredits(CreditsReceived);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("%s is not subclass of AAR_PlayerState"), GetPlayerState());
		return false;
	}
}

int32 AAR_Player::GetCreditsValue_Implementation()
{
	AAR_PlayerState* MyPlayerState = GetPlayerStateChecked<AAR_PlayerState>();
	return MyPlayerState->GetCredits();
}

/////////////////////
// PLAYER ACTIONS

void AAR_Player::MoveForward(float value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0;
	ControlRot.Roll = 0;

	AddMovementInput(ControlRot.Vector(), value);
}

void AAR_Player::MoveRight(float value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0;
	ControlRot.Roll = 0;

	AddMovementInput(FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y), value);
}

void AAR_Player::PrimaryAttack()
{
	ActionComponent->StartAction(this, "PrimaryAttack");
}

void AAR_Player::PrimaryInteract()
{
	if (InteractionComp)
	{
		InteractionComp->PrimaryInteract();
	}
}

void AAR_Player::SpecialAttack()
{
	ActionComponent->StartAction(this, "BlackHole");
}

void AAR_Player::SpecialAbility()
{
	ActionComponent->StartAction(this, "Dash");
}

void AAR_Player::SprintStart()
{
	ActionComponent->StartAction(this, "Sprint");
}

void AAR_Player::SprintStop()
{
	ActionComponent->StopAction(this, "Sprint");
}
