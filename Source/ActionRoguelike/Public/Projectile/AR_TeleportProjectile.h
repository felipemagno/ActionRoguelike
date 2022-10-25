// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AR_MagicProjectile.h"
#include "AR_TeleportProjectile.generated.h"

class UParticleSystem;

UCLASS(Abstract)
class ACTIONROGUELIKE_API AAR_TeleportProjectile : public AAR_BaseProjectile
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AAR_TeleportProjectile();

protected:
	UPROPERTY(EditAnywhere)
	FTimerHandle TimerHandle_ExplodeParticle;

	UPROPERTY(EditAnywhere)
	FTimerHandle TimerHandle_TeleportParticle;


	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ProjectileLifetime = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float TeleportDelay = 0.2f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float TeleportHitOffset = 80.0f;


	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

	virtual void Explode() override;
	
	void OnHit(UPrimitiveComponent* PrimitiveComponent, AActor* Actor, UPrimitiveComponent* PrimitiveComponent1,
	           FVector Vector, const FHitResult& HitResult);

	void TeleportInstigator();
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
