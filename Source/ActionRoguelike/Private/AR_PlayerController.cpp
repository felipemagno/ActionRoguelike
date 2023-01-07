// Fill out your copyright notice in the Description page of Project Settings.


#include "AR_PlayerController.h"
#include "EnhancedInput/Public/InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInput/Public/EnhancedInputComponent.h"

#include "EnhancedInputComponent.h"

static TAutoConsoleVariable<bool> CVarEnableDebugDraw(
	TEXT("ar_Debug.EnableAllDraw"), false,TEXT("Enable all Draw Debugs"),
	ECVF_Cheat);

void AAR_PlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	ClientAddMappingContext(InputMapping_Character);
}

void AAR_PlayerController::OnUnPossess()
{
	Super::OnUnPossess();

	ClientRemoveMappingContext(InputMapping_Character);
}

void AAR_PlayerController::ClientAddMappingContext_Implementation(UInputMappingContext* InputMappingContext)
{
	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* InputSystem = LocalPlayer->GetSubsystem<
			UEnhancedInputLocalPlayerSubsystem>())
		{
			if (InputMappingContext)
			{
				InputSystem->AddMappingContext(InputMappingContext, 0);
			}
		}
	}
}

void AAR_PlayerController::ClientRemoveMappingContext_Implementation(UInputMappingContext* InputMappingContext)
{
	if (ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(Player))
	{
		if (UEnhancedInputLocalPlayerSubsystem* InputSystem = LocalPlayer->GetSubsystem<
			UEnhancedInputLocalPlayerSubsystem>())
		{
			if (InputMappingContext)
			{
				InputSystem->RemoveMappingContext(InputMappingContext);
			}
		}
	}
}
