// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions/AR_Action_ProjectileAttack.h"

#include "Projectile/AR_BaseProjectile.h"
#include "GameFramework/Character.h"


UAR_Action_ProjectileAttack::UAR_Action_ProjectileAttack()
{
	AttackAnimDelay = 0.17f;
	ProjectileSpawnSocket = "Muzzle_01";
}

void UAR_Action_ProjectileAttack::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);
	ACharacter* InstigatorCharacter = Cast<ACharacter>(Instigator);
	if (InstigatorCharacter)
	{
		InstigatorCharacter->PlayAnimMontage(AttackAnimation);
		if (InstigatorCharacter->HasAuthority())
		{
			FTimerHandle TimerHandle_AttackDelay;
			FTimerDelegate Delegate;
			Delegate.BindUFunction(this, "AttackDelay_Elapsed", InstigatorCharacter);
			GetWorld()->GetTimerManager().SetTimer(TimerHandle_AttackDelay, Delegate, AttackAnimDelay, false);
		}
	}	
}

void UAR_Action_ProjectileAttack::AttackDelay_Elapsed(ACharacter* InstigatorCharacter)
{
	if (!ensure(ProjectileClass) || !ensure(InstigatorCharacter))
	{
		return;
	}

	const FVector SpawnLocation = InstigatorCharacter->GetMesh()->GetSocketLocation(ProjectileSpawnSocket);

	//Dedicated function below this function
	const FRotator SpawnRotation = GetRotationToView(InstigatorCharacter, SpawnLocation); 

	FTransform SpawnTransform = FTransform(SpawnRotation, SpawnLocation);
	
	FActorSpawnParameters SpawnParamns;
	SpawnParamns.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParamns.Instigator = InstigatorCharacter;

	GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTransform, SpawnParamns);
	StopAction(InstigatorCharacter);
}

FRotator UAR_Action_ProjectileAttack::GetRotationToView(ACharacter* InstigatorCharacter,
                                                        const FVector SpawnLocation) const
{
	FRotator RotationToView = InstigatorCharacter->GetControlRotation();

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(InstigatorCharacter);

	FVector StartLocation;

	FRotator EyeRotation;
	InstigatorCharacter->GetActorEyesViewPoint(StartLocation, EyeRotation);

	FVector EndLocation = StartLocation + EyeRotation.Vector() * 1000;

	FHitResult Hit;
	bool bBlockingHit = GetWorld()->LineTraceSingleByChannel(Hit, StartLocation, EndLocation,
	                                                         ECC_Visibility, QueryParams);
	if (bBlockingHit)
	{
		EndLocation = Hit.ImpactPoint;
	}
	RotationToView = FRotationMatrix::MakeFromX(EndLocation - SpawnLocation).Rotator();
	return RotationToView;
}
