// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/AR_IGameplayInterface.h"
#include "Projectile/AR_BaseProjectile.h"
#include "AR_Player.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UAR_InteractionComponent;
class UAnimMontage;
class AAR_MagicProjectile;
class UAR_AttributeComponent;
class UAR_ActionComponent;
class UAR_BaseAction;

UCLASS(Abstract)
class ACTIONROGUELIKE_API AAR_Player : public ACharacter, public IAR_IGameplayInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAR_Player();

protected:
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere)
	UAR_InteractionComponent* InteractionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UAR_AttributeComponent* AttributeComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UAR_ActionComponent* ActionComponent;

	UPROPERTY(EditAnywhere, Category = "Effects")
	FName HitFlashTime_ParameterName;

public:
	// MOVEMENT
	void MoveForward(float value);
	void MoveRight(float value);

	// ACTIONS
	void PrimaryAttack();
	void PrimaryInteract();
	void SpecialAttack();
	void SpecialAbility();
	void SprintStart();
	void SprintStop();

	// HELPER
	virtual void GetActorEyesViewPoint(FVector& OutLocation, FRotator& OutRotation) const override;

	// EVENTS
	UFUNCTION()
	void Death(AActor* InstigatingActor, UAR_AttributeComponent* OwningAttribute);
	UFUNCTION()
	void HealthChanged(AActor* InstigatingActor, UAR_AttributeComponent* OwningAttribute, float NewHealthValue,
	                   float DeltaValue, float NewHealthPercentage);

	// CONSOLE COMMANDS
	UFUNCTION(Exec, meta=(DevelopmentOnly))
	void AR_HealSelf(float Ammount = 100);

	UFUNCTION(Exec, meta=(DevelopmentOnly))
	void AR_ToggleGodMode();

	// OVERRIDES
	virtual void PostInitializeComponents() override;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// GAMEPLAY INTERFACE
	virtual bool SpendCredits_Implementation(int32 CreditsCost) override;
	virtual bool ReceiveCredits_Implementation(int32 DeltaValue) override;
	virtual int32 GetCreditsValue_Implementation() override;
};
