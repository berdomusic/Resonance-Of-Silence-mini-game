// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RSUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class RESONANCEOFSILENCE_API URSUserWidget : public UUserWidget
{
	GENERATED_BODY()
public:

	FORCEINLINE void SetFadeInOutTime(const FVector2D& InNewTime) { FadeInOutTime = InNewTime; };
	FORCEINLINE bool IsWidgetFading() { return FadeInOutTime != FVector2D::ZeroVector; }

protected:

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	FVector2D FadeInOutTime = FVector2D::ZeroVector;

	

	UFUNCTION(BlueprintCallable)
	void FadeInWidget();
	UFUNCTION(BlueprintCallable)
	void FadeOutWidget();

	
};
