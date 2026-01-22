// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Actors/Puzzles/RSPuzzleBase.h"
#include "RSPuzzleSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class RESONANCEOFSILENCE_API URSPuzzleSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	static URSPuzzleSubsystem* Get(UObject* InWorldContextObject);

	UFUNCTION(BlueprintCallable)
	void ActivatePuzzle(ARSPuzzleBase* InPuzzle);
	UFUNCTION(BlueprintCallable)
	void DeactivatePuzzle(ARSPuzzleBase* InPuzzle);

private:
	TArray<ARSPuzzleBase*> ActivePuzzles;
};
