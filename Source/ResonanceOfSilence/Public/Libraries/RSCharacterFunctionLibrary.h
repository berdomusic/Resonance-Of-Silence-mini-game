// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "RSCharacterFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class RESONANCEOFSILENCE_API URSCharacterFunctionLibrary : public URSMainFunctionLibrary
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintPure)
	static bool IsActorPlayerCharacter(const AActor* InActor);

	UFUNCTION(BlueprintPure, meta = (WorldContext = "InWorldContext", CompactNodeTitle = "RS PlayerController", DisplayName = "Get RS PlayerController"))
	static ARSPlayerController* GetPlayerController(const UObject* InWorldContext);

	UFUNCTION(BlueprintPure, meta = (WorldContext = "InWorldContext", CompactNodeTitle = "RS PlayerCharacter", DisplayName = "Get RS PlayerCharacter"))
	static ARSPlayerCharacter* GetPlayerCharacter(const UObject* InWorldContext);
};
