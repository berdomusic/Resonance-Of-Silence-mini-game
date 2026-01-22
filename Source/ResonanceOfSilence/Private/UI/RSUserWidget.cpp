// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RSUserWidget.h"

#include "System/UI/RSUISubsystem.h"

void URSUserWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (const UGameInstance* GI = GetGameInstance())
    {
        if (auto* Subsystem = GI->GetSubsystem<URSUISubsystem>())
        {
            Subsystem->RegisterUserWidget(this);
        }
    }
}

void URSUserWidget::NativeDestruct()
{
    if (const UGameInstance* GI = GetGameInstance())
    {
        if (auto* Subsystem = GI->GetSubsystem<URSUISubsystem>())
        {
            Subsystem->UnregisterUserWidget(this);
        }
    }

    Super::NativeDestruct();
}


void URSUserWidget::FadeInWidget()
{
}

void URSUserWidget::FadeOutWidget()
{
}
