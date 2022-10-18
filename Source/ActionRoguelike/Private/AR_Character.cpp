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
	PlayerInputComponent->BindAction("SpecialAttack", IE_Pressed, this, &AAR_Character::SpecialAttack);
	PlayerInputComponent->BindAction("SpecialAbility", IE_Pressed, this, &AAR_Character::SpecialAbility);
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

AAR_MagicProjectile* AAR_Character::SpawnProjectile(TSubclassOf<AAR_MagicProjectile> Projectile)
{
	if (Projectile == nullptr) nullptr;

	FVector const SpawnLocation = GetMesh()->GetSocketLocation("Muzzle_01");

	FRotator SpawnRotation = GetControlRotation();


	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(GetOwner());

	FVector StartLocation = CameraComp->GetComponentLocation();
	FVector EndLocation = StartLocation + CameraComp->GetForwardVector() * 1000;

	FHitResult Hit;
	bool bBlockingHit = GetWorld()->LineTraceSingleByChannel(Hit, StartLocation, EndLocation,
	                                                         ECollisionChannel::ECC_Visibility,
	                                                         QueryParams);
	if (bBlockingHit)
	{
		EndLocation = Hit.ImpactPoint;
	}
	SpawnRotation = FRotationMatrix::MakeFromX(EndLocation - SpawnLocation).Rotator();

	FTransform SpawnTransform = FTransform(SpawnRotation, SpawnLocation);
	FActorSpawnParameters SpawnParamns;
	SpawnParamns.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParamns.Instigator = this;

	return Cast<AAR_MagicProjectile>(GetWorld()->SpawnActor<AActor>(Projectile, SpawnTransform, SpawnParamns));
}

void AAR_Character::ExecutePrimaryAttack()
{
	SpawnProjectile(ProjectileClass);
}

void AAR_Character::PrimaryInteract()
{
	if (InteractionComp)
		InteractionComp->PrimaryInteract();
}

void AAR_Character::ExecuteSpecialAttack()
{
	SpawnProjectile(SpecialAttackProjectileClass);
}

void AAR_Character::SpecialAttack()
{
	PlayAnimMontage(AttackAnimation);

	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &AAR_Character::ExecuteSpecialAttack, 0.17f);
}

void AAR_Character::ExecuteSpecialAbility()
{
	SpawnProjectile(SpecialAbilityProjectileClass);	
}

void AAR_Character::SpecialAbility()
{
	PlayAnimMontage(AttackAnimation);

	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &AAR_Character::ExecuteSpecialAbility, 0.17f);
	
}

