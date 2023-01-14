// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactable/AR_Pickup.h"

#include "ActorComponent/AR_AttributeComponent.h"
#include "Components/SphereComponent.h"
#include "Net/UnrealNetwork.h"


// Sets default values
AAR_Pickup::AAR_Pickup()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>("CollisionSphere");
	RootComponent = SphereComponent;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	MeshComponent->SetupAttachment(SphereComponent);

	bIsActive = true;
	InactiveDuration = 5.0f;
	CreditsValue = 1;
	SetReplicates(true);
}

void AAR_Pickup::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AAR_Pickup, bIsActive);
}

bool AAR_Pickup::PickupBehavior(APawn* InstigatorPawn)
{
	return true;
}

// Called when the game starts or when spawned
void AAR_Pickup::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void AAR_Pickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AAR_Pickup::Interact_Implementation(APawn* InstigatorPawn)
{
	IAR_IGameplayInterface::Interact_Implementation(InstigatorPawn);

	if (bIsActive)
	{
		if (PickupBehavior(InstigatorPawn) )
		{
			MulticastPickupEffects();
			if(InactiveDuration > 0)
			{
				GetWorldTimerManager().SetTimer(InactiveTimer, this, &AAR_Pickup::MulticastPickupReset, InactiveDuration);
				bIsActive = false;
			}else
			{
				Destroy();
			}
		}
	}
}

void AAR_Pickup::MulticastPickupEffects_Implementation()
{
	SetActorEnableCollision(false);
	SetActorHiddenInGame(true);
	
}

void AAR_Pickup::MulticastPickupReset_Implementation()
{
	SetActorEnableCollision(true);
	SetActorHiddenInGame(false);
	bIsActive = true;
}

int32 AAR_Pickup::GetCreditsValue_Implementation()
{
	return CreditsValue;
}

bool AAR_Pickup::IsActive_Implementation()
{
	return bIsActive;
}
