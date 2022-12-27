// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions/AR_BaseActionEffect.h"

#include "ActorComponent/AR_ActionComponent.h"

UAR_BaseActionEffect::UAR_BaseActionEffect()
{
	AutoStart = true;
}

void UAR_BaseActionEffect::ExecutePeriodicEffect_Implementation(AActor* Instigator)
{
}


void UAR_BaseActionEffect::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);
	if (Duration > 0.0f)
	{
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "StopAction", Instigator);

		GetWorld()->GetTimerManager().SetTimer(DurationTimer, Delegate, Duration, false);
	}

	if (Period > 0.0f)
	{		
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "ExecutePeriodicEffect", Instigator);

		GetWorld()->GetTimerManager().SetTimer(PeriodTimer, Delegate, Period, true);
	}
}

void UAR_BaseActionEffect::StopAction_Implementation(AActor* Instigator)
{
	if(GetWorld()->GetTimerManager().GetTimerRemaining(PeriodTimer) < KINDA_SMALL_NUMBER )
	{
		ExecutePeriodicEffect(Instigator);
	}
	
	Super::StopAction_Implementation(Instigator);

	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);

	UAR_ActionComponent* ActionComp = GetOwningComponent();
	if(ActionComp)
	{
		ActionComp->RemoveAction(this);
	}
}
