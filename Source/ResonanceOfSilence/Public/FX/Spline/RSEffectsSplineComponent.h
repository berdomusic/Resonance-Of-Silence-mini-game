// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SplineComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "RSEffectsSplineComponent.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class RESONANCEOFSILENCE_API URSEffectsSplineComponent : public USplineComponent
{
	GENERATED_BODY()
public:

	virtual void ActivateVFX();
	virtual void DeactivateVFX();

	UPROPERTY(BlueprintAssignable)
	FRSDynamicMulticastBoolDelegate OnVFXActivated;
};
