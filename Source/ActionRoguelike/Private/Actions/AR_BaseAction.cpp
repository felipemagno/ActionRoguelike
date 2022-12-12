// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions/AR_BaseAction.h"

void UAR_BaseAction::StartAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("StartAction: %s"), *GetNameSafe(this));
	
}

void UAR_BaseAction::StopAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("StopAction: %s"), *GetNameSafe(this));
}

UWorld* UAR_BaseAction::GetWorld() const
{
	// Outer is set when creating action with NewObject<T>
	UActorComponent* comp = Cast<UActorComponent>(GetOuter());

	if (comp)
	{
		return comp->GetWorld();
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Failed to GetWorld(), no comp from GetOuter()"))
		return nullptr;
	}
}
