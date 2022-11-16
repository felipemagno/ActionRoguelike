// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AR_AICharacter.generated.h"

class UPawnSensingComponent;
class UAR_AttributeComponent;
UCLASS()
class ACTIONROGUELIKE_API AAR_AICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAR_AICharacter();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UPawnSensingComponent* PawnSensingComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UAR_AttributeComponent* AttributeComponent;

	FName TargetActorKey;

	UFUNCTION()
	void SightResponse(APawn* Pawn);
	virtual void PostInitializeComponents() override;
};
