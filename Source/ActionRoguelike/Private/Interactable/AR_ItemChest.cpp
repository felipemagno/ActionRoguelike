// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactable/AR_ItemChest.h"

#include "Interfaces/AR_IGameplayInterface.h"
#include "Components/StaticMeshComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AAR_ItemChest::AAR_ItemChest()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("BaseMesh");
	RootComponent = BaseMesh;

	LidMesh = CreateDefaultSubobject<UStaticMeshComponent>("LidMesh");
	LidMesh->SetupAttachment(BaseMesh);

	TargetPitch = 110.0f;
	
	SetReplicates(true);
}

// Called when the game starts or when spawned
void AAR_ItemChest::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AAR_ItemChest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AAR_ItemChest::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AAR_ItemChest,bLidOpened);
}


void AAR_ItemChest::Interact_Implementation(APawn* InstigatorPawn)
{
	IAR_IGameplayInterface::Interact_Implementation(InstigatorPawn);
	// LidMesh->SetRelativeRotation(FRotator(110, 0, 0));
	bLidOpened = !bLidOpened;
	OnRep_LidOpened();
	// Even though the OnRep function is called automatically because the variable is changed,
	// it is only called for Clients and not for the Server, that is why it is being called here directly. 
}

void AAR_ItemChest::OnRep_LidOpened_Implementation()
{
}
