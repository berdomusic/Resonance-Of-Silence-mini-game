// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "RSGameUserSettings.generated.h"

USTRUCT(BlueprintType)
struct FRSGameplayUserSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	ERSGameDifficulty GameDifficulty = ERSGameDifficulty::Medium;
};

USTRUCT(BlueprintType)
struct FRSControlsUserSettings
{
	GENERATED_BODY()

};

USTRUCT(BlueprintType)
struct FRSPerformanceUserSettings
{
	GENERATED_BODY()

};

static const FVector2D ResolutionPresets[] = {
	{1280, 720},
	{1600, 900},
	{1920, 1080}, //default
	{2560, 1440},
	{3840, 2160}
};

USTRUCT(BlueprintType)
struct FRSGraphicsUserSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int GameResolutionPreset = 2;
};

USTRUCT(BlueprintType)
struct FRSSoundUserSettings
{
	GENERATED_BODY()

};
/**
 * 
 */
UCLASS()
class RESONANCEOFSILENCE_API URSGameUserSettings : public UGameUserSettings
{
	GENERATED_BODY()

	UPROPERTY(config)
	FRSGameplayUserSettings GameplayUserSettings;
	UPROPERTY(config)
	FRSControlsUserSettings ControlsUserSettings;
	UPROPERTY(config)
	FRSPerformanceUserSettings PerformanceUserSettings;
	UPROPERTY(config)
	FRSGraphicsUserSettings GraphicsUserSettings;
	UPROPERTY(config)
	FRSSoundUserSettings SoundUserSettings;

public:
	const FRSGameplayUserSettings& GetGameplayUserSettings() const { return GameplayUserSettings; }
	const FRSControlsUserSettings& GetControlsUserSettings() const { return ControlsUserSettings; }
	const FRSPerformanceUserSettings& GetPerformanceUserSettings() const { return PerformanceUserSettings; }
	const FRSGraphicsUserSettings& GetGraphicsUserSettings() const { return GraphicsUserSettings; }
	const FRSSoundUserSettings& GetSoundUserSettings() const { return SoundUserSettings; }

	UFUNCTION(BlueprintPure, meta = (CompactNodeTitle = "RS GameUserSettings", DisplayName = "GetRSGameUserSettings"))
	static URSGameUserSettings* Get();
#pragma region Gameplay
	
	UPROPERTY(BlueprintAssignable)
	FRSOnDifficultyChanged OnDifficultyChanged;
	UFUNCTION(BlueprintCallable)
	void ApplyGameplaySettings(const FRSGameplayUserSettings& InNewSettings);

#pragma endregion

#pragma region Controls

	UFUNCTION(BlueprintCallable)
	void ApplyControlsSettings(const FRSControlsUserSettings& InNewSettings);

#pragma endregion

#pragma region Performance

	UFUNCTION(BlueprintCallable)
	void ApplyPerformanceSettings(const FRSPerformanceUserSettings& InNewSettings);
#pragma endregion

#pragma region Graphics
	UFUNCTION(BlueprintCallable)
	void ApplyGraphicsSettings(const FRSGraphicsUserSettings& InNewSettings);
		
	UFUNCTION(BlueprintPure)
	static FVector2D GetResolutionPresetByIndex(int Idx);
	
	static constexpr int32 MAX_RESOLUTION_PRESETS = 4;	
private:
	void SetGameResolutionByPreset(int Idx);
public:
#pragma endregion

#pragma region Sound

	UFUNCTION(BlueprintCallable)
	void ApplySoundSettings(const FRSSoundUserSettings& InNewSettings);
#pragma endregion

};

UCLASS()
class RESONANCEOFSILENCE_API URSGameUserSettingsFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintPure, Category = GameUserSettings)
	static FRSGameplayUserSettings GetGameplayUserSettings();
	UFUNCTION(BlueprintPure, Category = GameUserSettings)
	static FRSControlsUserSettings GetControlsUserSettings();
	UFUNCTION(BlueprintPure, Category = GameUserSettings)
	static FRSPerformanceUserSettings GetPerformanceUserSettings();
	UFUNCTION(BlueprintPure, Category = GameUserSettings)
	static FRSGraphicsUserSettings GetGraphicsUserSettings();
	UFUNCTION(BlueprintPure, Category = GameUserSettings)
	static FRSSoundUserSettings GetSoundUserSettings();	
};
