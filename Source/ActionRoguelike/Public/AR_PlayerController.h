// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "AR_PlayerController.generated.h"

class UInputMappingContext;
/**
 * 
 */
UCLASS(Abstract)
class ACTIONROGUELIKE_API AAR_PlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	UInputMappingContext* InputMapping_Character;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	UInputMappingContext* InputMapping_UI;

	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

	UFUNCTION(Client, Reliable)
	void ClientAddMappingContext(UInputMappingContext* InputMappingContext);
	UFUNCTION(Client, Reliable)
	void ClientRemoveMappingContext(UInputMappingContext* InputMappingContext);
};
