// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/AR_GameMode.h"

#include "AR_Player.h"
#include "EnvironmentQuery/EnvQueryManager.h"

static TAutoConsoleVariable<bool> CVarSpawnBots(TEXT("ar_Spawnbots"), true,TEXT("Enable spawning of bots via timer."),
                                                ECVF_Cheat);

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
	if (!CVarSpawnBots.GetValueOnGameThread())
	{
		UE_LOG(LogTemp,Log,TEXT("Spawn bot disabled by ar.Spawnbots"))
		return;
	}

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

void AAR_GameMode::RespawnPlayerElapsed(AController* Controller)
{
	if (ensure(Controller))
	{
		Controller->UnPossess();

		RestartPlayer(Controller);
	}
}

void AAR_GameMode::OnActorKilled(AActor* VictimActor, AActor* KillerActor)
{
	auto* Player = Cast<AAR_Player>(VictimActor);
	if (Player)
	{
		FTimerHandle TimerHandle_RespawnDelay;
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "RespawnPlayerElapsed", Player->GetController());

		GetWorldTimerManager().SetTimer(TimerHandle_RespawnDelay, Delegate, 2.0f, false);
	}

	UE_LOG(LogTemp, Log, TEXT("OnActorKilled: Victim: %s, Killer: %s"), *GetNameSafe(VictimActor),
	       *GetNameSafe(KillerActor));
}
