// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RoSInterfaces.generated.h"

/**
 * 
 */
UINTERFACE(BlueprintType)
class ULookAtInterface : public UInterface
{
	GENERATED_BODY()
};

class RESONANCEOFSILENCE_API ILookAtInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, Category = "LookAt")
	void OnLookAtStarted();
	virtual void OnLookAtStarted_Implementation();
	UFUNCTION(BlueprintNativeEvent, Category = "LookAt")
	void OnLookAtFinished();
	virtual void OnLookAtFinished_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "LookAt")
	float GetMaxInteractionDistance() const;
	virtual float GetMaxDetectionDistance_Implementation() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "LookAt")
	void OnBeginInteraction(AController* InteractionInstigator);
	virtual void OnBeginInteraction_Implementation(AController* InteractionInstigator);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "LookAt")
	void OnEndInteraction(AController* InteractionInstigator);
	virtual void OnEndInteraction_Implementation(AController* InteractionInstigator);
};