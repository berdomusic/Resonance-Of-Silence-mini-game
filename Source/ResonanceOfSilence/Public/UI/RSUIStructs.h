// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RSUIStructs.generated.h"

class URSTooltipEntry;

UENUM(BlueprintType)
enum class ERSTooltipPosition : uint8
{
	RightUp		UMETA(DisplayName = "RightUp"),
	LeftUp		UMETA(DisplayName = "LeftUp")
};

USTRUCT(BlueprintType)
struct FRSUITooltipHandle : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<URSTooltipEntry> WidgetClass;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	ERSTooltipPosition TooltipPosition = ERSTooltipPosition::RightUp;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector2D FadeInOut = FVector2D::ZeroVector;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Lifetime;

};