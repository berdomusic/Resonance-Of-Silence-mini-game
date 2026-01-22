// Fill out your copyright notice in the Description page of Project Settings.


#include "System/UI/Buttons/RSBaseButton.h"

void URSBaseButton::CallOnClickedExtended()
{
	OnClickedExtended.Broadcast(this);
}

void URSBaseButton::SynchronizeProperties()
{
	Super::SynchronizeProperties();
	if(bAddExtendedDelegate)
	{
		OnClicked.AddUniqueDynamic(this, &URSBaseButton::CallOnClickedExtended);
	}	
}
