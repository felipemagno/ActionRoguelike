// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Actor.h"
#include "AR_BaseProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UParticleSystemComponent;

UCLASS(Abstract)
class ACTIONROGUELIKE_API AAR_BaseProjectile : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AAR_BaseProjectile();

	// Gameplay Tag associated with the Parry mechanic
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Projectile")
	FGameplayTag ParryTag;

protected:
	// COMPONENTS
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	USphereComponent* SphereComp;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	UProjectileMovementComponent* MovementComp;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	UParticleSystemComponent* EffectComp;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	UAudioComponent* LoopSoundComponent;

	// VARIABLES
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "VFX")
	UParticleSystem* HitEffect;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "VFX")
	UParticleSystem* MuzzleEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VFX")
	TSubclassOf<UCameraShakeBase> Shake;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VFX")
	float ShakeInnerRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VFX")
	float ShakeOuterRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SFX")
	USoundBase* HitSFX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SFX")
	USoundBase* LoopSFX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	FName MuzzleSockName;

	// FUNCTIONS & EVENTS
	UFUNCTION()
	virtual void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                       int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                   FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	virtual void Explode();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
