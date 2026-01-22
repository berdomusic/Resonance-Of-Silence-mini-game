// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/EngineSubsystem.h"
#include "System/GameReferences/RSGameReferencesSettings.h"
#include "RSGameReferencesSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class RESONANCEOFSILENCE_API URSGameReferencesSubsystem : public UEngineSubsystem
{
	GENERATED_BODY()

public:
	static URSGameReferencesSubsystem& Get();

	UDataTable* GetSoundAssetDataTable() const;

protected:
	static void LoadTable(const TSoftObjectPtr<UDataTable>& SoftDataTableReference, UDataTable*& OutDataTable);

	UPROPERTY()
	mutable UDataTable* SoundAssetDataTable = nullptr;
};
