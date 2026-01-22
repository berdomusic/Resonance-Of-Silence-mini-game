// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "System/GameReferences/RSGameReferencesStructs.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "RSAudioFunctionLibrary.generated.h"

struct FRS_GR_SoundAsset;

UCLASS()
class RESONANCEOFSILENCE_API URSAudioFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFUNCTION(BlueprintPure)
	static FRS_GR_SoundAsset GetSoundAssetByName(const FName& RowName);
};
