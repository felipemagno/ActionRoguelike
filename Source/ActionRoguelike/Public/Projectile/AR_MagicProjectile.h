﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectile/AR_BaseProjectile.h"
#include "AR_MagicProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UParticleSystemComponent;
class UAR_BaseActionEffect;

UCLASS(Abstract)
class ACTIONROGUELIKE_API AAR_MagicProjectile : public AAR_BaseProjectile
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AAR_MagicProjectile();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float ProjectileDamage = 20;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UAR_BaseActionEffect> ProjectileEffect;

	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	               int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
