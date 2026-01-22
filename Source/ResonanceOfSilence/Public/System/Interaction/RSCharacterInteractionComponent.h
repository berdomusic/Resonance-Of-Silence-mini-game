// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RSCharacterInteractionComponent.generated.h"

class ARSPickableItem;

UCLASS( ClassGroup=(CharacterComponents), meta=(BlueprintSpawnableComponent) )
class RESONANCEOFSILENCE_API URSCharacterInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URSCharacterInteractionComponent();

	UPROPERTY(BlueprintReadOnly)
	AActor* CurrentLookedAtActor = nullptr;

protected:

	void UpdateLookAt();	

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
