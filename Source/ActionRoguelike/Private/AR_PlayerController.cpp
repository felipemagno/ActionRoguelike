// Fill out your copyright notice in the Description page of Project Settings.


#include "AR_PlayerController.h"
#include "EnhancedInput/Public/InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInput/Public/EnhancedInputComponent.h"

#include "EnhancedInputComponent.h"

void AAR_PlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(Player))
	{
		if (UEnhancedInputLocalPlayerSubsystem* InputSystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (InputMapping_Character)
			{
				InputSystem->AddMappingContext(InputMapping_Character, 0);
			}
		}
	}
}

void AAR_PlayerController::OnUnPossess()
{
	Super::OnUnPossess();

	if (ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(Player))
	{
		if (UEnhancedInputLocalPlayerSubsystem* InputSystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (InputMapping_Character)
			{
				InputSystem->RemoveMappingContext(InputMapping_Character);
			}
		}
	}
}
