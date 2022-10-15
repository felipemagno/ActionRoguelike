// Fill out your copyright notice in the Description page of Project Settings.


#include "AR_Character.h"

#include "AR_InteractionComponent.h"
#include "AR_MagicProjectile.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
/////////////////////
// SETUP FUNCTIONS

// Sets default values
AAR_Character::AAR_Character()
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
}

// Called to bind functionality to input
void AAR_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AAR_Character::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AAR_Character::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("PrimaryAttack", IE_Pressed, this, &AAR_Character::PrimaryAttack);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AAR_Character::Jump);
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AAR_Character::PrimaryInteract);
}

/////////////////////
// UNREAL FUNCTIONS

// Called when the game starts or when spawned
void AAR_Character::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AAR_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

/////////////////////
// PLAYER FUNCTIONS

void AAR_Character::MoveForward(float value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0;
	ControlRot.Roll = 0;

	AddMovementInput(ControlRot.Vector(), value);
}

void AAR_Character::MoveRight(float value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0;
	ControlRot.Roll = 0;

	AddMovementInput(FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y), value);
}

void AAR_Character::PrimaryAttack()
{
	PlayAnimMontage(AttackAnimation);

	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &AAR_Character::ExecutePrimaryAttack, 0.17f);
}

void AAR_Character::ExecutePrimaryAttack()
{
	FVector const SpawnLocation = GetMesh()->GetSocketLocation("Muzzle_01");

	FTransform SpawnTransform = FTransform(GetControlRotation(), SpawnLocation);
	FActorSpawnParameters SpawnParamns;
	SpawnParamns.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParamns.Owner = this;

	GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTransform, SpawnParamns);
}

void AAR_Character::PrimaryInteract()
{
	if (InteractionComp)
		InteractionComp->PrimaryInteract();
}
