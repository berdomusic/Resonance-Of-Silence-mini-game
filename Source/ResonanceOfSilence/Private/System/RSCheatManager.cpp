// Fill out your copyright notice in the Description page of Project Settings.


#include "System/RSCheatManager.h"
#include "System/GameUserSettings/RSGameUserSettings.h"

ERSPuzzleHelper URSCheatManager::CurrentPuzzleHelperState = ERSPuzzleHelper::Disabled;

URSCheatManager* URSCheatManager::GetCheatManager(const UObject* InWorldContext)
{
	ARSPlayerController* pc = URSCharacterFunctionLibrary::GetPlayerController(InWorldContext);
	return pc ? Cast<URSCheatManager>(pc -> CheatManager) : nullptr;
}

void URSCheatManager::SetGameDifficulty(ERSGameDifficulty NewDifficulty)
{
	if (URSGameUserSettings* Settings = URSGameUserSettings::Get())
	{
		FRSGameplayUserSettings newSettings = URSGameUserSettingsFunctionLibrary::GetGameplayUserSettings();
		newSettings.GameDifficulty = NewDifficulty;
		Settings->ApplyGameplaySettings(newSettings);
	}
}

void URSCheatManager::SetPuzzleHelper(ERSPuzzleHelper InNewState) const
{
	CurrentPuzzleHelperState = InNewState;
	OnPuzzleHelperStateChange.Broadcast(CurrentPuzzleHelperState);
}

void URSCheatManager::InitCheatManager()
{
	Super::InitCheatManager();
	AutoCompleteEntriesHandle = UConsole::RegisterConsoleAutoCompleteEntries.AddStatic(&URSCheatManager::PopulateAutoCompleteEntries);
}

void URSCheatManager::PopulateAutoCompleteEntries(TArray<FAutoCompleteCommand>& AutoCompleteList)
{
	const UEnum* EnumPtrDifficulty = StaticEnum<ERSGameDifficulty>();
	AddEnumDebug(AutoCompleteList, "SetGameDifficulty", "Set difficulty to", FColor::Green, EnumPtrDifficulty);
	const UEnum* EnumPtrPuzzleHelper = StaticEnum<ERSPuzzleHelper>();
	AddEnumDebug(AutoCompleteList, "SetPuzzleHelper", "Set Puzzle Helper", FColor::Green, EnumPtrPuzzleHelper);
}

void URSCheatManager::AddEnumDebug(TArray<FAutoCompleteCommand>& InAutoCompleteList, const FString& InFunctionName, const FString& InDescription, FColor InColor, const UEnum* InEnum)
{
	check(InEnum);

	for (int32 i = 0; i < InEnum->NumEnums(); ++i)
	{
		if (InEnum->HasMetaData(TEXT("Hidden"), i))
			continue;

		const FString EnumName = InEnum->GetNameStringByIndex(i);
		const FString CommandString = InFunctionName + TEXT(" ") + EnumName;

		const bool bAlreadyExists = InAutoCompleteList.ContainsByPredicate(
			[&CommandString](const FAutoCompleteCommand& Existing)
			{
				return Existing.Command.Equals(CommandString, ESearchCase::CaseSensitive);
			});

		if (bAlreadyExists)
			continue;

		FAutoCompleteCommand AutoCommand;
		AutoCommand.Command = CommandString;
		AutoCommand.Desc = InDescription;
		AutoCommand.Color = InColor;

		InAutoCompleteList.Add(AutoCommand);
	}
}