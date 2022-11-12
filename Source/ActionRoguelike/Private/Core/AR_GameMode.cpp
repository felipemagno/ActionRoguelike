// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/AR_GameMode.h"

#include "EnvironmentQuery/EnvQueryManager.h"


AAR_GameMode::AAR_GameMode()
{
	SpawnTimerInterval = 2.0f;
	AiHalfHeight = 44.0f;
}

void AAR_GameMode::StartPlay()
{
	Super::StartPlay();

	GetWorldTimerManager().SetTimer(TimerHandle_SpawnAI, this, &AAR_GameMode::SpawnBotTimerElapsed, SpawnTimerInterval,
	                                true);
}

void AAR_GameMode::SpawnBotTimerElapsed()
{
	UEnvQueryInstanceBlueprintWrapper* EQSWrapper = UEnvQueryManager::RunEQSQuery(
		this, SpawnBotQuery, this, EEnvQueryRunMode::RandomBest5Pct, nullptr);

	if (ensure(EQSWrapper))
		EQSWrapper->GetOnQueryFinishedEvent().AddDynamic(this, &AAR_GameMode::OnSpawnEQSFinished);
}

void AAR_GameMode::OnSpawnEQSFinished(UEnvQueryInstanceBlueprintWrapper* QueryInstance,
                                      EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("GameMode: Spawn bot EQS Failed!"))
		return;
	}

	if (DifficultyCurve)
	{
		BotMaxCount = FMath::CeilToInt(DifficultyCurve->GetFloatValue(GetWorld()->TimeSeconds));
	}

	if (BotCount >= BotMaxCount) return;

	TArray<FVector> EQSLocations = QueryInstance->GetResultsAsLocations();

	if (EQSLocations.Num() > 0)
	{
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;


		auto* NewBot = GetWorld()->SpawnActor<AActor>(AiClass, EQSLocations[0] + FVector(0, 0, AiHalfHeight),
		                                              FRotator::ZeroRotator,
		                                              SpawnParameters);

		if (NewBot) BotCount++;
		DrawDebugCapsule(GetWorld(), EQSLocations[0], 200, 50, FQuat::Identity, FColor::Black, false, 15);
	}
}
