// Fill out your copyright notice in the Description page of Project Settings.


#include "System/GameUserSettings/RSGameUserSettings.h"

URSGameUserSettings* URSGameUserSettings::Get()
{
	return Cast<URSGameUserSettings>(GetGameUserSettings());
}

void URSGameUserSettings::ApplyGameplaySettings(const FRSGameplayUserSettings& InNewSettings)
{
	if (InNewSettings.GameDifficulty != GameplayUserSettings.GameDifficulty)
	{
		OnDifficultyChanged.Broadcast(InNewSettings.GameDifficulty);
	}
	GameplayUserSettings = InNewSettings;
	SaveConfig();
}

void URSGameUserSettings::ApplyControlsSettings(const FRSControlsUserSettings& InNewSettings)
{
	SaveConfig();
}

void URSGameUserSettings::ApplyPerformanceSettings(const FRSPerformanceUserSettings& InNewSettings)
{
	SaveConfig();
}

void URSGameUserSettings::ApplyGraphicsSettings(const FRSGraphicsUserSettings& InNewSettings)
{
	if (InNewSettings.GameResolutionPreset != GraphicsUserSettings.GameResolutionPreset)
	{
		SetGameResolutionByPreset(InNewSettings.GameResolutionPreset);
	}
	GraphicsUserSettings = InNewSettings;
	SaveConfig();
}

void URSGameUserSettings::SetGameResolutionByPreset(int Idx)
{
	const FVector2D NewResolution = GetResolutionPresetByIndex(Idx);
	SetScreenResolution(FIntPoint(NewResolution.X, NewResolution.Y));
	ApplySettings(false);
}

void URSGameUserSettings::ApplySoundSettings(const FRSSoundUserSettings& InNewSettings)
{
	SaveConfig();
}

FVector2D URSGameUserSettings::GetResolutionPresetByIndex(int Idx)
{
	const int out = UKismetMathLibrary::Clamp(Idx, 0, MAX_RESOLUTION_PRESETS);
	return ResolutionPresets[out];
}

FRSGameplayUserSettings URSGameUserSettingsFunctionLibrary::GetGameplayUserSettings()
{
	if (const URSGameUserSettings* const settings = URSGameUserSettings::Get())
	{
		return settings->GetGameplayUserSettings();
	}
	return FRSGameplayUserSettings();
}

FRSControlsUserSettings URSGameUserSettingsFunctionLibrary::GetControlsUserSettings()
{
	if (const URSGameUserSettings* const settings = URSGameUserSettings::Get())
	{
		return settings->GetControlsUserSettings();
	}
	return FRSControlsUserSettings();
}

FRSPerformanceUserSettings URSGameUserSettingsFunctionLibrary::GetPerformanceUserSettings()
{
	if (const URSGameUserSettings* const settings = URSGameUserSettings::Get())
	{
		return settings->GetPerformanceUserSettings();
	}
	return FRSPerformanceUserSettings();
}

FRSGraphicsUserSettings URSGameUserSettingsFunctionLibrary::GetGraphicsUserSettings()
{
	if (const URSGameUserSettings* const settings = URSGameUserSettings::Get())
	{
		return settings->GetGraphicsUserSettings();
	}
	return FRSGraphicsUserSettings();
}

FRSSoundUserSettings URSGameUserSettingsFunctionLibrary::GetSoundUserSettings()
{
	if (const URSGameUserSettings* const settings = URSGameUserSettings::Get())
	{
		return settings->GetSoundUserSettings();
	}
	return FRSSoundUserSettings();
}
