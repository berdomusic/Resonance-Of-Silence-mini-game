// Fill out your copyright notice in the Description page of Project Settings.


#include "System/Interaction/RSInteractableActor.h"
#include "Components/SphereComponent.h"
#include "UI/RSInteractionWidget.h"

// Sets default values
ARSInteractableActor::ARSInteractableActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
	WidgetComponent->SetupAttachment(RootComponent);
	WidgetComponent->SetVisibility(false);
	WidgetComponent->SetDrawSize(FVector2D(75, 25));
	WidgetComponent->SetRelativeLocation(FVector(0, 0, 100));

	InteractionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionSphere"));
	InteractionSphere->SetupAttachment(RootComponent);
	InteractionSphere->SetCollisionProfileName(TEXT("Interactable"));
	InteractionSphere->SetSphereRadius(150.f);
	InteractionSphere->CanCharacterStepUpOn = ECB_No;
}

FName ARSInteractableActor::GetItemName_Implementation()
{
	return "Interactable";
}

void ARSInteractableActor::TryInteract(ARSCharacter* InInstigator)
{
}

void ARSInteractableActor::ChangeInteractionCollisionCollision(bool bPickedUp) const
{
	InteractionSphere->SetCollisionProfileName(GetCollisionProfileName(bPickedUp));
	if (!bPickedUp)
	{
		InteractionSphere->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
	}		
}

FName ARSInteractableActor::GetCollisionProfileName(bool bPickedUp)
{
	return bPickedUp ? "NoCollision" : "Interactable";
}

void ARSInteractableActor::OnLookAtStarted_Implementation()
{
	ILookAtInterface::OnLookAtStarted_Implementation();
	if (!IsValid(WidgetComponent))
		return;
	WidgetComponent->SetVisibility(true);
}

void ARSInteractableActor::OnLookAtFinished_Implementation()
{
	ILookAtInterface::OnLookAtFinished_Implementation();
	if (!IsValid(WidgetComponent))
		return;
	WidgetComponent->SetVisibility(false);
}

void ARSInteractableActor::SetInteractionWidgetText()
{
	URSInteractionWidget* interactionWidget = Cast<URSInteractionWidget>(WidgetComponent->GetUserWidgetObject());
	if(!IsValid(interactionWidget))
	{
		return;
	}
	interactionWidget->SetInteractionText(GetItemName());;
}

void ARSInteractableActor::BeginPlay()
{
	Super::BeginPlay();
	SetInteractionWidgetText();
}
