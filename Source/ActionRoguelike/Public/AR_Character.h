// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AR_Character.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UAR_InteractionComponent;
class UAnimMontage;
class AAR_MagicProjectile;
class UAR_AttributeComponent;

UCLASS()
class ACTIONROGUELIKE_API AAR_Character : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAR_Character();

protected:
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere)
	UAR_InteractionComponent* InteractionComp;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	UAR_AttributeComponent* AttributeComp;

	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AAR_MagicProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AAR_MagicProjectile> SpecialAttackProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AAR_MagicProjectile> SpecialAbilityProjectileClass;
	
	UPROPERTY(EditAnywhere, Category = "Attack")
	UAnimMontage* AttackAnimation;

	UPROPERTY(EditAnywhere, Category = "Attack")
	FTimerHandle TimerHandle_PrimaryAttack;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void MoveForward(float value);
	void MoveRight(float value);
	void ExecutePrimaryAttack();
	void PrimaryAttack();
	AAR_MagicProjectile* SpawnProjectile(TSubclassOf<AAR_MagicProjectile> Projectile);
	void PrimaryInteract();
	void ExecuteSpecialAttack();
	void SpecialAttack();
	void ExecuteSpecialAbility();
	void SpecialAbility();
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
