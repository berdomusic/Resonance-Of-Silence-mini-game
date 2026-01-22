// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/RSPickableItem.h"
#include "FX/Spline/RSJumpingVFXSpline.h"
#include "RSSoundChargedCrystal.generated.h"

class URSAudioComponent;
class ARSFrequencyPuzzle;

UCLASS()
class RESONANCEOFSILENCE_API ARSSoundChargedCrystal : public ARSPickableItem
{
	GENERATED_BODY()

public:
	ARSSoundChargedCrystal();

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	URSAudioComponent* AudioComponent;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	URSJumpingVFXSpline* EffectsSpline;

	void FrequencyPuzzleSetup(bool bInBind, ARSFrequencyPuzzle *InPuzzle);
	void SetupEffectsSpline(const TArray<FVector> &InTargetPoints) const;

	UFUNCTION(BlueprintImplementableEvent)
	void BP_AddItemHUD(ARSPickableItem *InItem);
	UFUNCTION(BlueprintImplementableEvent)
	void BP_RemoveItemHUD(ARSPickableItem *InItem);
	
	virtual void PickUp(ARSCharacter* InInstigator) override;

	UFUNCTION(BlueprintCallable)
	void InitVFXSplineSetup(const FVector& InInitTargetLocation);

	UFUNCTION(BlueprintNativeEvent)
	void OnChargeStarted();
	UFUNCTION(BlueprintNativeEvent)
	void OnChargeStopped();
	UFUNCTION(BlueprintNativeEvent)
	void OnChargeTick();
	UFUNCTION()
	void OnPuzzleSolved();
	UFUNCTION(BlueprintCallable)
	void SetMaxChargeValue(float InNewValue) { MaxChargeValue = InNewValue; }
	FORCEINLINE bool IsFullyCharged() const { return FMath::IsNearlyEqual(CurrentChargeValue, MaxChargeValue); }
	FORCEINLINE bool IsChargeDepleted() const { return FMath::IsNearlyZero(CurrentChargeValue); }
	bool CanBeCharged();

protected:
	FTimerHandle ChargeTimerHandle;
	UPROPERTY(BlueprintReadOnly)
	float CurrentChargeValue = 0.f;
	UPROPERTY(BlueprintReadOnly)
	float MaxChargeValue = 5.f;

public:
	UFUNCTION(BlueprintCallable)
	void TryPlaySound();
	UFUNCTION(BlueprintNativeEvent)
	void PlaySound();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void StopSound();
	UPROPERTY(BlueprintReadOnly)
	FRSTransmittedSoundData CurrentTransmittedSoundData;

	UPROPERTY(BlueprintAssignable)
	FRSTransmittedSoundDataDelegate OnSoundTransmitted;
	UPROPERTY(BlueprintAssignable)
	FRSTransmittedSoundDataDelegate OnSoundDataUpdate;
	UFUNCTION(BlueprintCallable)
	void UpdateTransmittedSoundData(const FRSTransmittedSoundData &InNewData);
	FORCEINLINE bool CanUpdateSoundData() const { return !bIsPlaying; }

	UFUNCTION()
	void SetPlayable(bool bInActive);
	UPROPERTY(BlueprintAssignable)
	FRSDynamicMulticastBoolDelegate OnActivationStateChanged;

	UPROPERTY(BlueprintAssignable)
	FRSDynamicMutlicastDelegate OnChargeFullyDepleted;
	UPROPERTY(BlueprintAssignable)
	FRSDynamicMutlicastDelegate OnMaxChargeValueReached;

private:
	bool bIsPlaying;
	bool CanPlaySound();

	bool bIsActivated;

	bool bIsBeingCharged;
	bool bWasPreviouslyCharged;
	void UpdateChargeValue(float InDelta);

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void FinishDroppingItem() override;
	virtual void OnPuzzleReset() override;
};
