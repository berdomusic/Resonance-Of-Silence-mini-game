// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Puzzles/RSPuzzleBase.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Components/AudioComponent.h"
#include "Items/RSSoundChargedCrystal.h"
#include "RSFrequencyPuzzle.generated.h"

USTRUCT(BlueprintType)
struct FRSFrequencyPuzzleSolvingCondition : public FRSPuzzleSolvingCondition
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int RequiredFrequency = 440.f;
};

USTRUCT(BlueprintType)
struct FRSFrequencyPuzzleDifficulty : public FRSPuzzleDifficultyBase
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Frequency")
	FVector2D RequiredFrequencyRange = { 220.f, 1000.f };
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Frequency")
	int MatchingFrequencyTolerance = 10;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Charging")
	float ChargingTime = 5.f;
};

UCLASS()
class RESONANCEOFSILENCE_API ARSFrequencyPuzzle : public ARSPuzzleBase
{
	GENERATED_BODY()

	ARSFrequencyPuzzle();
public:

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	TArray<TSoftObjectPtr<ARSSoundChargedCrystal>> AssociatedCrystalsSoft;
	UPROPERTY(BlueprintReadOnly)
	bool bPlayerHasCrystalEquipped;

protected:	

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UStaticMeshComponent* ChargingUpSphere;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	USphereComponent* ChargingUpSphereCollision;
	bool bPlayerOverlapsChargingSphere;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UAudioComponent* ChargingSphereAudioComponent;	
	bool bPlayerOverlapsActivationSphere;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UStaticMeshComponent* CrystalActivationSphere;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	USphereComponent* CrystalActivationSphereCollision;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UAudioComponent* CrystalActivationAudioComponent;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UBoxComponent* BlockingBox;

	UFUNCTION(BlueprintCallable)
	void OnChargingCrystalPickup(ARSPickableItem* InCrystal);
	UFUNCTION(BlueprintCallable)
	void OnChargingCrystalDropped(ARSPickableItem* InCrystal);

	bool CanCrystalBeCharged() const;
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void StartCharging();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void StopCharging();
	UFUNCTION()
	void ChargeTick();
public:
	UPROPERTY(BlueprintAssignable)
	FRSDynamicMutlicastDelegate OnChargingStarted;
	FRSDynamicMutlicastDelegate OnChargingStopped;
	FRSDynamicMutlicastDelegate OnChargingTick;
	FRSDynamicMulticastFloatDelegate OnChargeTimeUpdate;

protected:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void OpenDoors();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void CloseDoors();

	UPROPERTY(BlueprintReadOnly)
	bool bIsCharging;
	UFUNCTION(BlueprintCallable) // this is exposed because of blueprint binding to all of crystals events
	void OnCrystalMaxCharged();
	bool bCrystalMaxCharged;
	FTimerHandle ChargingTimerHandle;

	UFUNCTION()
	void OnChargeSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnChargeSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex);
	UFUNCTION(BlueprintNativeEvent)
	void OnCrystalActivationSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION(BlueprintNativeEvent)
	void OnCrystalActivationSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex);

public:
	UPROPERTY(BlueprintAssignable)
	FRSDynamicMulticastBoolDelegate OnSetCrystalPlayable;

protected:

	UFUNCTION(BlueprintNativeEvent)// this is exposed because of blueprint binding to all of crystals events
	void OnReceivedTransmittedSound(const FRSTransmittedSoundData& InData);

	UPROPERTY(BlueprintReadOnly)
	FRSFrequencyPuzzleSolvingCondition CurrentSolvingCondition;
	UFUNCTION(BlueprintCallable) // this is exposed because of blueprint binding to all of crystals events
	void SetNewSolvingConditionFromCurrentDifficulty();
	FRSFrequencyPuzzleSolvingCondition MakeNewSolvingConditionFromDifficultyPreset(const FRSFrequencyPuzzleDifficulty& InDifficulty) const;
	virtual void UpdateSolvingCondition(const FRSPuzzleSolvingCondition& InNewCondition) override;
	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnSolvingConditionUpdate();

	UPROPERTY(BlueprintReadOnly)
	FRSFrequencyPuzzleDifficulty CurrentPuzzleDifficulty;
public:
	FORCEINLINE FRSFrequencyPuzzleDifficulty GetPuzzleDifficulty() { return CurrentPuzzleDifficulty; }
protected:
	virtual void OnDifficultyChanged(const ERSGameDifficulty InNewDifficulty) override;
	virtual void ResetPuzzle() override;
	virtual void OnPrizeCollected_Implementation(ARSPickableItem* InItem) override;

	virtual void BeginPlay() override;	
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	void BindDelegates(bool bBind);
};
