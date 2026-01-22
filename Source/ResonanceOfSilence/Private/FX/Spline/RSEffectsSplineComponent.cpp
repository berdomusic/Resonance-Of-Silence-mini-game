// Fill out your copyright notice in the Description page of Project Settings.


#include "FX/Spline/RSEffectsSplineComponent.h"

void URSEffectsSplineComponent::ActivateVFX()
{
	OnVFXActivated.Broadcast(true);
}

void URSEffectsSplineComponent::DeactivateVFX()
{
	OnVFXActivated.Broadcast(false);
}