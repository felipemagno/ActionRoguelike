// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AR_InteractionComponent.generated.h"

class UAR_WorldUserWidget;

UCLASS(ClassGroup=(ActionRoguelike), meta=(BlueprintSpawnableComponent))
class ACTIONROGUELIKE_API UAR_InteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	void PrimaryInteract();

	// Sets default values for this component's properties
	UAR_InteractionComponent();

protected:
	UPROPERTY(BlueprintReadOnly, Category="Interaction")
	AActor* FocusedActor;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Interaction")
	TSubclassOf<UAR_WorldUserWidget> InteractWidgetClass;

	UPROPERTY(BlueprintReadOnly, Category="Interaction")
	UAR_WorldUserWidget* InteractWidgetInstance;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Interaction")
	float TraceDistance;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Interaction")
	float TraceRadius;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Interaction")
	TEnumAsByte<ECollisionChannel> TraceCollisionChannel;
	
	// Called when the game starts
	virtual void BeginPlay() override;

	void FindBestInteractable();


public:
	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
