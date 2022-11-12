// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "GameFramework/GameModeBase.h"
#include "AR_GameMode.generated.h"

class UEnvQuery;
class UEnvQueryInstanceBlueprintWrapper;
/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API AAR_GameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:
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


	UFUNCTION()
	void OnSpawnEQSFinished(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

	UFUNCTION()
	void SpawnBotTimerElapsed();

public:
	AAR_GameMode();


	virtual void StartPlay() override;
};
