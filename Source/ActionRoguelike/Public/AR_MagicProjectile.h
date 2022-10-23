// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AR_MagicProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UParticleSystemComponent;

UCLASS(Abstract)
class ACTIONROGUELIKE_API AAR_MagicProjectile : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AAR_MagicProjectile();

protected:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	USphereComponent* SphereComp;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	UProjectileMovementComponent* MovementComp;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	UParticleSystemComponent* EffectComp;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	UAudioComponent* LoopSoundComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SFX")
	USoundBase* HitSFX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SFX")
	USoundBase* LoopSFX;


	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* PrimitiveComponent, AActor* Actor, UPrimitiveComponent* PrimitiveComponent1,
	               int I, bool bArg, const FHitResult& HitResult);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
