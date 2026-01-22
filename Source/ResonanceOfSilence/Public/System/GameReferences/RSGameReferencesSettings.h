// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "RSGameReferencesSettings.generated.h"

USTRUCT(BlueprintType)
struct FRSGameReferenceTableInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<UDataTable> DataTableHandle;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bKeepLoaded = true;

};
UCLASS(Config = Game, defaultconfig, meta = (DisplayName = "RS - Game References"))
class RESONANCEOFSILENCE_API URSGameReferencesSettings final : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	static const URSGameReferencesSettings& Get();

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get Game References Settings"))
	static const URSGameReferencesSettings* K2_Get();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, config)
	FRSGameReferenceTableInfo SoundAssetTableSoft;
};
