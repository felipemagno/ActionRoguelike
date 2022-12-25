// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponent/AR_ActionComponent.h"

#include "Actions/AR_BaseAction.h"

// Sets default values for this component's properties
UAR_ActionComponent::UAR_ActionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	CurrentAction = FGameplayTag::EmptyTag;
}

// Called when the game starts
void UAR_ActionComponent::BeginPlay()
{
	Super::BeginPlay();

	for (TSubclassOf<UAR_BaseAction> ActionClass : DefaultActions)
	{
		AddAction(GetOwner(),ActionClass);
	}
}


// Called every frame
void UAR_ActionComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                        FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	FString DebugText = GetNameSafe(GetOwner()) + " : " + ActiveGameplayTags.ToStringSimple();
	GEngine->AddOnScreenDebugMessage(-1, 0, FColor::White, DebugText);
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
		Actions.Add(ActionObject);
		if(ActionObject->AutoStart && ensure(ActionObject->CanStartAction(Instigator)))
		{
			ActionObject->StartAction(Instigator);
		}
	}
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
			Action->StopAction(GetOwner());
			CurrentAction = FGameplayTag::EmptyTag;
			return true;
		}
	}
	return false;
}

void UAR_ActionComponent::RemoveAction(UAR_BaseAction* ActionToRemove)
{
	if (ensure(ActionToRemove && !ActionToRemove->IsRunning()))
	{
		return;
	}

	Actions.Remove(ActionToRemove);
}
