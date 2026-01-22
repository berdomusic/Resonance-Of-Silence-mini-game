// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Puzzles/RSPuzzleSubsystem.h"

#include "System/GameUserSettings/RSGameUserSettings.h"

URSPuzzleSubsystem* URSPuzzleSubsystem::Get(UObject* InWorldContextObject)
{
	if (IsValid(InWorldContextObject))
	{
		if (UWorld* World = InWorldContextObject->GetWorld())
		{
			return World->GetSubsystem<URSPuzzleSubsystem>();
		}
	}
	return nullptr;
}

void URSPuzzleSubsystem::ActivatePuzzle(ARSPuzzleBase* InPuzzle)
{
	check(InPuzzle);
	InPuzzle->ActivatePuzzle();
	ActivePuzzles.AddUnique(InPuzzle);
}

void URSPuzzleSubsystem::DeactivatePuzzle(ARSPuzzleBase* InPuzzle)
{
	check(InPuzzle);
	InPuzzle->DeactivatePuzzle();
	for (int i = ActivePuzzles.Num() - 1; i >= 0; --i)
	{
		if (ActivePuzzles[i] == InPuzzle)
		{
			ActivePuzzles.RemoveAt(i);
			break;
		}
	}
}
