// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/RSCharacter.h"



// Sets default values
ARSCharacter::ARSCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	InteractionComponent = CreateDefaultSubobject<URSCharacterInteractionComponent>(TEXT("InteractionComponent"));
	AudioComponent = CreateDefaultSubobject<URSCharacterAudioComponent>(TEXT("AudioComponent"));
	InventoryComponent = CreateDefaultSubobject<URSCharacterInventoryComponent>(TEXT("InventoryComponent"));
}

// Called when the game starts or when spawned
void ARSCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

USkeletalMeshComponent* ARSCharacter::GetCharacterMesh() const
{
	USkeletalMeshComponent* FoundMesh = this->FindComponentByClass<USkeletalMeshComponent>();
	return FoundMesh;
}

// Called every frame
void ARSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ARSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

