// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponent/AR_ActionComponent.h"

#include "ActionRoguelike/ActionRoguelike.h"
#include "Actions/AR_BaseAction.h"
#include "Engine/ActorChannel.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UAR_ActionComponent::UAR_ActionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	CurrentAction = FGameplayTag::EmptyTag;
	SetIsReplicatedByDefault(true);
}

void UAR_ActionComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UAR_ActionComponent, Actions);
}

bool UAR_ActionComponent::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);
	for (UAR_BaseAction* Action : Actions)
	{
		if (Action)
		{
			WroteSomething |= Channel->ReplicateSubobject(Action, *Bunch, *RepFlags);
		}
	}
	return WroteSomething;
}

// Called when the game starts
void UAR_ActionComponent::BeginPlay()
{
	Super::BeginPlay();

	// Server Only
	if (GetOwner()->HasAuthority())
	{
		for (TSubclassOf<UAR_BaseAction> ActionClass : DefaultActions)
		{
			AddAction(GetOwner(), ActionClass);
		}
	}
}



// Called every frame
void UAR_ActionComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                        FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	// FString DebugText = GetNameSafe(GetOwner()) + " : " + ActiveGameplayTags.ToStringSimple();
	// GEngine->AddOnScreenDebugMessage(-1, 0, FColor::White, DebugText);

	for (UAR_BaseAction* Action : Actions)
	{
		FColor TextColor = Action->IsRunning() ? FColor::Blue : FColor::White;

		FString ActionMsg = FString::Printf(TEXT("[%s] Action: %s"), *GetNameSafe(GetOwner()), *Action->GetName());
		LogOnScreen(this, ActionMsg, TextColor, 0.0f);
	}
}

FGameplayTag UAR_ActionComponent::GetCurrentAction()
{
	return CurrentAction;
}

void UAR_ActionComponent::AddAction(AActor* Instigator, TSubclassOf<UAR_BaseAction> NewAction)
{
	if (!ensure(NewAction)) { return; }

	UAR_BaseAction* ActionObject = NewObject<UAR_BaseAction>(this, NewAction);
	if (ensure(ActionObject))
	{
		if (!GetOwner()->HasAuthority())
		{
			ServerAddAction(Instigator, NewAction);
		}

		Actions.Add(ActionObject);
		if (ActionObject->AutoStart && ensure(ActionObject->CanStartAction(Instigator)))
		{
			ActionObject->StartAction(Instigator);
		}
	}
}

void UAR_ActionComponent::RemoveAction(UAR_BaseAction* ActionToRemove)
{
	if (ensure(ActionToRemove && !ActionToRemove->IsRunning()))
	{
		return;
	}
	
	if (!GetOwner()->HasAuthority())
	{
		ServerRemoveAction(ActionToRemove);
	}
	
	Actions.Remove(ActionToRemove);
}

bool UAR_ActionComponent::StartAction(AActor* Instigator, FGameplayTag ActionTag)
{
	for (UAR_BaseAction* Action : Actions)
	{
		if (Action && Action->ActionTag == ActionTag)
		{
			if (!Action->CanStartAction(Instigator))
			{
				FString FailedMsg = FString::Printf(TEXT("Failed to run: %s"), *ActionTag.ToString());
				GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, FailedMsg);
				continue;
			}

			if (!GetOwner()->HasAuthority())
			{
				ServerStartAction(Instigator, ActionTag);
			}

			Action->StartAction(GetOwner());
			CurrentAction = Action->ActionTag;
			return true;
		}
	}
	return false;
}

bool UAR_ActionComponent::StopAction(AActor* Instigator, FGameplayTag ActionTag)
{
	for (UAR_BaseAction* Action : Actions)
	{
		if (Action && Action->ActionTag == ActionTag && Action->IsRunning())
		{
			if (!GetOwner()->HasAuthority())
			{
				ServerStopAction(Instigator, ActionTag);
			}
			
			Action->StopAction(GetOwner());
			CurrentAction = FGameplayTag::EmptyTag;
			return true;
		}
	}
	return false;
}

void UAR_ActionComponent::ServerStartAction_Implementation(AActor* Instigator, FGameplayTag ActionTag)
{
	StartAction(Instigator, ActionTag);
}

void UAR_ActionComponent::ServerStopAction_Implementation(AActor* Instigator, FGameplayTag ActionTag)
{
	StopAction(Instigator,ActionTag);
}

void UAR_ActionComponent::ServerAddAction_Implementation(AActor* Instigator, TSubclassOf<UAR_BaseAction> NewAction)
{
	AddAction(Instigator, NewAction);
}

void UAR_ActionComponent::ServerRemoveAction_Implementation(UAR_BaseAction* ActionToRemove)
{
	RemoveAction(ActionToRemove);
}