// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RSMenuWidget.h"

#include "Blueprint/WidgetBlueprintLibrary.h"

void URSMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
	UGameplayStatics::SetGamePaused(this, true);
	if (!IsValid(GetWorld()))
	{
		checkNoEntry()
			return;
	}
	ARSPlayerController* playerController = URSCharacterFunctionLibrary::GetPlayerController(GetWorld());
	if (!IsValid(playerController))
	{
		checkNoEntry()
			return;
	}
	playerController->SetShowMouseCursor(true);
	UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(playerController, this);
}

void URSMenuWidget::NativeDestruct()
{
	ARSPlayerController* playerController = URSCharacterFunctionLibrary::GetPlayerController(GetWorld());
	if (IsValid(playerController))
	{
		playerController->SetShowMouseCursor(false);
		UWidgetBlueprintLibrary::SetInputMode_GameOnly(playerController, false);
	}
	UGameplayStatics::SetGamePaused(this, false);	
	Super::NativeDestruct();
}

void URSMenuWidget::OnNewGameClicked()
{
	if (!IsValid(GetWorld()))
	{
		checkNoEntry()
			return;
	}
	ARSPlayerController* playerController = URSCharacterFunctionLibrary::GetPlayerController(GetWorld());
	if (!IsValid(playerController))
	{
		checkNoEntry()
			return;
	}
	playerController->SetShowMouseCursor(true);
	UWidgetBlueprintLibrary::SetInputMode_GameOnly(playerController, false);
}

void URSMenuWidget::OnExitGameClicked()
{
}
