// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RSInteractionWidget.h"

void URSInteractionWidget::GetInteractionText(FText& OutText) const
{
	OutText = FText::FromName(InteractionDisplayName);
}

void URSInteractionWidget::SetInteractionText(FName InNewName)
{
	InteractionDisplayName = InNewName;
}
