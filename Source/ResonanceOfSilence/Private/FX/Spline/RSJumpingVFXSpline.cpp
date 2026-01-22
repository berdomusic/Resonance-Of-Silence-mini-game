// Fill out your copyright notice in the Description page of Project Settings.


#include "FX/Spline/RSJumpingVFXSpline.h"

#include "Items/RSSoundChargedCrystal.h"

URSJumpingVFXSpline::URSJumpingVFXSpline()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;

	ClearSplinePoints();
	AddSplinePoint(FVector::ZeroVector, ESplineCoordinateSpace::Local);
	AddSplinePoint(FVector(100.f, 0.f, 100.f), ESplineCoordinateSpace::Local);
	AddSplinePoint(FVector(100.f, 0.f, 0.f), ESplineCoordinateSpace::Local);
}

void URSJumpingVFXSpline::InitSetupJumpingSpline(const FVector& InInitTargetLocation)
{
	CurrentTarget = InInitTargetLocation;
}

void URSJumpingVFXSpline::SetupJumpingSpline(const TArray<FVector>& InTargetPoints)
{
	SetComponentTickEnabled(true);
	if (bManualPointsSetup) return;

	check(InTargetPoints.Num() == NumberOfTargetPoints)
	TargetPoints.Empty();
	TargetPoints = InTargetPoints;
	SetNewPositionsForSplinePoints();	
	
	ActivateVFX();

	ARSSoundChargedCrystal* Crystal = Cast<ARSSoundChargedCrystal>(GetOwner());
	if (!IsValid(Crystal)) return;
	Crystal->OnItemDroppedFinish.AddUniqueDynamic(this, &URSJumpingVFXSpline::OnOwnerDropped);
}

void URSJumpingVFXSpline::OnChargeStarted()
{
	SetCurrentStepIdx(1);
}

void URSJumpingVFXSpline::OnCrystalPlayableSet(bool bInPlayable)
{
	ARSSoundChargedCrystal* Crystal = Cast<ARSSoundChargedCrystal>(GetOwner());
	check(IsValid(Crystal))
	if(Crystal->IsChargeDepleted())
	{
		SetCurrentStepIdx(0);
		return;
	}
	SetCurrentStepIdx(bInPlayable ? 2 : 1);
}

void URSJumpingVFXSpline::OnPuzzleReset()
{
	SetCurrentStepIdx(0);
}

void URSJumpingVFXSpline::OnChargeDepleted()
{
	SetCurrentStepIdx(0);
}

void URSJumpingVFXSpline::OnOwnerDropped(ARSPickableItem* InItem)
{
	DeactivateVFX();
	SetComponentTickEnabled(false);
}

void URSJumpingVFXSpline::SetCurrentStepIdx(int InIdx)
{
	if (!bCanJump)
		return;
	if(CurrentStepIdx == InIdx)
		return;
	CurrentStepIdx = InIdx;
	if (TargetPoints.IsEmpty())
	{
		checkNoEntry()
			return;
	}	
	SetNewPositionsForSplinePoints();
}

void URSJumpingVFXSpline::SetNewPositionsForSplinePoints()
{	
	if(TargetPoints.IsEmpty())
	{
		checkNoEntry()
			return;
	}
	CurrentStart = CurrentTarget;
	CurrentTarget = TargetPoints[CurrentStepIdx];

	CurrentLerp = 0.f;
	bLerpVectors = true;
}

void URSJumpingVFXSpline::PositionSplinePoints(const FVector& InVector)
{
	SetLocationAtSplinePoint(0, GetOwner()->GetActorLocation(), ESplineCoordinateSpace::World);
	SetLocationAtSplinePoint(TargetPoints.Num() - 2, InVector + PreTargetOffset, ESplineCoordinateSpace::World);
	SetLocationAtSplinePoint(TargetPoints.Num() - 1, InVector, ESplineCoordinateSpace::World);
}

void URSJumpingVFXSpline::DeactivateVFX()
{
	Super::DeactivateVFX();
	ARSSoundChargedCrystal* Crystal = Cast<ARSSoundChargedCrystal>(GetOwner());
	if (IsValid(Crystal))
	{
		Crystal->OnItemDroppedFinish.RemoveDynamic(this, &URSJumpingVFXSpline::OnOwnerDropped);		
	}
}

void URSJumpingVFXSpline::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if(bLerpVectors)
	{
		CurrentLerp += DeltaTime / 2.0f;
		CurrentLerp = FMath::Clamp(CurrentLerp, 0.0f, 1.0f);
		
		const FVector& currentLerpVector = UKismetMathLibrary::VLerp(CurrentStart, CurrentTarget, CurrentLerp);
		bLerpVectors = CurrentLerp < 1.f;
		PositionSplinePoints(currentLerpVector);
	}
	else
	{
		PositionSplinePoints(CurrentTarget);
	}	
}

void URSJumpingVFXSpline::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	DeactivateVFX();
	Super::EndPlay(EndPlayReason);
}
