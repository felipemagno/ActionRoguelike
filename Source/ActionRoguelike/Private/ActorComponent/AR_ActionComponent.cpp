// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponent/AR_ActionComponent.h"

#include "Actions/AR_BaseAction.h"

// Sets default values for this component's properties
UAR_ActionComponent::UAR_ActionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	CurrentAction = "None";
}

// Called when the game starts
void UAR_ActionComponent::BeginPlay()
{
	Super::BeginPlay();

	for (TSubclassOf<UAR_BaseAction> ActionClass : DefaultActions)
	{
		AddAction(ActionClass);
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

FName UAR_ActionComponent::GetCurrentAction()
{
	return CurrentAction;
}

void UAR_ActionComponent::AddAction(TSubclassOf<UAR_BaseAction> NewAction)
{
	if (!ensure(NewAction)) { return; }

	UAR_BaseAction* ActionObject = NewObject<UAR_BaseAction>(this, NewAction);
	if (ensure(ActionObject))
	{
		Actions.Add(ActionObject);
	}
}

bool UAR_ActionComponent::StartAction(AActor* Instigator, FName ActionName)
{
	for (UAR_BaseAction* Action : Actions)
	{
		if (Action && Action->ActionName == ActionName)
		{
			if (!Action->CanStartAction(Instigator))
			{
				FString FailedMsg = FString::Printf(TEXT("Failed to run: %s"), *ActionName.ToString());
				GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, FailedMsg);
				continue;
			}

			Action->StartAction(GetOwner());
			CurrentAction = Action->ActionName;
			return true;
		}
	}
	return false;
}

bool UAR_ActionComponent::StopAction(AActor* Instigator, FName ActionName)
{
	for (UAR_BaseAction* Action : Actions)
	{
		if (Action && Action->ActionName == ActionName && Action->IsRunning())
		{
			Action->StopAction(GetOwner());
			CurrentAction = "None";
			return true;
		}
	}
	return false;
}
