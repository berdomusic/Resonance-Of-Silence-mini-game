// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/WidgetComponent.h"
#include "RSInteractableActor.generated.h"

class USphereComponent;

UCLASS()
class RESONANCEOFSILENCE_API ARSInteractableActor : public AActor, public ILookAtInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARSInteractableActor();

	UFUNCTION(BlueprintNativeEvent, BlueprintPure)
	FName GetItemName();

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UWidgetComponent* WidgetComponent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USphereComponent* InteractionSphere;	

	virtual void TryInteract(ARSCharacter* InInstigator);

	virtual void ChangeInteractionCollisionCollision(bool bPickedUp) const;
	static FName GetCollisionProfileName(bool bPickedUp);

	virtual void OnLookAtStarted_Implementation() override;
	virtual void OnLookAtFinished_Implementation() override;
	void SetInteractionWidgetText();

	virtual void BeginPlay() override;

};
