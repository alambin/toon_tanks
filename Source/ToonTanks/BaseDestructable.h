// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
// This file should always be included last - rule of UE
#include "BaseDestructable.generated.h"

class UActorComponent;
class UWidgetComponent;
class UGeometryCollectionComponent;

UCLASS()
class TOONTANKS_API ABaseDestructable : public AActor {
  GENERATED_BODY()

 public:
  // Sets default values for this actor's properties
  ABaseDestructable();
  virtual void HandleDestruction(FVector HitLocation, FVector ShotFromDirection);

  UFUNCTION(BlueprintImplementableEvent)
  void ApplyExplosionForceBPEvent(const FVector& HitLocation, const FVector& ShotFromDirection);

  UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
  UGeometryCollectionComponent* GeometryCollectionComponent{nullptr};
  UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
  UActorComponent* HealthComponent{nullptr};
  UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
  UWidgetComponent* HealthBarWidget{nullptr};

 private:
  UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
  float ExplosionForce{4000.0};
};
