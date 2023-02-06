// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "GameFramework/GameModeBase.h"
#include "AR_GameMode.generated.h"

class UEnvQuery;
class UEnvQueryInstanceBlueprintWrapper;
class AController;
class UAR_SaveGame;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnActorKilled, AActor*, VictimActor, AActor*, KillerActor);

/**
 * 
 */
UCLASS(Abstract)
class ACTIONROGUELIKE_API AAR_GameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:
	FString SlotName;

	UPROPERTY()
	UAR_SaveGame* CurrentSaveGame;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	UEnvQuery* SpawnBotQuery;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	UClass* AiClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	double AiHalfHeight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float SpawnTimerInterval;

	UPROPERTY(EditAnywhere, Category = "AI")
	UCurveFloat* DifficultyCurve;

	UPROPERTY(EditAnywhere, Category = "AI")
	int BotMaxCount;

	UPROPERTY(VisibleAnywhere, Category = "AI")
	int BotCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	FTimerHandle TimerHandle_SpawnAI;

	// FUNCTIONS
	UFUNCTION()
	void SpawnEQSFinishedEvent(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

	UFUNCTION()
	void SpawnBotTimerElapsed();

	UFUNCTION()
	void RespawnPlayerElapsed(AController* Controller);

public:
	AAR_GameMode();

	UPROPERTY(BlueprintAssignable)
	FOnActorKilled OnActorKilled;

	UFUNCTION()
	virtual void ActorKilledEvent(AActor* VictimActor, AActor* KillerActor);

	virtual void StartPlay() override;

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

	UFUNCTION(BlueprintCallable, Category = "Savegame")
	void WriteSaveGame();

	void LoadSaveGame();

	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;
};
