// Fill out your copyright notice in the Description page of Project Settings.

#include "Items/RSSoundChargedCrystal.h"
#include "Audio/Components/RSAudioComponent.h"
#include "Actors/Puzzles/RSFrequencyPuzzle.h"

void ARSSoundChargedCrystal::OnChargeStarted_Implementation()
{
	if (!CanBeCharged())
		return;
	bIsBeingCharged = true;
	EffectsSpline->OnChargeStarted();
}

void ARSSoundChargedCrystal::OnChargeStopped_Implementation()
{
	bIsBeingCharged = false;
}

void ARSSoundChargedCrystal::OnChargeTick_Implementation()
{
}

ARSSoundChargedCrystal::ARSSoundChargedCrystal()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;

	AudioComponent = CreateDefaultSubobject<URSAudioComponent>(TEXT("AudioComponent"));
	AudioComponent->SetupAttachment(RootComponent);
	AudioComponent->bAutoActivate = false;

	EffectsSpline = CreateDefaultSubobject<URSJumpingVFXSpline>(TEXT("EffectsSpline"));
	EffectsSpline->SetupAttachment(RootComponent);
}

void ARSSoundChargedCrystal::FrequencyPuzzleSetup(bool bInBind, ARSFrequencyPuzzle* InPuzzle)
{
	if (!IsValid(InPuzzle))
	{
		checkNoEntry()
			return;
	}
	if(bInBind)
	{
		InPuzzle->OnChargingStarted.AddUniqueDynamic(this, &ARSSoundChargedCrystal::OnChargeStarted);
		InPuzzle->OnChargingStopped.AddUniqueDynamic(this, &ARSSoundChargedCrystal::OnChargeStopped);
		InPuzzle->OnChargingTick.AddUniqueDynamic(this, &ARSSoundChargedCrystal::OnChargeTick);
		InPuzzle->OnSetCrystalPlayable.AddUniqueDynamic(this, &ARSSoundChargedCrystal::SetPlayable);
		InPuzzle->OnChargeTimeUpdate.AddUniqueDynamic(this, &ARSSoundChargedCrystal::SetMaxChargeValue);
		InPuzzle->OnPuzzleReset.AddUniqueDynamic(this, &ARSSoundChargedCrystal::OnPuzzleReset);
		InPuzzle->OnPuzzleSolved.AddUniqueDynamic(this, &ARSSoundChargedCrystal::OnPuzzleSolved);

		SetMaxChargeValue(InPuzzle->GetPuzzleDifficulty().ChargingTime);
	}
	else
	{
		InPuzzle->OnChargingStarted.RemoveDynamic(this, &ARSSoundChargedCrystal::OnChargeStarted);
		InPuzzle->OnChargingStopped.RemoveDynamic(this, &ARSSoundChargedCrystal::OnChargeStopped);
		InPuzzle->OnChargingTick.RemoveDynamic(this, &ARSSoundChargedCrystal::OnChargeTick);
		InPuzzle->OnSetCrystalPlayable.RemoveDynamic(this, &ARSSoundChargedCrystal::SetPlayable);
		InPuzzle->OnChargeTimeUpdate.RemoveDynamic(this, &ARSSoundChargedCrystal::SetMaxChargeValue);
		InPuzzle->OnPuzzleReset.RemoveDynamic(this, &ARSSoundChargedCrystal::OnPuzzleReset);
		InPuzzle->OnPuzzleSolved.RemoveDynamic(this, &ARSSoundChargedCrystal::OnPuzzleSolved);
	}	
}

void ARSSoundChargedCrystal::SetupEffectsSpline(const TArray<FVector>& InTargetPoints) const
{
	check(EffectsSpline);
	EffectsSpline->SetupJumpingSpline(InTargetPoints);
}

void ARSSoundChargedCrystal::PickUp(ARSCharacter* InInstigator)
{
	Super::PickUp(InInstigator);
	UpdateTransmittedSoundData(CurrentTransmittedSoundData);
}

void ARSSoundChargedCrystal::OnPuzzleSolved()
{
	EffectsSpline->bCanJump = false;
}

bool ARSSoundChargedCrystal::CanBeCharged()
{
	return IsEquippedByPlayer() && !bIsPlaying && CurrentChargeValue < MaxChargeValue;
}

void ARSSoundChargedCrystal::TryPlaySound()
{
	if(!CanPlaySound())
	{
		return;
	}
	PlaySound();
}

void ARSSoundChargedCrystal::UpdateTransmittedSoundData(const FRSTransmittedSoundData& InNewData)
{
	if (!CanUpdateSoundData())
		return;
	CurrentTransmittedSoundData = InNewData;
	OnSoundDataUpdate.Broadcast(CurrentTransmittedSoundData);
	AudioComponent->SetFloatParameter("Frequency", CurrentTransmittedSoundData.Frequency);
}

void ARSSoundChargedCrystal::SetPlayable(bool bInActive)
{
	bIsActivated = bInActive;
	OnActivationStateChanged.Broadcast(bInActive);
	EffectsSpline->OnCrystalPlayableSet(bInActive);
}

void ARSSoundChargedCrystal::PlaySound_Implementation()
{
	if (!bIsActivated)
		return;
	if (bIsPlaying)
		return;
	if (CurrentChargeValue <= KINDA_SMALL_NUMBER)
		return;
	AudioComponent->Play();
	bIsPlaying = true;
	OnSoundTransmitted.Broadcast(CurrentTransmittedSoundData);
}

void ARSSoundChargedCrystal::StopSound_Implementation()
{
	AudioComponent->Stop();
	bIsPlaying = false;
	if(bWasPreviouslyCharged && CurrentChargeValue <= KINDA_SMALL_NUMBER)
	{
		bWasPreviouslyCharged = false;
		OnChargeFullyDepleted.Broadcast();
		SetPlayable(false);
		EffectsSpline->OnChargeDepleted();
	}
}

bool ARSSoundChargedCrystal::CanPlaySound()
{
	return !bIsBeingCharged && IsEquippedByPlayer() && CurrentChargeValue > 0;
}

void ARSSoundChargedCrystal::UpdateChargeValue(float InDelta)
{
	if (!IsEquippedByPlayer())
		return;
	if(bIsBeingCharged)
	{
		if (CurrentChargeValue < MaxChargeValue)
		{
			CurrentChargeValue += InDelta;
		}
		else
		{
			CurrentChargeValue = MaxChargeValue;
			bIsBeingCharged = false;
			OnMaxChargeValueReached.Broadcast();
		}
		if (!bWasPreviouslyCharged)
			bWasPreviouslyCharged = true;
		return;
	}
	if (bIsPlaying)
	{
		if (CurrentChargeValue > 0)
		{
			CurrentChargeValue -= InDelta;
		}
		else
		{
			CurrentChargeValue = 0.f;
			StopSound();
		}
		if(!bIsActivated)
		{
			StopSound();
		}
	}	
}

void ARSSoundChargedCrystal::BeginPlay()
{
	Super::BeginPlay();
	OnItemPickedUp.AddUniqueDynamic(this, &ARSSoundChargedCrystal::BP_AddItemHUD);
	OnItemDroppedStart.AddUniqueDynamic(this, &ARSSoundChargedCrystal::BP_RemoveItemHUD);
}

void ARSSoundChargedCrystal::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	OnItemPickedUp.RemoveAll(this);
	OnItemDroppedStart.RemoveAll(this);
	Super::EndPlay(EndPlayReason);
}

void ARSSoundChargedCrystal::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	UpdateChargeValue(DeltaSeconds);
}

void ARSSoundChargedCrystal::FinishDroppingItem()
{
	Super::FinishDroppingItem();
	bIsBeingCharged = false;
	SetPlayable(false);
	if(bIsPlaying)
	{
		StopSound();
	}	
}

void ARSSoundChargedCrystal::OnPuzzleReset()
{
	Super::OnPuzzleReset();
	CurrentChargeValue = 0.f;
	StopSound();
	OnChargeStopped();
	EffectsSpline->OnPuzzleReset();
	if (IsEquippedByPlayer())
	{
		Drop(URSCharacterFunctionLibrary::GetPlayerCharacter(this));
	}	
}
