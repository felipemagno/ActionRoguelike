﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/AR_IGameplayInterface.h"
#include "AR_ItemChest.generated.h"

class UInstancedStaticMeshComponent;

UCLASS()
class ACTIONROGUELIKE_API AAR_ItemChest : public AActor, public IAR_IGameplayInterface
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	float TargetPitch;

	void Interact_Implementation(APawn* InstigatorPawn) override;

	// Sets default values for this actor's properties
	AAR_ItemChest();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* LidMesh;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
