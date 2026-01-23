// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RSMenuWidget.h"
#include "UObject/Object.h"
#include "System/GameUserSettings/RSGameUserSettings.h"
#include "RSOptionWidget.generated.h"

/**
 * 
 */
UCLASS()
class RESONANCEOFSILENCE_API URSOptionWidget : public URSMenuWidget
{
	GENERATED_BODY()
public:
	void UpdateInternalVariables();
	void NativeConstruct() override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ApplyInternalSettings();

	UPROPERTY(BlueprintReadWrite)
	FRSGameplayUserSettings InternalTemp_GameplayUserSettings;
	UPROPERTY(BlueprintReadWrite)
	FRSControlsUserSettings InternalTemp_ControlsUserSettings;
	UPROPERTY(BlueprintReadWrite)
	FRSPerformanceUserSettings InternalTemp_PerformanceUserSettings;
	UPROPERTY(BlueprintReadWrite)
	FRSGraphicsUserSettings InternalTemp_GraphicsUserSettings;
	UPROPERTY(BlueprintReadWrite)
	FRSSoundUserSettings InternalTemp_SoundUserSettings;
};
