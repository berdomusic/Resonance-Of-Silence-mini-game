// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Puzzles/RSPuzzleBase.h"

#include "System/GameUserSettings/RSGameUserSettings.h"

ARSPuzzleBase::ARSPuzzleBase()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
}

void ARSPuzzleBase::DeactivatePuzzle_Implementation()
{
	bPuzzleIsActive = false;
	check(GetWorld());
	GetWorld()->GetTimerManager().ClearTimer(PuzzleCompletionTimerHandle);

	PrimaryActorTick.SetTickFunctionEnable(bPuzzleIsActive);
}

void ARSPuzzleBase::ActivatePuzzle_Implementation()
{
	bPuzzleIsActive = true;
	check(GetWorld());
	ensure(!PrizeItemSoft.IsNull());
	PuzzleCompletionTime = 0.f;
	GetWorld()->GetTimerManager().SetTimer(PuzzleCompletionTimerHandle, this, &ARSPuzzleBase::OnPuzzleTimerElapsed, .1f, true);
	OnDifficultyChanged(URSGameUserSettingsFunctionLibrary::GetGameplayUserSettings().GameDifficulty);
	PrimaryActorTick.SetTickFunctionEnable(bPuzzleIsActive);
}

void ARSPuzzleBase::OnPrizeCollected_Implementation(ARSPickableItem* InItem)
{
	/*if (!IsValid(InItem))
		return;*/
	/*const ARSPickableItem* PrizeItem = Cast<ARSPuzzlePrizeBase>(PrizeItemSoft.Get());
	if (InItem != PrizeItem)
		return;*/
	CompletePuzzle();
}

void ARSPuzzleBase::ResetPuzzle()
{
	SetPuzzleState(FRSPuzzleState());
	PuzzleCompletionTime = 0.f;
	OnPuzzleReset.Broadcast();
}

void ARSPuzzleBase::CompletePuzzle()
{
	SetPuzzleState(FRSPuzzleState(true, PuzzleCompletionTime));
	DeactivatePuzzle();
	OnPuzzleCompleted.Broadcast();
	UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("Puzzle solved in %f seconds"), PuzzleCompletionTime), true, true, FLinearColor::Green, 10.f);
}

void ARSPuzzleBase::OnPuzzleTimerElapsed()
{
	PuzzleCompletionTime += .1f;
}

void ARSPuzzleBase::SetPuzzleState(const FRSPuzzleState& InNewState)
{
	CurrentPuzzleState = InNewState;
	if (CurrentPuzzleState.bWasSolved)
	{
		OnPuzzleSolved.Broadcast();
	}
}

void ARSPuzzleBase::ChangeBlockersAccessibility(bool bInUnlock, TArray<UPrimitiveComponent*> InBlockers)
{
	check(!InBlockers.IsEmpty())
	for(const auto blocker : InBlockers)
	{
		const FName newProfile = bInUnlock ? "NoCollision" : "BlockPawn";
		if (blocker->GetCollisionProfileName() != newProfile)
		{
			blocker->SetCollisionProfileName(newProfile, false);
		}
	}
}

void ARSPuzzleBase::OnDifficultyChanged(const ERSGameDifficulty InNewDifficulty)
{
	if(bPuzzleIsActive)
	{
		ResetPuzzle();
	}
	//this function is implemented in derived class
	//do not add any puzzle logic here, use only for registering/subsystem etc.
}

void ARSPuzzleBase::UpdateSolvingCondition(const FRSPuzzleSolvingCondition& InNewCondition)
{
	//this function is implemented in derived class
	//do not add any puzzle logic here, use only for registering/subsystem etc.
}

void ARSPuzzleBase::BeginPlay()
{
	Super::BeginPlay();

	URSGameUserSettings* settings = URSGameUserSettings::Get();
	if (!IsValid(settings))
	{
		checkNoEntry()
		return;
	}
	settings->OnDifficultyChanged.AddUniqueDynamic(this, &ARSPuzzleBase::OnDifficultyChanged);
}

void ARSPuzzleBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	GetWorld()->GetTimerManager().ClearTimer(PuzzleCompletionTimerHandle);

	URSGameUserSettings* settings = URSGameUserSettings::Get();
	if (IsValid(settings))
	{
		settings->OnDifficultyChanged.RemoveAll(this);
	}
	Super::EndPlay(EndPlayReason);
}

// Called every frame
void ARSPuzzleBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

