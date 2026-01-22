// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/RSCharacterInventoryComponent.h"

#include "Items/RSPickableItem.h"

// Sets default values for this component's properties
URSCharacterInventoryComponent::URSCharacterInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

bool URSCharacterInventoryComponent::bCanPickUpItem()
{
	return true;
}

void URSCharacterInventoryComponent::OnPickUpItem(ARSPickableItem* InItem)
{
	CurrentItemInHand = InItem;
}

void URSCharacterInventoryComponent::OnDropItem(ARSPickableItem* InItem)
{
	if (CurrentItemInHand == InItem)
	{
		CurrentItemInHand = nullptr;
	}
}

// Called when the game starts
void URSCharacterInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void URSCharacterInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

