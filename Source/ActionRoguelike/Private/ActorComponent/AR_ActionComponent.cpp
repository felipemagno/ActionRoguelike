// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponent/AR_ActionComponent.h"

#include "Object/AR_ActionObject.h"

// Sets default values for this component's properties
UAR_ActionComponent::UAR_ActionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UAR_ActionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}


// Called every frame
void UAR_ActionComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                        FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UAR_ActionComponent::AddAction(TSubclassOf<UAR_ActionObject> NewAction)
{
	if (!ensure(NewAction)) { return; }

	UAR_ActionObject* ActionObject = NewObject<UAR_ActionObject>(this, NewAction);
	if (ensure(ActionObject))
	{
		Actions.Add(ActionObject);
	}
}

bool UAR_ActionComponent::StartAction(AActor* Instigator, FName ActionName)
{
	for (UAR_ActionObject* Action : Actions)
	{
		if (Action && Action->ActionName == ActionName)
		{
			Action->StartAction(GetOwner());
			return true;
		}
	}
	return false;
}

bool UAR_ActionComponent::StopAction(AActor* Instigator, FName ActionName)
{
	for (UAR_ActionObject* Action : Actions)
	{
		if (Action && Action->ActionName == ActionName)
		{
			Action->StopAction(GetOwner());
			return true;
		}
	}
	return false;
}
