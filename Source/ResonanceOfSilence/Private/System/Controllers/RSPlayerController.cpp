// Fill out your copyright notice in the Description page of Project Settings.


#include "System/Controllers/RSPlayerController.h"

ARSPlayerController::ARSPlayerController()
{
	CheatClass = URSCheatManager::StaticClass();
	SetTickableWhenPaused(true);
}