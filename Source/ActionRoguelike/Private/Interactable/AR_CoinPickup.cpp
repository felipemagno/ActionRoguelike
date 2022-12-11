// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactable/AR_CoinPickup.h"

#include "AR_Player.h"

AAR_CoinPickup::AAR_CoinPickup()
{
	CreditsValue = 1;
}

bool AAR_CoinPickup::PickupBehavior(APawn* InstigatorPawn)
{
	
	if (InstigatorPawn->Implements<UAR_IGameplayInterface>() && InstigatorPawn->IsA(AAR_Player::StaticClass()))
	{
		IAR_IGameplayInterface::Execute_ReceiveCredits(InstigatorPawn, CreditsValue);
		Super::PickupBehavior(InstigatorPawn);
		return true;
	}
	return false;
}
