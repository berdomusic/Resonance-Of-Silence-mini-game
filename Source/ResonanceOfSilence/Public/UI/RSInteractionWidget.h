// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RSInteractionWidget.generated.h"

/**
 * 
 */
UCLASS()
class RESONANCEOFSILENCE_API URSInteractionWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintPure)
	void GetInteractionText(FText& OutText) const;
	void SetInteractionText(FName InNewName);

protected:
	FName InteractionDisplayName = "Interactable";
};
