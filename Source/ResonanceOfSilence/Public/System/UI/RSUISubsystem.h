// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "UI/RSUserWidget.h"
#include "UI/RSGameplayWidget.h"
#include "RSUISubsystem.generated.h"

/**
 * 
 */
UCLASS()
class RESONANCEOFSILENCE_API URSUISubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	static URSUISubsystem* Get(const UObject* InWorldContextObject);

	void RegisterUserWidget(URSUserWidget* InWidget);
	void UnregisterUserWidget(URSUserWidget* InWidget);

	UPROPERTY()
	TArray<TWeakObjectPtr<URSUserWidget>> ActiveWidgets;
	UPROPERTY()
	TArray<URSGameplayWidget*> HiddenGameplayWidgets;

	UFUNCTION(BlueprintCallable)
	void SwapWidgetInViewport(URSUserWidget* InWidget, int32 InZOrder = 0, bool bPreserveWidgets = false);
	UFUNCTION(BlueprintCallable)
	void AddOptionsWidget(URSUserWidget* InOptionsWidget, bool bHideGameplayWidgets = true);
	UFUNCTION(BlueprintCallable)
	void RemoveOptionsWidget(URSUserWidget* InOptionsWidget, bool bRestoreHiddenGameplayWidgets = true);
	UFUNCTION(BlueprintCallable)
	void RemoveWidgetFromViewport(URSUserWidget* InWidget);

	UPROPERTY(BlueprintReadOnly)
	URSUserWidget* CurrentWidget;
};
