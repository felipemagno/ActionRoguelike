// Fill out your copyright notice in the Description page of Project Settings.


#include "AR_TargetDummy.h"

#include "ActorComponent/AR_AttributeComponent.h"


void AAR_TargetDummy::OnHitEvent(AActor* InstigatingActor, UAR_AttributeComponent* OwningAttribute,
                                 float NewHealthValue, float DeltaValue, float NewHealthPercentage)
{
	if (DeltaValue < 0)
		MeshComp->SetScalarParameterValueOnMaterials("TimeToHit", GetWorld()->TimeSeconds);
}

// Sets default values
AAR_TargetDummy::AAR_TargetDummy()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComponent");
	RootComponent = MeshComp;

	AttributeComponent = CreateDefaultSubobject<UAR_AttributeComponent>("AttributeComponent");
	AttributeComponent->OnHealthChanged.AddDynamic(this, &AAR_TargetDummy::OnHitEvent);
}

// Called when the game starts or when spawned
void AAR_TargetDummy::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AAR_TargetDummy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
