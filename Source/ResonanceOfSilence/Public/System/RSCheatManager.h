// Fill out your copyright notice in the Description page of Project Settings.

#pragma once



#include "CoreMinimal.h"
#include "GameFramework/CheatManager.h"
#include "Engine/Console.h"
#include "RSCheatManager.generated.h"

UENUM(BlueprintType)
enum class ERSPuzzleHelper : uint8
{    
    Disabled	UMETA(DisplayName = "Disabled"),
    Enabled		UMETA(DisplayName = "Enabled"),

    MAX   UMETA(Hidden)
};

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRSPuzzleHelperState, ERSPuzzleHelper, InPuzzleHelperState);

UCLASS()
class RESONANCEOFSILENCE_API URSCheatManager : public UCheatManager
{
	GENERATED_BODY()
public:
    UFUNCTION(BlueprintPure, meta = (WorldContext = "InWorldContext"))
    static URSCheatManager* GetCheatManager(const UObject* InWorldContext);
    
    UFUNCTION(exec)
	static void SetGameDifficulty(ERSGameDifficulty NewDifficulty);
    
    static ERSPuzzleHelper CurrentPuzzleHelperState;
    UFUNCTION(exec)
    void SetPuzzleHelper(ERSPuzzleHelper InNewState) const;
    UFUNCTION(BlueprintPure)
    static ERSPuzzleHelper GetPuzzleHelper() { return CurrentPuzzleHelperState; };
    UPROPERTY(BlueprintAssignable)
    FRSPuzzleHelperState OnPuzzleHelperStateChange;
    

	virtual void InitCheatManager() override;
    static void PopulateAutoCompleteEntries(TArray<FAutoCompleteCommand>& AutoCompleteList);
    FDelegateHandle AutoCompleteEntriesHandle;

    static void AddEnumDebug(TArray<FAutoCompleteCommand>& InAutoCompleteList, const FString& InFunctionName, const FString& InDescription, FColor InColor, const UEnum* InEnum);

};

template <typename TEnum>
static void AddEnumAutoCompleteCommands(UConsole* Console, const FString& CommandPrefix, const FString& DescriptionPrefix)
{
    if (!Console)
        return;

    const UEnum* EnumPtr = StaticEnum<TEnum>();
    if (!EnumPtr)
    {
        UE_LOG(LogTemp, Warning, TEXT("AddEnumAutoCompleteCommands: Invalid enum type."));
        return;
    }

    const int32 EnumCount = EnumPtr->NumEnums();
    for (int32 i = 0; i < EnumCount; ++i)
    {
        if (EnumPtr->HasMetaData(TEXT("Hidden"), i))
            continue;

        const FString EnumName = EnumPtr->GetNameStringByIndex(i);
        const FString Command = FString::Printf(TEXT("%s %s"), *CommandPrefix, *EnumName);
        const FString Desc = FString::Printf(TEXT("%s %s"), *DescriptionPrefix, *EnumName);

        Console->AutoCompleteList.Add({ Command, Desc });
    }
}