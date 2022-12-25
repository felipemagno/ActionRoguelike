// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions/AR_BaseAction.h"

#include "ActorComponent/AR_ActionComponent.h"

UAR_BaseAction::UAR_BaseAction()
{
	AutoStart = false;
}

bool UAR_BaseAction::CanStartAction_Implementation(AActor* Instigator)
{
	UAR_ActionComponent* ActionComponent = GetOwningComponent();

	if (ActionComponent->ActiveGameplayTags.HasAny(BlockedTags) || IsRunning())
	{
		return false;
	}

	return true;
}

void UAR_BaseAction::StartAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("StartAction: %s"), *GetNameSafe(this));

	UAR_ActionComponent* ActionComponent = GetOwningComponent();
	ActionComponent->ActiveGameplayTags.AppendTags(GrantedTags);

	bIsRunning = true;
}

void UAR_BaseAction::StopAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("StopAction: %s"), *GetNameSafe(this));

	ensureAlways(bIsRunning);

	UAR_ActionComponent* ActionComponent = GetOwningComponent();
	ActionComponent->ActiveGameplayTags.RemoveTags(GrantedTags);

	bIsRunning = false;
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

UAR_ActionComponent* UAR_BaseAction::GetOwningComponent() const
{
	return Cast<UAR_ActionComponent>(GetOuter());
}

bool UAR_BaseAction::IsRunning() const
{
	return bIsRunning;
}
