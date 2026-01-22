// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Items/RSPuzzlePrizeBase.h"
#include "RSPuzzleBase.generated.h"

//class URSListeningComponent;
class ARSPuzzlePrizeBase;

USTRUCT(BlueprintType)
struct FRSPuzzleState
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	bool bWasSolved = false;
	UPROPERTY(BlueprintReadWrite)
	float SolvingTime = 0.f;
};

USTRUCT(BlueprintType)
struct FRSPuzzleSolvingCondition
{
	GENERATED_BODY()
	
};

UCLASS()
class RESONANCEOFSILENCE_API ARSPuzzleBase : public AActor
{
	GENERATED_BODY()
	
public:	

	ARSPuzzleBase();

	UPROPERTY(BlueprintReadOnly, EditInstanceOnly)
	TSoftObjectPtr<ARSPuzzlePrizeBase> PrizeItemSoft;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ActivatePuzzle();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void DeactivatePuzzle();
	UPROPERTY(BlueprintReadOnly)
	bool bPuzzleIsActive;
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnPrizeCollected(ARSPickableItem* InItem);
	
	UPROPERTY(BlueprintAssignable)
	FRSDynamicMutlicastDelegate OnPuzzleReset;
	FRSDynamicMutlicastDelegate OnPuzzleSolved;
	UPROPERTY(BlueprintAssignable)
	FRSDynamicMutlicastDelegate OnPuzzleCompleted;
	
protected:	
	virtual void ResetPuzzle();
	void CompletePuzzle();

	FTimerHandle PuzzleCompletionTimerHandle;
	UPROPERTY(BlueprintReadOnly)
	float PuzzleCompletionTime = 0.f;
	void OnPuzzleTimerElapsed();

	UFUNCTION(BlueprintCallable)
	void SetPuzzleState(const FRSPuzzleState& InNewState);
	FRSPuzzleState GetPuzzleState() const { return CurrentPuzzleState; }

	UFUNCTION(BlueprintCallable)
	void ChangeBlockersAccessibility(bool bInUnlock, TArray<UPrimitiveComponent*> InBlockers);

	UPROPERTY(EditDefaultsOnly, Category="Diffculty")
	UDataTable* DifficultyDataTable;
	UPROPERTY(EditAnywhere, Category = "Diffculty")
	FDataTableRowHandle CurrentDifficultyRowHandle;
	UFUNCTION()
	virtual void OnDifficultyChanged(const ERSGameDifficulty InNewDifficulty);
	virtual void UpdateSolvingCondition(const FRSPuzzleSolvingCondition& InNewCondition);
	FRSPuzzleDifficultyBase PuzzleDifficultyBase;

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY(BlueprintReadOnly)
	FRSPuzzleState CurrentPuzzleState;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
