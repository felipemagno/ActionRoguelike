// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactable/AR_ItemChest.h"

#include "Interfaces/AR_IGameplayInterface.h"
#include "Components/StaticMeshComponent.h"

void AAR_ItemChest::Interact_Implementation(APawn* InstigatorPawn)
{
	IAR_IGameplayInterface::Interact_Implementation(InstigatorPawn);
	LidMesh->SetRelativeRotation(FRotator(110, 0, 0));
}

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
