// Fill out your copyright notice in the Description page of Project Settings.


#include "System/GameReferences/RSGameReferencesSettings.h"

const URSGameReferencesSettings& URSGameReferencesSettings::Get()
{
	return *CastChecked<URSGameReferencesSettings>(StaticClass()->GetDefaultObject());
}

const URSGameReferencesSettings* URSGameReferencesSettings::K2_Get()
{
	return &Get();
}
