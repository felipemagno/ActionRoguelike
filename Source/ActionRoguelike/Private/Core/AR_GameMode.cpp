// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/AR_GameMode.h"

#include "AR_Player.h"
#include "Core/AR_PlayerState.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "Interfaces/AR_IGameplayInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Core/AR_SaveGame.h"
#include "GameFramework/GameStateBase.h"


static TAutoConsoleVariable<bool> CVarSpawnBots(TEXT("ar_Spawnbots"), true,TEXT("Enable spawning of bots via timer."),
                                                ECVF_Cheat);

AAR_GameMode::AAR_GameMode()
{
	SpawnTimerInterval = 2.0f;
	AiHalfHeight = 44.0f;
	SlotName = "SaveGame01";
}

void AAR_GameMode::StartPlay()
{
	Super::StartPlay();

	GetWorldTimerManager().SetTimer(TimerHandle_SpawnAI, this, &AAR_GameMode::SpawnBotTimerElapsed, SpawnTimerInterval,
	                                true);
}

void AAR_GameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
	LoadSaveGame();
	UE_LOG(LogTemp,Log,TEXT("InitGame start."))
}

void AAR_GameMode::SpawnBotTimerElapsed()
{
	if (!CVarSpawnBots.GetValueOnGameThread())
	{
		UE_LOG(LogTemp, Log, TEXT("Spawn bot disabled by ar.Spawnbots"))
		return;
	}

	UEnvQueryInstanceBlueprintWrapper* EQSWrapper = UEnvQueryManager::RunEQSQuery(
		this, SpawnBotQuery, this, EEnvQueryRunMode::RandomBest5Pct, nullptr);

	if (ensure(EQSWrapper))
		EQSWrapper->GetOnQueryFinishedEvent().AddDynamic(this, &AAR_GameMode::SpawnEQSFinishedEvent);
}

void AAR_GameMode::SpawnEQSFinishedEvent(UEnvQueryInstanceBlueprintWrapper* QueryInstance,
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

void AAR_GameMode::ActorKilledEvent(AActor* VictimActor, AActor* KillerActor)
{
	auto* Player = Cast<AAR_Player>(VictimActor);
	if (Player)
	{
		FTimerHandle TimerHandle_RespawnDelay;
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "RespawnPlayerElapsed", Player->GetController());

		GetWorldTimerManager().SetTimer(TimerHandle_RespawnDelay, Delegate, 2.0f, false);
	}

	// Get Credits value from enemy and give it to player
	if (KillerActor->Implements<UAR_IGameplayInterface>() && VictimActor->Implements<UAR_IGameplayInterface>())
	{
		const int32 Credits = IAR_IGameplayInterface::Execute_GetCreditsValue(VictimActor);
		IAR_IGameplayInterface::Execute_ReceiveCredits(KillerActor, Credits);
	}

	OnActorKilled.Broadcast(VictimActor, KillerActor);


	UE_LOG(LogTemp, Log, TEXT("OnActorKilled: Victim: %s, Killer: %s"), *GetNameSafe(VictimActor),
	       *GetNameSafe(KillerActor));
}

void AAR_GameMode::WriteSaveGame()
{
	for(int32 i = 0; i < GameState->PlayerArray.Num(); i++)
	{
		AAR_PlayerState* PlayerState = Cast<AAR_PlayerState>(GameState->PlayerArray[i]);
		if (PlayerState)
		{
			PlayerState->SavePlayerState(CurrentSaveGame);
			break;// only single player right now
		}
	}
	
	UGameplayStatics::SaveGameToSlot(CurrentSaveGame,SlotName,0);
}

void AAR_GameMode::LoadSaveGame()
{
	if (UGameplayStatics::DoesSaveGameExist(SlotName, 0))
	{
		CurrentSaveGame = Cast<UAR_SaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));
		if (CurrentSaveGame == nullptr)
		{
			UE_LOG(LogTemp,Warning,TEXT("Failed to load save data that already exists."))
			return;
		}

		UE_LOG(LogTemp,Log,TEXT("Loaded save game data."))
	}
	else
	{
		CurrentSaveGame = Cast<UAR_SaveGame>(UGameplayStatics::CreateSaveGameObject(UAR_SaveGame::StaticClass()));

		UE_LOG(LogTemp,Log,TEXT("Created save game data."))
	}
}

void AAR_GameMode::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);
	UE_LOG(LogTemp,Log,TEXT("Handle new player %s"),*GetNameSafe(NewPlayer))
	AAR_PlayerState* PlayerState = NewPlayer->GetPlayerState<AAR_PlayerState>();
	if(PlayerState)
	{
		PlayerState->LoadPlayerState(CurrentSaveGame);
	}
}