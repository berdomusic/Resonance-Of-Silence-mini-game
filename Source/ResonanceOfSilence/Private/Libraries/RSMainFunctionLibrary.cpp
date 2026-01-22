// Fill out your copyright notice in the Description page of Project Settings.


#include "Libraries/RSMainFunctionLibrary.h"
#include "Engine/AssetManager.h"
#include "Kismet/GameplayStatics.h"
#include "ResonanceOfSilence/RSPlayerCharacter.h"
#include "System/GameUserSettings/RSGameUserSettings.h"

bool URSMainFunctionLibrary::IsWorldType(const UObject* InWorldContextObject, const EWorldType::Type InWorldType)
{
	const UWorld* const World = InWorldContextObject ? InWorldContextObject->GetWorld() : nullptr;
	if (World)
	{
		return World->WorldType == InWorldType;
	}
	return false;
}

bool URSMainFunctionLibrary::IsInGameWorld(const UObject* InWorldContextObject)
{
	return
		IsWorldType(InWorldContextObject, EWorldType::PIE) ||
		IsWorldType(InWorldContextObject, EWorldType::Game);
}

URSLoadSoftArray_Async* URSLoadSoftArray_Async::LoadSoftArray_Async(TArray<TSoftObjectPtr<UObject>> InElementsToLoad, URSLoadSoftArray_Async*& OutRequestHandle)
{
	URSLoadSoftArray_Async* const action = NewObject<URSLoadSoftArray_Async>();
	OutRequestHandle = action;

	action->ElementsToLoad = InElementsToLoad;

	return action;
}

void URSLoadSoftArray_Async::Cancel()
{
	if (LastAsyncLoadHandle && LastAsyncLoadHandle->IsActive())
	{
		LastAsyncLoadHandle->CancelHandle();
	}
}

void URSLoadSoftArray_Async::Activate()
{
	TArray<FSoftObjectPath> pathsToLoad;
	for (const TSoftObjectPtr<UObject>& elementsToLoad : ElementsToLoad)
	{
		pathsToLoad.AddUnique(elementsToLoad.ToSoftObjectPath());
	}

	pathsToLoad.RemoveAll([](const FSoftObjectPath& InPath)
		{
			return InPath.IsNull();
		});

	if (!pathsToLoad.Num())
	{
		OnLoaded();
	}
	else
	{
		FStreamableManager& streamableManager = UAssetManager::Get().GetStreamableManager();
		FStreamableDelegate asyncdelegate = FStreamableDelegate::CreateUObject(this, &URSLoadSoftArray_Async::OnLoaded);

		if (LastAsyncLoadHandle && LastAsyncLoadHandle->IsActive())
		{
			LastAsyncLoadHandle->CancelHandle();
		}

		LastAsyncLoadHandle = streamableManager.RequestAsyncLoad(pathsToLoad, asyncdelegate);
	}
}

void URSLoadSoftArray_Async::OnLoaded()
{
	TArray<FSoftObjectPath> loadedPaths;

	for (const TSoftObjectPtr<UObject>& effectToLoad : ElementsToLoad)
	{
		loadedPaths.AddUnique(effectToLoad.ToSoftObjectPath());
	}

	const TArray<UObject*>& elements = GetLoadedElements(loadedPaths);
	OnSoftArrayLoaded.Broadcast(elements);
}

TArray<UObject*> URSLoadSoftArray_Async::GetLoadedElements(const TArray<FSoftObjectPath>& InPaths)
{
	TArray<UObject*> loadedElements;

	for (FSoftObjectPath path : InPaths)
	{
		UObject* resolved = Cast<UObject>(path.ResolveObject());
		if (IsValid(resolved))
		{
			loadedElements.AddUnique(resolved);
		}
	}

	return loadedElements;
}
