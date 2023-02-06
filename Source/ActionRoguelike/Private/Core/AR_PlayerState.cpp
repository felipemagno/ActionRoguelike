// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/AR_PlayerState.h"

#include "Core/AR_SaveGame.h"
#include "Net/UnrealNetwork.h"


AAR_PlayerState::AAR_PlayerState()
{
	PlayerCredits = 0;

	SetReplicates(true);
}

void AAR_PlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AAR_PlayerState, PlayerCredits);
}

bool AAR_PlayerState::UpdateCredits(int32 DeltaValue)
{
	if (PlayerCredits + DeltaValue >= 0 && DeltaValue != 0)
	{
		ServerUpdateCredits(DeltaValue);
		return true;
	}
	else
	{
		return false;
	}
}

void AAR_PlayerState::ServerUpdateCredits_Implementation(int32 DeltaValue)
{
	PlayerCredits = FMath::Max(PlayerCredits + DeltaValue, 0);
	MulticastOnUpdateCredits(PlayerCredits);
}

int32 AAR_PlayerState::GetCredits()
{
	return PlayerCredits;
}

void AAR_PlayerState::LoadPlayerState_Implementation(UAR_SaveGame* SaveGame)
{
	if(SaveGame)
	{
		PlayerCredits = SaveGame->Credits;
	}
}

void AAR_PlayerState::SavePlayerState_Implementation(UAR_SaveGame* SaveGame)
{
	if(SaveGame)
	{
		SaveGame->Credits = PlayerCredits;
	}
}

void AAR_PlayerState::MulticastOnUpdateCredits_Implementation(int32 NewPlayerCredits)
{
	OnUpdateCredits.Broadcast(NewPlayerCredits);
}
