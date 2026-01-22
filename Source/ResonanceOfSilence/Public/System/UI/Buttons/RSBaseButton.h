// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "RSBaseButton.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnClickedExtended, URSBaseButton*, InButton);

UCLASS()
class RESONANCEOFSILENCE_API URSBaseButton : public UButton
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, Category = "Extended delegate")
	bool bAddExtendedDelegate;
	UFUNCTION()
	void CallOnClickedExtended();
	
	virtual void SynchronizeProperties() override;

	UPROPERTY(BlueprintAssignable)
	FOnClickedExtended OnClickedExtended;
	
};
