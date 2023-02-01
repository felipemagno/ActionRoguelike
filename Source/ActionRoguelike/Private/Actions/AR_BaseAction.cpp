// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions/AR_BaseAction.h"

#include "ActionRoguelike/ActionRoguelike.h"
#include "ActorComponent/AR_ActionComponent.h"
#include "Net/UnrealNetwork.h"

UAR_BaseAction::UAR_BaseAction()
{
	AutoStart = false;
}

void UAR_BaseAction::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UAR_BaseAction, RepData);
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
	//LogOnScreen(this, FString::Printf(TEXT("StartAction: %s"), *GetNameSafe(this)), FColor::Green);

	UAR_ActionComponent* ActionComponent = GetOwningComponent();
	ActionComponent->ActiveGameplayTags.AppendTags(GrantedTags);

	RepData.bIsRunning = true;
	RepData.Instigator = Instigator;
}

void UAR_BaseAction::StopAction_Implementation(AActor* Instigator)
{
	 UE_LOG(LogTemp, Log, TEXT("StopAction: %s"), *GetNameSafe(this));
	//LogOnScreen(this, FString::Printf(TEXT("StopAction: %s"), *GetNameSafe(this)), FColor::White);

	//ensureAlways(bIsRunning);

	UAR_ActionComponent* ActionComponent = GetOwningComponent();
	ActionComponent->ActiveGameplayTags.RemoveTags(GrantedTags);

	RepData.bIsRunning = false;
	RepData.Instigator = Instigator;
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

void UAR_BaseAction::OnRep_RepData()
{
	LogOnScreen(
		this, FString::Printf(
			TEXT("OnRep_RepData: IsRunning %s; Instigator %s"), RepData.bIsRunning ? TEXT("TRUE") : TEXT("FALSE"),
			*GetNameSafe(RepData.Instigator)), FColor::Red);
	if (RepData.bIsRunning)
	{
		StartAction(RepData.Instigator);
	}
	else
	{
		StopAction(RepData.Instigator);
	}
}

bool UAR_BaseAction::IsRunning() const
{
	return RepData.bIsRunning;
}
