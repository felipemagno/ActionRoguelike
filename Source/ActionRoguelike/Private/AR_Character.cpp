// Fill out your copyright notice in the Description page of Project Settings.


#include "AR_Character.h"

#include "ActorComponent/AR_InteractionComponent.h"
#include "Projectile/AR_MagicProjectile.h"
#include "Projects.h"
#include "ActorComponent/AR_AttributeComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
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
	AttributeComp = CreateDefaultSubobject<UAR_AttributeComponent>("AttributeComponent");
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

void AAR_Character::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AttributeComp->OnDeath.AddDynamic(this, &AAR_Character::Death);
	AttributeComp->OnHealthChanged.AddDynamic(this, &AAR_Character::HealthChanged);
}

void AAR_Character::GetActorEyesViewPoint(FVector& OutLocation, FRotator& OutRotation) const
{
	OutLocation = CameraComp->GetComponentLocation();
	OutRotation = CameraComp->GetComponentRotation();
}

/////////////////////
// EVENTS

void AAR_Character::Death(AActor* InstigatingActor, UAR_AttributeComponent* OwningAttribute)
{
	auto* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		DisableInput(PlayerController);
	}
	SetActorEnableCollision(false);
}

void AAR_Character::HealthChanged(AActor* InstigatingActor, UAR_AttributeComponent* OwningAttribute,
								  float NewHealthValue, float DeltaValue, float NewHealthPercentage)
{
	if (!OwningAttribute->IsAlive())
	{
		return;
	}
	GetMesh()->SetScalarParameterValueOnMaterials("HitTime", GetWorld()->TimeSeconds);
}

/////////////////////
// HELPER FUNCTIONS
AAR_BaseProjectile* AAR_Character::SpawnProjectile(TSubclassOf<AAR_BaseProjectile> Projectile)
{
	if (!ensure(Projectile))
	{
		return nullptr;
	}

	const FVector SpawnLocation = GetMesh()->GetSocketLocation("Muzzle_01");

	FRotator SpawnRotation = GetControlRotation();

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(GetOwner());

	FVector StartLocation = CameraComp->GetComponentLocation();
	FVector EndLocation = StartLocation + CameraComp->GetForwardVector() * 1000;

	FHitResult Hit;
	bool bBlockingHit = GetWorld()->LineTraceSingleByChannel(Hit, StartLocation, EndLocation,
															 ECC_Visibility,
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

	return Cast<AAR_BaseProjectile>(GetWorld()->SpawnActor<AActor>(Projectile, SpawnTransform, SpawnParamns));
}


/////////////////////
// PLAYER ACTIONS

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

void AAR_Character::ExecutePrimaryAttack()
{
	SpawnProjectile(ProjectileClass);
}

void AAR_Character::PrimaryAttack()
{
	PlayAnimMontage(AttackAnimation);

	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &AAR_Character::ExecutePrimaryAttack, 0.17f);
}

void AAR_Character::PrimaryInteract()
{
	if (InteractionComp)
	{
		InteractionComp->PrimaryInteract();
	}
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
