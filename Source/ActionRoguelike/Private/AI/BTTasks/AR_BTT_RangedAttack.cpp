// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Bttasks/AR_BTT_RangedAttack.h"

#include "AIController.h"
#include "ActorComponent/AR_AttributeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"


UAR_BTT_RangedAttack::UAR_BTT_RangedAttack()
{
	FireSpread_Yaw = 5.0f;
	FireSpread_Pitch = 5.0f;
}

EBTNodeResult::Type UAR_BTT_RangedAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	AActor* TargetActor = Cast<AActor>(
		OwnerComp.GetBlackboardComponent()->GetValueAsObject(TargetActorKey.SelectedKeyName));

	if (!TargetActor) return EBTNodeResult::Failed;
	UAR_AttributeComponent* TargetAttribute = Cast<UAR_AttributeComponent>(
		TargetActor->GetComponentByClass(UAR_AttributeComponent::StaticClass()));
	if (!TargetAttribute) return EBTNodeResult::Failed;
	if (!TargetAttribute->IsAlive()) return EBTNodeResult::Failed;

	if (ensure(AIController))
	{
		ACharacter* AICharacter = Cast<ACharacter>(AIController->GetPawn());
		if (ensure(AICharacter))
		{
			FVector MuzzleLocation = AICharacter->GetMesh()->GetSocketLocation(SpawnSocketName);
			FVector ProjectileDirection = TargetActor->GetActorLocation() - MuzzleLocation;
			FRotator MuzzleRotation = ProjectileDirection.Rotation();

			AICharacter->FaceRotation(MuzzleRotation);

			MuzzleRotation.Yaw += FMath::RandRange(0.0f, FireSpread_Yaw);
			MuzzleRotation.Pitch += FMath::RandRange(-FireSpread_Pitch, FireSpread_Pitch);

			// FVector DebugLineStart = AICharacter->GetActorLocation();
			// FVector DebugLineEnd = DebugLineStart + MuzzleRotation.Vector() * 1500.0f;
			// DrawDebugDirectionalArrow(GetWorld(), DebugLineStart, DebugLineEnd, 15.0f, FColor::Black, false, 3, 0, 2);

			FActorSpawnParameters SpawnParameters;
			SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			SpawnParameters.Instigator = AICharacter;

			const AAR_BaseProjectile* Projectile = GetWorld()->SpawnActor<AAR_BaseProjectile>(
				ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParameters);

			return Projectile ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
		}
		else
		{
			return EBTNodeResult::Failed;
		}
	}

	return EBTNodeResult::Failed;
}
