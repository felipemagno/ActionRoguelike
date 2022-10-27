// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AR_AICharacter.h"

// Sets default values
AAR_AICharacter::AAR_AICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AAR_AICharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAR_AICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AAR_AICharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

