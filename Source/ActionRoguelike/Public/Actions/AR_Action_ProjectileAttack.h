// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actions/AR_BaseAction.h"
#include "AR_Action_ProjectileAttack.generated.h"

class AAR_BaseProjectile;
/**
 * 
 */
UCLASS(Abstract)
class ACTIONROGUELIKE_API UAR_Action_ProjectileAttack : public UAR_BaseAction
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AAR_BaseProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Attack")
	UAnimMontage* AttackAnimation;

	UPROPERTY(EditAnywhere, Category = "Attack")
	FName ProjectileSpawnSocket = "Muzzle_01";

	UPROPERTY(EditAnywhere, Category = "Effects")
	float AttackAnimDelay;

	UFUNCTION()
	void AttackDelay_Elapsed(ACharacter* InstigatorCharacter);

	FRotator GetRotationToView(ACharacter* InstigatorCharacter, const FVector SpawnLocation) const;
public:
	UAR_Action_ProjectileAttack();

	virtual void StartAction_Implementation(AActor* Instigator) override;
};
