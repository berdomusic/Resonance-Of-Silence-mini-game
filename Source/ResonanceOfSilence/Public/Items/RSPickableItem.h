// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "System/Interaction/RSInteractableActor.h"
#include "Components/TextRenderComponent.h"
#include "RSPickableItem.generated.h"

struct FRSDropItemData
{
	FVector DropItemTargetLocation;
	FTransform CurrentItemTrans;
};

class USphereComponent;
class ARSCharacter;
class URSUserWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemPickedUp, ARSPickableItem*, PickedUpItem);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemDroppedStart, ARSPickableItem*, DroppedItem);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemDroppedFinish, ARSPickableItem*, DroppedItem);

UCLASS()
class RESONANCEOFSILENCE_API ARSPickableItem : public ARSInteractableActor
{
	GENERATED_BODY()
	
public:	

	ARSPickableItem();

	//this is temp, need to change later for localization
	
	UFUNCTION(BlueprintPure)
	bool IsEquippedByPlayer();

	UPROPERTY(BlueprintAssignable)
	FOnItemPickedUp OnItemPickedUp;
	UPROPERTY(BlueprintAssignable)
	FOnItemDroppedStart OnItemDroppedStart;
	UPROPERTY(BlueprintAssignable)
	FOnItemDroppedFinish OnItemDroppedFinish;

protected:

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UStaticMeshComponent* ItemMesh;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	FTransform AttachmmentTransform;
	UPROPERTY(BlueprintReadOnly)
	FTransform InitPickedUpTransform;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "UI")
	URSUserWidget* ItemHUDWidget = nullptr;

	virtual void TryInteract(ARSCharacter* InInstigator) override;
	virtual void PickUp(ARSCharacter* InInstigator);	
	void Drop(ARSCharacter* InInstigator);

	void StartDroppingItem(const FVector& InLocation);
	virtual void FinishDroppingItem();
	void DropItemTick(float InDelta);
	FVector GetDropLocation(ARSCharacter* InDropInstigator);	
	FRSDropItemData DropData;
	float CurrentDropAlpha;
	const float DropItemDuration = .15f;
	bool bDroppedItemTickEnabled;	

	FTimerHandle DropTimer;

	UFUNCTION()
	virtual void OnPuzzleReset();

	UFUNCTION(BlueprintCallable)
	virtual void ChangeInteractionCollisionCollision(bool bPickedUp) const override;

	bool bCanBePickedUp;
	virtual void OnLookAtStarted_Implementation() override;
	virtual void OnLookAtFinished_Implementation() override;

	virtual void OnBeginInteraction_Implementation(AController* InteractionInstigator) override;

	virtual void Tick(float DeltaSeconds) override;
};
