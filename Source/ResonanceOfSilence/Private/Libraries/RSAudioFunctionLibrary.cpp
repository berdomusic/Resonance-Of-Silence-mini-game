// Fill out your copyright notice in the Description page of Project Settings.


#include "Libraries/RSAudioFunctionLibrary.h"

#include "System/GameReferences/RSGameReferencesSubsystem.h"

FRS_GR_SoundAsset URSAudioFunctionLibrary::GetSoundAssetByName(const FName& RowName)
{
    const UDataTable* SoundAssetsTable = URSGameReferencesSubsystem::Get().GetSoundAssetDataTable();
    if (!SoundAssetsTable)
    {
        return FRS_GR_SoundAsset();
    }
    if (const FRS_GR_SoundAsset* Asset = SoundAssetsTable->FindRow<FRS_GR_SoundAsset>(RowName, TEXT("Lookup Sound Asset")))
    {
        return *Asset;
    }
    return FRS_GR_SoundAsset();
}
