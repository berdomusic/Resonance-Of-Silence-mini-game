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

void URSUISubsystem::SwapWidgetInViewport(URSUserWidget* InWidget, int32 InZOrder)
{	
	/*ARSPlayerController* RSPlayerController = URSCharacterFunctionLibrary::GetPlayerController(this);
	check(RSPlayerController)
	if (CurrentWidget)
	{
		RemoveWidgetFromViewport(CurrentWidget);
		//CurrentWidget = nullptr;
	}
	if (!InWidget)
		return;
	if (const TSubclassOf<URSUserWidget> WidgetClass = InWidget->GetClass())
	{
		CurrentWidget = CreateWidget<URSUserWidget>(RSPlayerController, WidgetClass);
		InWidget->AddToViewport(InZOrder);
	}*/
	
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

void URSUISubsystem::RemoveWidgetFromViewport(URSUserWidget* InWidget)
{
	if (!InWidget)
		return;
	const ARSPlayerController* RSPlayerController = URSCharacterFunctionLibrary::GetPlayerController(this);
	check(RSPlayerController)
	InWidget->RemoveFromParent();
	CurrentWidget = nullptr;
}

void URSUISubsystem::AddTooltipWidget(URSTooltipEntry* InTooltip)
{
	

	InTooltip->AddToViewport();
}

void URSUISubsystem::CreateTooltipWidgetFromRowHandle(UObject* WorldContextObject, const FDataTableRowHandle& TooltipRowHandle)
{
	/*check(WidgetClass)
	// tooltips are designed to manage their destruction
	ensure(WidgetClass->TooltipHandle.Lifetime > 0.f || WidgetClass->IsWidgetFading());*/
}
