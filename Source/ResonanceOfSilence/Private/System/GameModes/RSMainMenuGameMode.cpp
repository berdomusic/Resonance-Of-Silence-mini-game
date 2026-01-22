// Fill out your copyright notice in the Description page of Project Settings.


#include "System/GameModes/RSMainMenuGameMode.h"
#include "GameFramework/DefaultPawn.h"

ARSMainMenuGameMode::ARSMainMenuGameMode()
{
	DefaultPawnClass = ADefaultPawn::StaticClass();
	PlayerControllerClass = ARSPlayerController::StaticClass();
}
