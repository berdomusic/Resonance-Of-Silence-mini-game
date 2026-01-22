// Copyright Epic Games, Inc. All Rights Reserved.

#include "ResonanceOfSilenceGameMode.h"
#include "RSPlayerCharacter.h"
#include "System/Controllers/RSPlayerController.h"
#include "UObject/ConstructorHelpers.h"

AResonanceOfSilenceGameMode::AResonanceOfSilenceGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
	PlayerControllerClass = ARSPlayerController::StaticClass();
}
