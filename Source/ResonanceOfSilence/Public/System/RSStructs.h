// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RSStructs.generated.h"

/**
 * 
 */

class USoundBase;

USTRUCT(BlueprintType)
struct FRSTransmittedSoundData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int Frequency = 440.f;
};

UENUM(BlueprintType)
enum class ERSGameDifficulty : uint8
{
	Easy		UMETA(DisplayName = "Easy"),
	Medium		UMETA(DisplayName = "Medium"),
	Hard		UMETA(DisplayName = "Hard"),

	MAX   UMETA(Hidden)
};

USTRUCT(BlueprintType)
struct FRSPuzzleDifficultyBase :public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "General")
	bool bIsFixedDifficulty = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "General")
	ERSGameDifficulty AssociatedGameDifficulty = ERSGameDifficulty::Medium;
};