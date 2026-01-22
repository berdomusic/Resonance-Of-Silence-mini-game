// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RSWorldBoundaryActor.generated.h"

class USphereComponent;
class URSEffectsSplineComponent;
class UNiagaraSystem;
class URSTooltipEntry;

UCLASS()
class RESONANCEOFSILENCE_API ARSWorldBoundaryActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ARSWorldBoundaryActor();

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void OnPlayerPushedBack(FTransform InTrans);

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	USphereComponent* CollisionSphere;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	URSEffectsSplineComponent* BoundarySpline;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TSoftObjectPtr<UNiagaraSystem> BumpVFX;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TSoftObjectPtr<UNiagaraSystem> SplineVFX;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TSoftObjectPtr<URSTooltipEntry> BoundaryWidget;

	UPROPERTY(BlueprintReadOnly, EditInstanceOnly)
	float ActivationDistance = 1000.f;
	
	UPROPERTY(BlueprintReadOnly)
	bool bIsActive;

	UFUNCTION()
	void OnCollisionSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, 
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintImplementableEvent)
	void ManageSplineVFX(bool bInActive);
	bool ShouldBoundaryBeActive(float& OutDistanceSquared, FVector& OutSphereLoc) const;
};
