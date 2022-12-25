// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AR_TargetDummy.generated.h"

class UAR_AttributeComponent;

UCLASS(Abstract)
class ACTIONROGUELIKE_API AAR_TargetDummy : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AAR_TargetDummy();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* MeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UAR_AttributeComponent* AttributeComponent;

	UFUNCTION()
	void OnHitEvent(AActor* InstigatingActor, UAR_AttributeComponent* OwningAttribute, float NewHealthValue,
	                float DeltaValue, float NewHealthPercentage);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
