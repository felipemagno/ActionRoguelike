// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/AR_ActionObject.h"

void UAR_ActionObject::StartAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp,Log,TEXT("StartAction: %s"),*GetNameSafe(this));
}

void UAR_ActionObject::StopAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp,Log,TEXT("StopAction: %s"),*GetNameSafe(this));
}
