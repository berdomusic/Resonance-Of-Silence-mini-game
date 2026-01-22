// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/RSWorldBoundaryActor.h"

#include "Components/SphereComponent.h"
#include "FX/Spline/RSEffectsSplineComponent.h"

ARSWorldBoundaryActor::ARSWorldBoundaryActor()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = .25f;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Collision Sphere"));
	CollisionSphere->SetupAttachment(RootComponent);
	CollisionSphere->SetSphereRadius(100.f);
	CollisionSphere->SetCollisionProfileName("OverlapOnlyPawn");

	BoundarySpline = CreateDefaultSubobject<URSEffectsSplineComponent>(TEXT("Boundary Spline"));
	BoundarySpline->SetupAttachment(RootComponent);
	BoundarySpline->SetClosedLoop(true);
	BoundarySpline->ClearSplinePoints(true);
	BoundarySpline->AddSplinePoint(FVector(0.f, 0.f, 0.f), ESplineCoordinateSpace::Local);
	BoundarySpline->AddSplinePoint(FVector(0.f, 500.f, 0.f), ESplineCoordinateSpace::Local);
	BoundarySpline->AddSplinePoint(FVector(500.f, 500.f, 0.f), ESplineCoordinateSpace::Local);
	BoundarySpline->AddSplinePoint(FVector(500.f, 0.f, 0.f), ESplineCoordinateSpace::Local);
}

void ARSWorldBoundaryActor::OnCollisionSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!URSCharacterFunctionLibrary::IsActorPlayerCharacter(OtherActor))
		return;
	ARSPlayerCharacter* player = Cast<ARSPlayerCharacter>(OtherActor);
	if (!IsValid(player))
		return;

	const FVector sphereLocation = CollisionSphere->GetComponentLocation();
	const FVector direction = (player->GetActorLocation() - sphereLocation).GetSafeNormal();
	
	player->LaunchCharacter(direction * 1250.f, true, false);

	const FRotator Rot = direction.Rotation();
	const FTransform FxTransform(Rot, sphereLocation, FVector::OneVector);

	OnPlayerPushedBack(FxTransform);
}

void ARSWorldBoundaryActor::BeginPlay()
{
	Super::BeginPlay();
	CollisionSphere->OnComponentBeginOverlap.AddUniqueDynamic(this, &ARSWorldBoundaryActor::OnCollisionSphereOverlap);
}

void ARSWorldBoundaryActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	CollisionSphere->OnComponentBeginOverlap.RemoveAll(this);
	Super::EndPlay(EndPlayReason);
}

void ARSWorldBoundaryActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	float distanceToPlayerSquared;
	FVector closestPoint;
	const bool bWasActive = bIsActive; 
	bIsActive = ShouldBoundaryBeActive(distanceToPlayerSquared, closestPoint);
	if (bWasActive != bIsActive)
	{
		ManageSplineVFX(bIsActive);	
	}	
	if (bIsActive)
	{
		CollisionSphere->SetWorldLocation(closestPoint);
	}	
}

bool ARSWorldBoundaryActor::ShouldBoundaryBeActive(float& OutDistanceSquared, FVector& OutSphereLoc) const
{
	const ARSPlayerCharacter* const player = URSCharacterFunctionLibrary::GetPlayerCharacter(this);
	if (!IsValid(player) || !IsValid(BoundarySpline))
		return false;

	const FVector playerLoc = player->GetActorLocation();
	const FVector closestPoint = BoundarySpline->FindLocationClosestToWorldLocation(playerLoc, ESplineCoordinateSpace::World);
	OutSphereLoc = FVector(closestPoint.X, closestPoint.Y, playerLoc.Z);
	OutDistanceSquared = UKismetMathLibrary::Vector_DistanceSquared(playerLoc, OutSphereLoc);
	return OutDistanceSquared < FMath::Square(ActivationDistance);
}

