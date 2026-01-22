// Fill out your copyright notice in the Description page of Project Settings.


#include "Libraries/RSCharacterFunctionLibrary.h"

bool URSCharacterFunctionLibrary::IsActorPlayerCharacter(const AActor* InActor)
{
	if (!IsValid(InActor))
		return false;
	return InActor->IsA<ARSPlayerCharacter>();
}

ARSPlayerController* URSCharacterFunctionLibrary::GetPlayerController(const UObject* InWorldContext)
{
	if (IsValid(InWorldContext))
	{
		ARSPlayerController* playerController = Cast<ARSPlayerController>(UGameplayStatics::GetPlayerController(InWorldContext, 0));
		if (IsValid(playerController))
		{
			return playerController;
		}
	}
	return nullptr;
}

ARSPlayerCharacter* URSCharacterFunctionLibrary::GetPlayerCharacter(const UObject* InWorldContext)
{
	if (IsValid(InWorldContext))
	{
		ARSPlayerCharacter* playerCharacter = Cast<ARSPlayerCharacter>(UGameplayStatics::GetPlayerCharacter(InWorldContext, 0));
		if (IsValid(playerCharacter))
		{
			return playerCharacter;
		}
	}
	return nullptr;
}
