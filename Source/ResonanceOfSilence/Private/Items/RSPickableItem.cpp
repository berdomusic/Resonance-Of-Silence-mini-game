// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/RSPickableItem.h"

#include "Characters/RSCharacter.h"

// Sets default values
ARSPickableItem::ARSPickableItem()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	ItemMesh->SetupAttachment(RootComponent);
	ItemMesh->CanCharacterStepUpOn = ECB_No;
}

bool ARSPickableItem::IsEquippedByPlayer()
{
	ARSPlayerCharacter* player = URSCharacterFunctionLibrary::GetPlayerCharacter(this);
	if (!IsValid(player))
		return false;
	const URSCharacterInventoryComponent* const inventory = player->GetInventoryComponent();
	if (!IsValid(player))
		return false;
	return inventory->CurrentItemInHand == this;
}

void ARSPickableItem::TryInteract(ARSCharacter* InInstigator)
{
	Super::TryInteract(InInstigator);
	if (!IsValid(InInstigator))
		return;

	URSCharacterInventoryComponent* inventory = InInstigator->GetInventoryComponent();
	if (!IsValid(inventory))
	{
		checkNoEntry()
			return;
	}
	ARSPickableItem* currentItem = inventory->CurrentItemInHand;
	if (currentItem != nullptr)
	{
		currentItem->Drop(InInstigator);
		if(currentItem == this)
		{			
			return;
		}
	}
	if (!bCanBePickedUp || !inventory->bCanPickUpItem())
	{
		UE_LOG(LogTemp, Warning, TEXT("Can't pick up item"));
		return;
	}	
	PickUp(InInstigator);
}

void ARSPickableItem::PickUp(ARSCharacter* InInstigator)
{
	if (!IsValid(InInstigator))
		return;
	if (!IsValid(InInstigator->GetCharacterMesh()))
		return;

	URSCharacterInventoryComponent* inventory = InInstigator->GetInventoryComponent();
	if (!IsValid(inventory))
	{
		checkNoEntry()
			return;
	}
	inventory->OnPickUpItem(this);

	InitPickedUpTransform = GetActorTransform();
	ChangeInteractionCollisionCollision(true);
	FAttachmentTransformRules rules(EAttachmentRule::SnapToTarget, true);
	AttachToComponent(InInstigator->GetCharacterMesh(), rules, InInstigator->GetEquippedItemSocketName());
	SetActorRelativeTransform(AttachmmentTransform);

	PrimaryActorTick.SetTickFunctionEnable(true);
	OnItemPickedUp.Broadcast(this);
}

void ARSPickableItem::Drop(ARSCharacter* InInstigator)
{
	if (!IsValid(InInstigator))
		return;
	if (!IsValid(InInstigator->GetCharacterMesh()))
		return;

	URSCharacterInventoryComponent* inventory = InInstigator->GetInventoryComponent();
	if (!IsValid(inventory))
	{
		checkNoEntry()
			return;
	}
	inventory->OnDropItem(this);
	const FVector& dropLoc = GetDropLocation(InInstigator);
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	StartDroppingItem(dropLoc);
	OnItemDroppedStart.Broadcast(this);
}

void ARSPickableItem::StartDroppingItem(const FVector& InLocation)
{
	DropData.DropItemTargetLocation = InLocation;
	DropData.CurrentItemTrans = GetActorTransform();
	CurrentDropAlpha = 0.f;

	bDroppedItemTickEnabled = true;
}

void ARSPickableItem::FinishDroppingItem()
{
	ChangeInteractionCollisionCollision(false);
	PrimaryActorTick.SetTickFunctionEnable(false);
	bDroppedItemTickEnabled = false;
	OnItemDroppedFinish.Broadcast(this);
}

void ARSPickableItem::DropItemTick(float InDelta)
{
	CurrentDropAlpha += InDelta / 0.15f;
	CurrentDropAlpha = FMath::Clamp(CurrentDropAlpha, 0.f, 1.f);

	const FVector& NewLocation = FMath::Lerp(DropData.CurrentItemTrans.GetLocation(), DropData.DropItemTargetLocation, CurrentDropAlpha);
	const FQuat StartQuat = DropData.CurrentItemTrans.GetRotation();
	const FQuat EndQuat = InitPickedUpTransform.GetRotation();
	const FQuat SlerpedQuat = FQuat::Slerp(StartQuat, EndQuat, CurrentDropAlpha);

	SetActorLocationAndRotation(NewLocation, SlerpedQuat);

	if (CurrentDropAlpha >= 1.f)
	{
		FinishDroppingItem();
	}
}

FVector ARSPickableItem::GetDropLocation(ARSCharacter* InDropInstigator)
{	
	if (!IsValid(InDropInstigator))
		return FVector();
	FVector outVector = InDropInstigator->GetActorForwardVector() * 50.f;
	if (!IsValid(GetWorld()))
		return outVector;

	FCollisionQueryParams params;
	params.AddIgnoredActors(TArray<AActor*> { this, InDropInstigator } );
	FHitResult hit;
	GetWorld()->LineTraceSingleByChannel(hit, GetActorLocation(), GetActorLocation() - FVector(0, 0, 500.f),
		ECC_Visibility, params, FCollisionResponseParams::DefaultResponseParam);
	if(hit.bBlockingHit)
	{
		outVector = hit.ImpactPoint;
	}
	return outVector;
}

void ARSPickableItem::OnPuzzleReset()
{
}

void ARSPickableItem::ChangeInteractionCollisionCollision(bool bPickedUp) const
{
	Super::ChangeInteractionCollisionCollision(bPickedUp);
	ItemMesh->SetCollisionProfileName(GetCollisionProfileName(bPickedUp));
}

void ARSPickableItem::OnLookAtStarted_Implementation()
{
	Super::OnLookAtStarted_Implementation();
	bCanBePickedUp = true;
	/*if(ItemNameText)
	{
		ItemNameText->SetVisibility(true, false);
	}*/
}

void ARSPickableItem::OnLookAtFinished_Implementation()
{	
	
	Super::OnLookAtFinished_Implementation();
	bCanBePickedUp = false;
	/*if (ItemNameText)
	{
		ItemNameText->SetVisibility(false, false);
	}*/
}

void ARSPickableItem::OnBeginInteraction_Implementation(AController* InteractionInstigator)
{
	Super::OnBeginInteraction_Implementation(InteractionInstigator);
	if (!IsValid(InteractionInstigator))
		return;
	ARSCharacter* InstigatorCharacter = Cast<ARSCharacter>(InteractionInstigator->GetPawn());
	if (!IsValid(InstigatorCharacter))
		return;
	URSCharacterInventoryComponent* inventory = InstigatorCharacter->GetInventoryComponent();
	if (!IsValid(inventory))
	{
		checkNoEntry()
			return;
	}
	TryInteract(InstigatorCharacter);
}

void ARSPickableItem::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	if(bDroppedItemTickEnabled)
	{
		DropItemTick(DeltaSeconds);
	}
}



