// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/AR_GameMode.h"

#include "AR_Player.h"
#include "EngineUtils.h"
#include "Core/AR_PlayerState.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "Interfaces/AR_IGameplayInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Core/AR_SaveGame.h"
#include "GameFramework/GameStateBase.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"


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
	LoadActorsData();
}

void AAR_GameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
	LoadSaveGame();
	UE_LOG(LogTemp, Log, TEXT("InitGame start."))
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
	for (int32 i = 0; i < GameState->PlayerArray.Num(); i++)
	{
		AAR_PlayerState* PlayerState = Cast<AAR_PlayerState>(GameState->PlayerArray[i]);
		if (PlayerState)
		{
			PlayerState->SavePlayerState(CurrentSaveGame);
			break; // only single player right now
		}
	}

	CurrentSaveGame->SavedActors.Empty();
	for (FActorIterator It(GetWorld()); It; ++It)
	{
		AActor* Actor = *It;
		if (!Actor->Implements<UAR_IGameplayInterface>())
		{
			continue;
		}

		FActorSaveData ActorData;
		ActorData.ActorName = Actor->GetName();
		ActorData.ActorTransform = Actor->GetActorTransform();

		FMemoryWriter MemoryWriter(ActorData.ByteData);
		// Find only variables with UPROPERTY(SaveGame)
		FObjectAndNameAsStringProxyArchive Archive(MemoryWriter, true);
		// Converts Actor's savegame UPROPERTIES into binary array
		Archive.ArIsSaveGame = true;

		Actor->Serialize(Archive);
		
		CurrentSaveGame->SavedActors.Add(ActorData);
	}

	UGameplayStatics::SaveGameToSlot(CurrentSaveGame, SlotName, 0);
}

void AAR_GameMode::LoadSaveGame()
{
	if (UGameplayStatics::DoesSaveGameExist(SlotName, 0))
	{
		CurrentSaveGame = Cast<UAR_SaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));
		if (CurrentSaveGame == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to load save data that already exists."))
			return;
		}

		// This will not work for world partition:
		/*"FYI In UE5 if you have World Partition enabled and Actor has "Is Spatially Loaded" set to true,
		 * then it may not be available in the GetWorld() actors iterator during GameModeBase::InitGame." 
		 */

		// for (FActorIterator It(GetWorld()); It; ++It)
		// {
		// 	AActor* Actor = *It;
		// 	if (!Actor->Implements<UAR_IGameplayInterface>())
		// 	{
		// 		continue;
		// 	}
		//
		// 	for (FActorSaveData ActorData : CurrentSaveGame->SavedActors)
		// 	{
		// 		if (ActorData.ActorName == Actor->GetName())
		// 		{
		// 			Actor->SetActorTransform(ActorData.ActorTransform);
		// 			break;
		// 		}
		// 	}
		// }

		UE_LOG(LogTemp, Log, TEXT("Loaded save game data."))
	}
	else
	{
		CurrentSaveGame = Cast<UAR_SaveGame>(UGameplayStatics::CreateSaveGameObject(UAR_SaveGame::StaticClass()));

		UE_LOG(LogTemp, Log, TEXT("Created save game data."))
	}
}

void AAR_GameMode::LoadActorsData()
{
	for (FActorIterator It(GetWorld()); It; ++It)
	{
		AActor* Actor = *It;
		if (!Actor->Implements<UAR_IGameplayInterface>())
		{
			continue;
		}

		for (FActorSaveData ActorData : CurrentSaveGame->SavedActors)
		{
			if (ActorData.ActorName == Actor->GetName())
			{
				Actor->SetActorTransform(ActorData.ActorTransform);

				FMemoryReader MemoryReader(ActorData.ByteData);
				
				FObjectAndNameAsStringProxyArchive Archive(MemoryReader, true);				
				Archive.ArIsSaveGame = true;
				// Converts binary array into Actor's variables
				Actor->Serialize(Archive);

				IAR_IGameplayInterface::Execute_OnActorLoaded(Actor);
				break;
			}
		}
	}
}

void AAR_GameMode::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);
	UE_LOG(LogTemp, Log, TEXT("Handle new player %s"), *GetNameSafe(NewPlayer))
	AAR_PlayerState* PlayerState = NewPlayer->GetPlayerState<AAR_PlayerState>();
	if (PlayerState)
	{
		PlayerState->LoadPlayerState(CurrentSaveGame);
	}

	// Loading information when each player is loaded
	LoadActorsData();
}
