// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/AR_IGameplayInterface.h"
#include "AR_Pickup.generated.h"

class USphereComponent;

UCLASS(Abstract)
class ACTIONROGUELIKE_API AAR_Pickup : public AActor, public IAR_IGameplayInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AAR_Pickup();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USphereComponent* SphereComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float InactiveDuration;

	UPROPERTY(Replicated,EditAnywhere, BlueprintReadWrite)
	bool bIsActive;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FTimerHandle InactiveTimer;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 CreditsValue;

	virtual bool PickupBehavior(APawn* InstigatorPawn);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(NetMulticast,Unreliable)
	virtual void MulticastPickupEffects();

	UFUNCTION(NetMulticast,Reliable)
	virtual void MulticastPickupReset();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// GAMEPLAY INTERFACE
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;
	virtual int32 GetCreditsValue_Implementation() override;
	virtual bool IsActive_Implementation() override;
};
