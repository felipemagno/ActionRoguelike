// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AR_PlayerState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUpdateCredits, int32, CreditsNewValue);

/**
 * 
 */
UCLASS(Abstract)
class ACTIONROGUELIKE_API AAR_PlayerState : public APlayerState
{
	GENERATED_BODY()

protected:
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly, Category = "Player")
	int32 PlayerCredits;

	UFUNCTION(Server, Reliable)
	void ServerUpdateCredits(int32 DeltaValue);

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastOnUpdateCredits(int32 NewPlayerCredits);


public:
	AAR_PlayerState();

	UPROPERTY(BlueprintAssignable)
	FOnUpdateCredits OnUpdateCredits;

	UFUNCTION(BlueprintCallable)
	bool UpdateCredits(int32 DeltaValue);

	UFUNCTION(BlueprintGetter)
	int32 GetCredits();

};
