// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AR_BaseProjectile.h"
#include "GameFramework/Actor.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "AR_BlackHoleProjectile.generated.h"

UCLASS()
class ACTIONROGUELIKE_API AAR_BlackHoleProjectile : public AAR_BaseProjectile
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AAR_BlackHoleProjectile();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	URadialForceComponent* RadialForceComponent;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                       int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
