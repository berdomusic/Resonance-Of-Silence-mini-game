// Fill out your copyright notice in the Description page of Project Settings.


#include "System/RoSInterfaces.h"

void ILookAtInterface::OnLookAtStarted_Implementation()
{
}

void ILookAtInterface::OnLookAtFinished_Implementation()
{
}

float ILookAtInterface::GetMaxDetectionDistance_Implementation() const
{
	return 1000.f;
}

void ILookAtInterface::OnBeginInteraction_Implementation(AController* InteractionInstigator)
{
}

void ILookAtInterface::OnEndInteraction_Implementation(AController* InteractionInstigator)
{
}