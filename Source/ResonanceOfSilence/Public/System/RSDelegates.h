// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RSDelegates.generated.h"

class USoundBase;
/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FRSDynamicMutlicastDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRSDynamicMulticastNameDelegate, FName, InName);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRSDynamicMulticastBoolDelegate, bool, bInResult);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRSDynamicMulticastFloatDelegate, float, InValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRSDynamicMulticastIntDelegate, int32, InValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRSDynamicMulticastVectorDelegate, FVector, InVector);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRSDynamicMulticastVector2DDelegate, FVector2D, InVector2D);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRSTransmittedSoundDataDelegate, const FRSTransmittedSoundData&, InData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRSOnDifficultyChanged, const ERSGameDifficulty, InNewDifficulty);

UCLASS()
class URSDummy : public UObject
{
	GENERATED_BODY()
};