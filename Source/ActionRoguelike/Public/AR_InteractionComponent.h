﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AR_InteractionComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ACTIONROGUELIKE_API UAR_InteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	void PrimaryInteract();
	
	// Sets default values for this component's properties
	UAR_InteractionComponent();

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool bIsDebugging = false;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
