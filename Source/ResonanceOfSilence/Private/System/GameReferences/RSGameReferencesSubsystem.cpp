// Fill out your copyright notice in the Description page of Project Settings.


#include "System/GameReferences/RSGameReferencesSubsystem.h"

URSGameReferencesSubsystem& URSGameReferencesSubsystem::Get()
{
	URSGameReferencesSubsystem* const subsystem = GEngine->GetEngineSubsystem<URSGameReferencesSubsystem>();
	check(subsystem);
	return *subsystem;
}

UDataTable* URSGameReferencesSubsystem::GetSoundAssetDataTable() const
{
	if(!SoundAssetDataTable)
	{
		SoundAssetDataTable = URSGameReferencesSettings::Get().SoundAssetTableSoft.DataTableHandle.LoadSynchronous();
	}
	return SoundAssetDataTable;
}

void URSGameReferencesSubsystem::LoadTable(const TSoftObjectPtr<UDataTable>& SoftDataTableReference,
                                           UDataTable*& OutDataTable)
{
	if (!OutDataTable)
		return;
	
	OutDataTable = SoftDataTableReference.LoadSynchronous();	
}
