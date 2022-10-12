// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AR_ExplosiveBarrel.generated.h"


class UStaticMeshComponent;
class URadialForceComponent;


UCLASS()
class ACTIONROGUELIKE_API AAR_ExplosiveBarrel : public AActor
{
	GENERATED_BODY()

public:		
	// Sets default values for this actor's properties
	AAR_ExplosiveBarrel();

protected:
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	UStaticMeshComponent* StaticMeshComp;	

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	URadialForceComponent* RadialForceComp;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* PrimitiveComponent, AActor* Actor, UPrimitiveComponent* PrimitiveComponent1, FVector Vector, const FHitResult& HitResult);
public:
};
