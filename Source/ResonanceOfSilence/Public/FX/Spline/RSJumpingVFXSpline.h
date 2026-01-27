// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FX/Spline/RSEffectsSplineComponent.h"
#include "RSJumpingVFXSpline.generated.h"

/**
 * 
 */
UCLASS()
class RESONANCEOFSILENCE_API URSJumpingVFXSpline : public URSEffectsSplineComponent
{
	GENERATED_BODY()

	URSJumpingVFXSpline();
public:
	
	void SetupJumpingSpline(const TArray<FVector>& InTargetPoints);	
	
	bool bCanJump = true;

	virtual void DeactivateVFX() override;

	void OnChargeStarted();
	void OnCrystalPlayableSet(bool bInPlayable);
	void OnPuzzleReset();
	void OnChargeDepleted();

protected:
	UFUNCTION()
	void OnOwnerDropped(ARSPickableItem* InItem);
	UPROPERTY(EditAnywhere, Category=Setup)
	bool bManualPointsSetup;
	UPROPERTY(EditAnywhere, Category = Setup)
	int NumberOfTargetPoints = 3;
	UPROPERTY(EditAnywhere, Category = Setup)
	FVector PreTargetOffset = FVector(0,0,100.f);

	UPROPERTY(BlueprintReadOnly)
	FVector CurrentStart;
	UPROPERTY(BlueprintReadOnly)
	FVector CurrentTarget;
	UPROPERTY(BlueprintReadOnly)
	TArray<FVector> TargetPoints;

	const float LerpSpeedUnitsPerSecond = 750.f;
	float CurrentLerpDistance;
	float CurrentLerpValue;
	bool bLerpVectors;
	
	UFUNCTION(BlueprintCallable)
	void SetCurrentStepIdx(int InIdx);
	int CurrentStepIdx;

	void SetNewPositionsForSplinePoints();
	void PositionSplinePoints(const FVector& InVector);
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};
