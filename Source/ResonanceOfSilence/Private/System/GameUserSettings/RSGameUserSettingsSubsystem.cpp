// Fill out your copyright notice in the Description page of Project Settings.


#include "System/GameUserSettings/RSGameUserSettingsSubsystem.h"

URSGameUserSettingsSubsystem* URSGameUserSettingsSubsystem::Get(const UObject* InWorldContext)
{
	if(IsValid(InWorldContext))
	{
		const UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(InWorldContext);
		if (IsValid(GameInstance))
		{
			return GameInstance->GetSubsystem<URSGameUserSettingsSubsystem>();
		}
	}
	return nullptr;
}

