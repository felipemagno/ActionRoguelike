// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/AR_PlayerState.h"

AAR_PlayerState::AAR_PlayerState()
{
	PlayerCredits = 0;
}

bool AAR_PlayerState::UpdateCredits(int32 DeltaValue)
{
	if (PlayerCredits + DeltaValue >= 0 && DeltaValue != 0)
	{
		PlayerCredits = FMath::Max(PlayerCredits + DeltaValue, 0);
		OnUpdateCredits.Broadcast(PlayerCredits);
		return true;
	}
	else
	{
		return false;
	}
}

int32 AAR_PlayerState::GetCredits()
{
	return PlayerCredits;
}
