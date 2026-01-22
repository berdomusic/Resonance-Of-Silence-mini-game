// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Audio/RSCharacterAudioComponent.h"
#include "System/Interaction/RSCharacterInteractionComponent.h"
#include "Characters/RSCharacterInventoryComponent.h"

#include "RSCharacter.generated.h"

UCLASS()
class RESONANCEOFSILENCE_API ARSCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARSCharacter();

	USkeletalMeshComponent* GetCharacterMesh() const;
	UFUNCTION(BlueprintPure, Category = "RS|Character")
	FName GetEquippedItemSocketName() { return "hand_r"; }

	FORCEINLINE URSCharacterInteractionComponent* GetInteractionComponent() const { return InteractionComponent; }
	FORCEINLINE URSCharacterAudioComponent* GetAudioComponent() const { return AudioComponent; }
	FORCEINLINE URSCharacterInventoryComponent* GetInventoryComponent() const { return InventoryComponent;  }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	URSCharacterInteractionComponent* InteractionComponent;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	URSCharacterAudioComponent* AudioComponent;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	URSCharacterInventoryComponent* InventoryComponent;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
