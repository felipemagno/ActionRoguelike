// Fill out your copyright notice in the Description page of Project Settings.


#include "AR_Player.h"

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
}

/////////////////////
// UNREAL FUNCTIONS

// Called when the game starts or when spawned
void AAR_Player::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AAR_Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

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
// HELPER FUNCTIONS

AAR_BaseProjectile* AAR_Player::SpawnProjectile(TSubclassOf<AAR_BaseProjectile> Projectile)
{
	if (!ensure(Projectile))
	{
		return nullptr;
	}

	const FVector SpawnLocation = GetMesh()->GetSocketLocation(ProjectileSpawnSocket);
	const FRotator SpawnRotation = GetRotationToView(SpawnLocation);

	FTransform SpawnTransform = FTransform(SpawnRotation, SpawnLocation);
	FActorSpawnParameters SpawnParamns;
	SpawnParamns.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParamns.Instigator = this;

	return Cast<AAR_BaseProjectile>(GetWorld()->SpawnActor<AActor>(Projectile, SpawnTransform, SpawnParamns));
}

FRotator AAR_Player::GetRotationToView(const FVector SpawnLocation)
{
	FRotator RotationToView = GetControlRotation();

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
	RotationToView = FRotationMatrix::MakeFromX(EndLocation - SpawnLocation).Rotator();
	return RotationToView;
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

void AAR_Player::ExecutePrimaryAttack()
{
	SpawnProjectile(ProjectileClass);
}

void AAR_Player::PrimaryAttack()
{
	PlayAnimMontage(AttackAnimation);

	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &AAR_Player::ExecutePrimaryAttack, 0.17f);
}

void AAR_Player::PrimaryInteract()
{
	if (InteractionComp)
	{
		InteractionComp->PrimaryInteract();
	}
}

void AAR_Player::ExecuteSpecialAttack()
{
	SpawnProjectile(SpecialAttackProjectileClass);
}

void AAR_Player::SpecialAttack()
{
	PlayAnimMontage(AttackAnimation);

	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &AAR_Player::ExecuteSpecialAttack, 0.17f);
}

void AAR_Player::ExecuteSpecialAbility()
{
	SpawnProjectile(SpecialAbilityProjectileClass);
}

void AAR_Player::SpecialAbility()
{
	PlayAnimMontage(AttackAnimation);

	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &AAR_Player::ExecuteSpecialAbility, 0.17f);
}
