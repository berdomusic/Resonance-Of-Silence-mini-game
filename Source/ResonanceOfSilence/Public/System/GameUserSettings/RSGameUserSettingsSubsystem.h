// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "RSGameUserSettingsSubsystem.generated.h"

USTRUCT(BlueprintType)
struct FRSUserGraphicsSettings
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector2D Resolution = { 1920.f, 1080 };
};

UCLASS()
class RESONANCEOFSILENCE_API URSGameUserSettingsSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	static URSGameUserSettingsSubsystem* Get(const UObject* InWorldContext);
	
};
