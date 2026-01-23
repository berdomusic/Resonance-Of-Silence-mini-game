// Fill out your copyright notice in the Description page of Project Settings.

#include "System/UI/RSUISubsystem.h"

#include "UI/RSTooltipEntry.h"

URSUISubsystem* URSUISubsystem::Get(const UObject* InWorldContextObject)
{
	if (InWorldContextObject)
	{
		if (UWorld* World = InWorldContextObject->GetWorld())
		{
			if (UGameInstance* GameInstance = World->GetGameInstance())
			{
				return GameInstance->GetSubsystem<URSUISubsystem>();
			}
		}
	}
	return nullptr;
}

void URSUISubsystem::RegisterUserWidget(URSUserWidget* InWidget)
{
	if (!IsValid(InWidget))
		return;

	ActiveWidgets.AddUnique(InWidget);
}

void URSUISubsystem::UnregisterUserWidget(URSUserWidget* InWidget)
{
	ActiveWidgets.RemoveAll([](const TWeakObjectPtr<URSUserWidget>& Ptr)
		{
			return !Ptr.IsValid();
		});
	ActiveWidgets.RemoveAll(
		[InWidget](const TWeakObjectPtr<URSUserWidget>& Ptr)
		{
			return Ptr.Get() == InWidget;
		}
	);	
}

void URSUISubsystem::SwapWidgetInViewport(URSUserWidget* InWidget, int32 InZOrder, bool bPreserveWidgets)
{	
	ARSPlayerController* RSPlayerController = URSCharacterFunctionLibrary::GetPlayerController(this);
	if (!ensure(RSPlayerController))
		return;
	
	if (IsValid(CurrentWidget))
	{
		RemoveWidgetFromViewport(CurrentWidget);
		CurrentWidget = nullptr;
	}
    
	if (!IsValid(InWidget))
		return;
	
	CurrentWidget = InWidget;
	CurrentWidget->AddToViewport(InZOrder);
}

void URSUISubsystem::AddOptionsWidget(URSUserWidget* InOptionsWidget, bool bHideGameplayWidgets)
{
	if (bHideGameplayWidgets)
	{
		for (const TWeakObjectPtr<URSUserWidget>& widgetPtr : ActiveWidgets)
		{
			if (widgetPtr.IsValid())
			{
				if (URSGameplayWidget* gameplayWidget = Cast<URSGameplayWidget>(widgetPtr.Get()))
				{
					HiddenGameplayWidgets.AddUnique(gameplayWidget);
					gameplayWidget->SetVisibility(ESlateVisibility::Hidden);
				}
			}
		}
	}
	CurrentWidget = InOptionsWidget;
	CurrentWidget->AddToViewport(0);
}

void URSUISubsystem::RemoveOptionsWidget(URSUserWidget* InOptionsWidget, bool bRestoreHiddenGameplayWidgets)
{
	if (bRestoreHiddenGameplayWidgets && !HiddenGameplayWidgets.IsEmpty())
	{
		for (URSGameplayWidget* gameplayWidget : HiddenGameplayWidgets)
		{
			gameplayWidget->SetVisibility(ESlateVisibility::Visible);
		}
		HiddenGameplayWidgets.Empty();
	}
	RemoveWidgetFromViewport(InOptionsWidget);
}

void URSUISubsystem::RemoveWidgetFromViewport(URSUserWidget* InWidget)
{
	if (!InWidget)
		return;
	const ARSPlayerController* RSPlayerController = URSCharacterFunctionLibrary::GetPlayerController(this);
	check(RSPlayerController)
	InWidget->RemoveFromParent();
	CurrentWidget = nullptr;
}
