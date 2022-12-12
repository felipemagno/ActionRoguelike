// Fill out your copyright notice in the Description page of Project Settings.


#include "UserInterface/AR_WorldUserWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/SizeBox.h"
#include "Kismet/GameplayStatics.h"

void UAR_WorldUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (IsValid(AttachedActor) && ParentSizeBox)
	{
		FVector2d ScreenPosition;
		if (UWidgetLayoutLibrary::ProjectWorldLocationToWidgetPosition(GetOwningPlayer(),
		                                                               AttachedActor->GetActorLocation() + WorldOffset,
		                                                               ScreenPosition, false))
		{
			ParentSizeBox->SetRenderTranslation(ScreenPosition);
		}
	}
	else
	{
		RemoveFromParent();
		UE_LOG(LogTemp, Warning, TEXT("Attached Actor no Longer Valid, removing Enemy Health Widget"));
	}
}
