// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
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
class UInputAction;



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

	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTag GameTag_PrimaryAttack;

	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTag GameTag_SpecialAttack;

	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTag GameTag_MovementAbility;

	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTag GameTag_Sprint;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	UInputAction* Input_MoveForward;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	UInputAction* Input_MoveRight;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	UInputAction* Input_LookUp;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	UInputAction* Input_Turn;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	UInputAction* Input_PrimaryAttack;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	UInputAction* Input_SecondaryAttack;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	UInputAction* Input_Interact;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	UInputAction* Input_MovementAbility;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	UInputAction* Input_Jump;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	UInputAction* Input_Sprint;
public:
	// MOVEMENT
	void MoveForward(const FInputActionValue& InputActionValue);
	void MoveRight(const FInputActionValue& InputActionValue);

	// LOOK ROTATIONS
	void Turn(const FInputActionValue& InputActionValue);
	void LookUp(const FInputActionValue& InputActionValue);

	// ACTIONS
	void PrimaryAttack(const FInputActionValue& InputActionValue);
	void PrimaryInteract(const FInputActionValue& InputActionValue);
	void SpecialAttack(const FInputActionValue& InputActionValue);
	void SpecialAbility(const FInputActionValue& InputActionValue);
	void SprintStart(const FInputActionValue& InputActionValue);
	void SprintStop(const FInputActionValue& InputActionValue);

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
