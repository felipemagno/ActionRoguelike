// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable/AR_Pickup.h"
#include "AR_CoinPickup.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API AAR_CoinPickup : public AAR_Pickup
{
	GENERATED_BODY()
public:
	AAR_CoinPickup();

protected:

	virtual bool PickupBehavior(APawn* InstigatorPawn) override;
};
