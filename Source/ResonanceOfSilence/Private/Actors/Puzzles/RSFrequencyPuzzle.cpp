// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Puzzles/RSFrequencyPuzzle.h"

#include "Actors/Puzzles/RSPuzzleSubsystem.h"
#include "Items/RSPickableItem.h"

ARSFrequencyPuzzle::ARSFrequencyPuzzle()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	ChargingUpSphere = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ChargingUpSphere"));
	ChargingUpSphere->SetupAttachment(RootComponent);
	ChargingUpSphere->SetCollisionProfileName("BlockAllDynamicNoCamera");

	ChargingUpSphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("ChargingUpSphereCollision"));
	ChargingUpSphereCollision->SetupAttachment(ChargingUpSphere);
	ChargingUpSphereCollision->SetSphereRadius(150.f);
	ChargingUpSphereCollision->SetCollisionProfileName("Trigger");

	ChargingSphereAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("ChargingUpAudioComponent"));
	ChargingSphereAudioComponent->SetupAttachment(ChargingUpSphere);

	CrystalActivationSphere = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CrystalActivationSphere"));
	CrystalActivationSphere->SetupAttachment(RootComponent);
	CrystalActivationSphere->SetCollisionProfileName("BlockAllDynamicNoCamera");

	CrystalActivationSphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("CrystalActivationSphereCollision"));
	CrystalActivationSphereCollision->SetupAttachment(CrystalActivationSphere);
	CrystalActivationSphereCollision->SetSphereRadius(150.f);
	CrystalActivationSphereCollision->SetCollisionProfileName("Trigger");

	CrystalActivationAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("CrystalActivationAudioComponent"));
	CrystalActivationAudioComponent->SetupAttachment(CrystalActivationSphere);

	BlockingBox = CreateDefaultSubobject<UBoxComponent>(TEXT("BlockingBox"));
	BlockingBox->SetupAttachment(RootComponent);
	BlockingBox->SetCollisionProfileName("BlockPawn");
}

void ARSFrequencyPuzzle::OnChargingCrystalPickup(ARSPickableItem* InCrystal)
{
	if (!IsValid(InCrystal))
		return;
	ARSSoundChargedCrystal* const soundCrystal = Cast<ARSSoundChargedCrystal>(InCrystal);
	if (!IsValid(soundCrystal))
		return;

	soundCrystal->OnChargeFullyDepleted.AddUniqueDynamic(this, &ARSFrequencyPuzzle::SetNewSolvingConditionFromCurrentDifficulty);
	soundCrystal->OnSoundTransmitted.AddUniqueDynamic(this, &ARSFrequencyPuzzle::OnReceivedTransmittedSound);
	soundCrystal->OnMaxChargeValueReached.AddUniqueDynamic(this, &ARSFrequencyPuzzle::OnCrystalMaxCharged);
	bCrystalMaxCharged = soundCrystal->IsFullyCharged();

	soundCrystal->FrequencyPuzzleSetup(true, this);
	bPlayerHasCrystalEquipped = true;

	const TArray<FVector>& targetPointsForSpline ={
		ChargingUpSphere->GetComponentLocation(),
		CrystalActivationSphere->GetComponentLocation(),
		PrizeItemSoft.Get()->GetActorLocation()
	};
	soundCrystal->SetupEffectsSpline(targetPointsForSpline);
}

void ARSFrequencyPuzzle::OnChargingCrystalDropped(ARSPickableItem* InCrystal)
{
	if (!IsValid(InCrystal))
		return;
	ARSSoundChargedCrystal* const soundCrystal = Cast<ARSSoundChargedCrystal>(InCrystal);
	if (!IsValid(soundCrystal))
		return;

	soundCrystal->OnChargeFullyDepleted.RemoveDynamic(this, &ARSFrequencyPuzzle::SetNewSolvingConditionFromCurrentDifficulty);
	soundCrystal->OnSoundTransmitted.RemoveDynamic(this, &ARSFrequencyPuzzle::OnReceivedTransmittedSound);
	soundCrystal->OnMaxChargeValueReached.RemoveDynamic(this, &ARSFrequencyPuzzle::OnCrystalMaxCharged);
	bCrystalMaxCharged = false;

	soundCrystal->FrequencyPuzzleSetup(false, this);
	
	bPlayerHasCrystalEquipped = false;
}

bool ARSFrequencyPuzzle::CanCrystalBeCharged() const
{
	return bPlayerOverlapsChargingSphere && bPlayerHasCrystalEquipped && !bCrystalMaxCharged;
}

void ARSFrequencyPuzzle::StartCharging_Implementation()
{
	if (!CanCrystalBeCharged())
		return;
	if(!IsValid(GetWorld()))
	{
		checkNoEntry()
			return;
	}	
	bIsCharging = true;
	OnChargingStarted.Broadcast();
	GetWorld()->GetTimerManager().SetTimer(ChargingTimerHandle, this, &ARSFrequencyPuzzle::ChargeTick, .1f, true);
	ChargingSphereAudioComponent->Play(0);
}

void ARSFrequencyPuzzle::StopCharging_Implementation()
{
	bIsCharging = false;
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(ChargingTimerHandle);
	}
	ChargingSphereAudioComponent->Stop();
	OnChargingStopped.Broadcast();
}

void ARSFrequencyPuzzle::ChargeTick()
{
	if(bIsCharging)
	{		
		if(!CanCrystalBeCharged())
		{
			StopCharging();
			return;
		}
		OnChargingTick.Broadcast();
	}
	if(CanCrystalBeCharged() && !bIsCharging)
	{
		StartCharging();
	}
}

void ARSFrequencyPuzzle::OnCrystalMaxCharged()
{
	if(!bCrystalMaxCharged)
	{
		bCrystalMaxCharged = true;
	}
	StopCharging();
}

void ARSFrequencyPuzzle::OnChargeSphereOverlap(UPrimitiveComponent* OverlappedComponent,
                                               AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                               const FHitResult& SweepResult)
{
	if(!URSCharacterFunctionLibrary::IsActorPlayerCharacter(OtherActor))
	{
		return;
	}
	bPlayerOverlapsChargingSphere = true;	
	if(CanCrystalBeCharged())
	{
		OpenDoors();
		StartCharging();
	}
}

void ARSFrequencyPuzzle::OnChargeSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                                 UPrimitiveComponent* OtherComp, int OtherBodyIndex)
{
	if (!URSCharacterFunctionLibrary::IsActorPlayerCharacter(OtherActor))
		return;

	bPlayerOverlapsChargingSphere = false;
	CloseDoors();
	if (!bIsCharging)
		return;
	StopCharging();
}

void ARSFrequencyPuzzle::OnCrystalActivationSphereOverlap_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!URSCharacterFunctionLibrary::IsActorPlayerCharacter(OtherActor))
		return;	
	if (!bPlayerHasCrystalEquipped)
		return;
	
	OnSetCrystalPlayable.Broadcast(true);
}

void ARSFrequencyPuzzle::OnCrystalActivationSphereEndOverlap_Implementation(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex)
{
	if (!URSCharacterFunctionLibrary::IsActorPlayerCharacter(OtherActor))
		return;
	if (!bPlayerHasCrystalEquipped)
		return;
	
	OnSetCrystalPlayable.Broadcast(false);
}

void ARSFrequencyPuzzle::OnReceivedTransmittedSound_Implementation(const FRSTransmittedSoundData& InData)
{
	if (bCrystalMaxCharged)
	{
		bCrystalMaxCharged = false;
	}		
	const int receivedFreq = InData.Frequency;
	const int solvingFreq = CurrentSolvingCondition.RequiredFrequency;
	const int tolerance = CurrentPuzzleDifficulty.MatchingFrequencyTolerance;
	if(FMath::Abs(receivedFreq - solvingFreq) <= tolerance)
	{
		SetPuzzleState(FRSPuzzleState(true, PuzzleCompletionTime));
		OpenDoors();
		BindDelegates(false);
	}
}

void ARSFrequencyPuzzle::SetNewSolvingConditionFromCurrentDifficulty()
{
	const FRSFrequencyPuzzleSolvingCondition newCondition = MakeNewSolvingConditionFromDifficultyPreset(CurrentPuzzleDifficulty);
	UpdateSolvingCondition(newCondition);
}

FRSFrequencyPuzzleSolvingCondition ARSFrequencyPuzzle::MakeNewSolvingConditionFromDifficultyPreset(
	const FRSFrequencyPuzzleDifficulty& InDifficulty) const
{
	FRSFrequencyPuzzleSolvingCondition newCondition;
	newCondition.RequiredFrequency = UKismetMathLibrary::RandomIntegerInRange
	(CurrentPuzzleDifficulty.RequiredFrequencyRange.X, CurrentPuzzleDifficulty.RequiredFrequencyRange.Y);

	return newCondition;
}

void ARSFrequencyPuzzle::UpdateSolvingCondition(const FRSPuzzleSolvingCondition& InNewCondition)
{
	Super::UpdateSolvingCondition(InNewCondition);
	if (const auto* Derived = static_cast<const FRSFrequencyPuzzleSolvingCondition*>(&InNewCondition))
	{
		CurrentSolvingCondition = *Derived;
		OnChargeTimeUpdate.Broadcast(CurrentPuzzleDifficulty.ChargingTime);
		ChargingSphereAudioComponent->SetFloatParameter("Frequency", CurrentSolvingCondition.RequiredFrequency);
		//for updating crystal max charge time
		BP_OnSolvingConditionUpdate();
	}
	else
	{
		checkNoEntry();
	}
}

void ARSFrequencyPuzzle::OnDifficultyChanged(const ERSGameDifficulty InNewDifficulty)
{
	Super::OnDifficultyChanged(InNewDifficulty);
	CurrentPuzzleDifficulty = *GetRowFromHandle<FRSFrequencyPuzzleDifficulty>(CurrentDifficultyRowHandle);
	if(CurrentPuzzleDifficulty.bIsFixedDifficulty)
		return;

	CurrentDifficultyRowHandle = GetHandleForDifficulty<FRSFrequencyPuzzleDifficulty>(DifficultyDataTable, InNewDifficulty);
	CurrentPuzzleDifficulty = *GetRowFromHandle<FRSFrequencyPuzzleDifficulty>(CurrentDifficultyRowHandle);
	SetNewSolvingConditionFromCurrentDifficulty();
}

void ARSFrequencyPuzzle::ResetPuzzle()
{
	Super::ResetPuzzle();
	StopCharging();
	CloseDoors();
}

void ARSFrequencyPuzzle::OnPrizeCollected_Implementation(ARSPickableItem* InItem)
{
	Super::OnPrizeCollected_Implementation(InItem);
	for (const TSoftObjectPtr<ARSSoundChargedCrystal>& crystalSoftPtr : AssociatedCrystalsSoft)
	{
		if (!crystalSoftPtr.IsValid())
			continue;
		ARSSoundChargedCrystal* const crystal = crystalSoftPtr.Get();
		crystal->Destroy();
	}
}

void ARSFrequencyPuzzle::BeginPlay()
{
	Super::BeginPlay();
	ensure(!AssociatedCrystalsSoft.IsEmpty());
	BindDelegates(true);
}

void ARSFrequencyPuzzle::EndPlay(const EEndPlayReason::Type EndPlayReason)
{	
	BindDelegates(false);
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(ChargingTimerHandle);
	}
	Super::EndPlay(EndPlayReason);
}

void ARSFrequencyPuzzle::BindDelegates(bool bBind)
{
	if (bBind)
	{
		ChargingUpSphereCollision->OnComponentBeginOverlap.AddUniqueDynamic(this, &ARSFrequencyPuzzle::OnChargeSphereOverlap);
		ChargingUpSphereCollision->OnComponentEndOverlap.AddUniqueDynamic(this, &ARSFrequencyPuzzle::OnChargeSphereEndOverlap);
		CrystalActivationSphereCollision->OnComponentBeginOverlap.AddUniqueDynamic(this, &ARSFrequencyPuzzle::OnCrystalActivationSphereOverlap);
		CrystalActivationSphereCollision->OnComponentEndOverlap.AddUniqueDynamic(this, &ARSFrequencyPuzzle::OnCrystalActivationSphereEndOverlap);
	}
	else
	{
		ChargingUpSphereCollision->OnComponentBeginOverlap.RemoveDynamic(this, &ARSFrequencyPuzzle::OnChargeSphereOverlap);
		ChargingUpSphereCollision->OnComponentEndOverlap.RemoveDynamic(this, &ARSFrequencyPuzzle::OnChargeSphereEndOverlap);
		CrystalActivationSphereCollision->OnComponentBeginOverlap.RemoveDynamic(this, &ARSFrequencyPuzzle::OnCrystalActivationSphereOverlap);
		CrystalActivationSphereCollision->OnComponentEndOverlap.RemoveDynamic(this, &ARSFrequencyPuzzle::OnCrystalActivationSphereEndOverlap);
	}
}
