// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
//#include "System/Controllers/RSPlayerController.h"
#include "Engine/StreamableManager.h"
#include "RSPlayerCharacter.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "RSMainFunctionLibrary.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnArrayLoaded_Del, const TArray<UObject*>&, OutLoadedArray);

UCLASS()
class RESONANCEOFSILENCE_API URSMainFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	static bool IsWorldType(const UObject* InWorldContextObject, const EWorldType::Type InWorldType);
	UFUNCTION(BlueprintPure)
	static bool IsInGameWorld(const UObject* InWorldContextObject);

};

template<typename T>
FDataTableRowHandle GetHandleForDifficulty(UDataTable* Table, ERSGameDifficulty Difficulty)
{
	static_assert(TIsDerivedFrom<T, FRSPuzzleDifficultyBase>::IsDerived,
		"T must derive from FRSPuzzleDifficultyBase");

	if (!Table)
	{
		UE_LOG(LogTemp, Warning, TEXT("GetHandleForDifficulty: Table is null!"));
		return FDataTableRowHandle();
	}

	for (auto& RowPair : Table->GetRowMap())
	{
		FName RowName = RowPair.Key;
		T* Row = Table->FindRow<T>(RowName, TEXT("GetHandleForDifficulty"), true);
		if (Row && Row->AssociatedGameDifficulty == Difficulty)
		{
			FDataTableRowHandle Handle;
			Handle.DataTable = Table;
			Handle.RowName = RowName;
			return Handle;
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("No row found for difficulty %d in table %s"),
		static_cast<uint8>(Difficulty), *Table->GetName());
	return FDataTableRowHandle();
}

template<typename T>
T* GetRowFromHandle(const FDataTableRowHandle& Handle)
{
	if (!Handle.DataTable)
	{
		UE_LOG(LogTemp, Warning, TEXT("GetRowFromHandle: No DataTable in handle!"));
		return nullptr;
	}

	T* Row = Handle.DataTable->FindRow<T>(Handle.RowName, TEXT(""), true);
	if (!Row)
	{
		UE_LOG(LogTemp, Warning, TEXT("GetRowFromHandle: Row '%s' not found in %s"),
			*Handle.RowName.ToString(),
			*Handle.DataTable->GetName());
	}

	return Row;
}

UCLASS(BlueprintType)
class RESONANCEOFSILENCE_API URSLoadSoftArray_Async : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable)
	FOnArrayLoaded_Del OnSoftArrayLoaded;

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "InWorldContextObject"))
	static URSLoadSoftArray_Async* LoadSoftArray_Async(TArray<TSoftObjectPtr<UObject>> InElementsToLoad, URSLoadSoftArray_Async*& OutRequestHandle);

	UFUNCTION(BlueprintCallable)
	void Cancel();

	virtual void Activate() override;

protected:

	UFUNCTION()
	void OnLoaded();

	TArray<UObject*> GetLoadedElements(const TArray<FSoftObjectPath>& InPaths);

	TArray<TSoftObjectPtr<UObject>> ElementsToLoad = {};
	TSharedPtr<FStreamableHandle> LastAsyncLoadHandle;
};