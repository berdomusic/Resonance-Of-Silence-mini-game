// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RSUserWidget.h"
#include "Blueprint/UserWidget.h"
#include "RSMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class RESONANCEOFSILENCE_API URSMenuWidget : public URSUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UFUNCTION(BlueprintCallable)
	void OnNewGameClicked();
	UFUNCTION(BlueprintCallable)
	void OnExitGameClicked();
};
