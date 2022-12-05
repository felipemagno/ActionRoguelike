// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Projectile/AR_BaseProjectile.h"
#include "AR_Player.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UAR_InteractionComponent;
class UAnimMontage;
class AAR_MagicProjectile;
class UAR_AttributeComponent;

UCLASS(Abstract)
class ACTIONROGUELIKE_API AAR_Player : public ACharacter
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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UAR_AttributeComponent* AttributeComp;

	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AAR_BaseProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AAR_BaseProjectile> SpecialAttackProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AAR_BaseProjectile> SpecialAbilityProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Attack")
	UAnimMontage* AttackAnimation;

	UPROPERTY(EditAnywhere, Category = "Attack")
	FName ProjectileSpawnSocket = "Muzzle_01";

	UPROPERTY(EditAnywhere, Category = "Attack")
	FTimerHandle TimerHandle_PrimaryAttack;

	UPROPERTY(EditAnywhere, Category = "Effects")
	FName HitFlashTime_ParameterName;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// MOVEMENT
	void MoveForward(float value);
	void MoveRight(float value);

	// ACTIONS
	void ExecutePrimaryAttack();
	void PrimaryAttack();
	void PrimaryInteract();
	void ExecuteSpecialAttack();
	void SpecialAttack();
	void ExecuteSpecialAbility();
	void SpecialAbility();

	// HELPER
	AAR_BaseProjectile* SpawnProjectile(TSubclassOf<AAR_BaseProjectile> Projectile);
	FRotator GetRotationToView(FVector SpawnLocation);
	virtual void GetActorEyesViewPoint(FVector& OutLocation, FRotator& OutRotation) const override;

	UFUNCTION()
	void Death(AActor* InstigatingActor, UAR_AttributeComponent* OwningAttribute);
	UFUNCTION()
	void HealthChanged(AActor* InstigatingActor, UAR_AttributeComponent* OwningAttribute, float NewHealthValue,
	                   float DeltaValue, float NewHealthPercentage);
	
	// CONSOLE COMMANDS
	UFUNCTION(Exec,meta=(DevelopmentOnly))
	void AR_HealSelf(float Ammount = 100);

	UFUNCTION(Exec,meta=(DevelopmentOnly))
	void AR_ToggleGodMode();

	// OVERRIDES
	virtual void PostInitializeComponents() override;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
