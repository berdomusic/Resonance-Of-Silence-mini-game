// Fill out your copyright notice in the Description page of Project Settings.


#include "System/Interaction/RSCharacterInteractionComponent.h"

// Sets default values for this component's properties
URSCharacterInteractionComponent::URSCharacterInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void URSCharacterInteractionComponent::UpdateLookAt()
{
    const AActor* Owner = GetOwner();
    if (!IsValid(Owner))
        return;

    const ARSPlayerController* PlayerController = Cast<ARSPlayerController>(Owner->GetInstigatorController());
    if (!IsValid(PlayerController))
        return;

    FVector CameraLocation;
    FRotator CameraRotation;
    PlayerController->GetPlayerViewPoint(CameraLocation, CameraRotation);

    const FVector Start = CameraLocation;
    const FVector End = Start + CameraRotation.Vector() * 500.f;

    const FCollisionQueryParams QueryParams(SCENE_QUERY_STAT(LookAtTrace), false, Owner);
    FHitResult HitResult;

    const bool bHit = GetWorld()->SweepSingleByChannel(
        HitResult, Start, End, FQuat::Identity,
        ECC_GameTraceChannel1, FCollisionShape::MakeSphere(10.f), QueryParams
    );

    AActor* HitActor = HitResult.GetActor();
    if (bHit && IsValid(HitActor))
    {
        if (HitActor->Implements<ULookAtInterface>())
        {
            if (HitActor != CurrentLookedAtActor)
            {
                if (IsValid(CurrentLookedAtActor))
                {
                    ILookAtInterface::Execute_OnLookAtFinished(CurrentLookedAtActor);
                }
                CurrentLookedAtActor = HitActor;
                ILookAtInterface::Execute_OnLookAtStarted(CurrentLookedAtActor);
            }
            return;
        }
    }
    if (IsValid(CurrentLookedAtActor))
    {
        ILookAtInterface::Execute_OnLookAtFinished(CurrentLookedAtActor);
        CurrentLookedAtActor = nullptr;
    }
}


// Called every frame
void URSCharacterInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	UpdateLookAt();
	// ...
}

