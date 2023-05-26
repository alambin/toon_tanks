// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BasePawn.h"
#include "CoreMinimal.h"
// This file should always be included last - rule of UE
#include "Tower.generated.h"

class ATank;

/**
 *
 */
UCLASS()
class TOONTANKS_API ATower : public ABasePawn {
  GENERATED_BODY()

 public:
  void HandleDestruction() override;

 protected:
  // Called when the game starts or when spawned
  virtual void BeginPlay() override;

 private:
  // Called every frame
  virtual void Tick(float DeltaTime) override;
  void CheckFireConditions();
  bool IsTankInFireRange() const;

  UPROPERTY(EditDefaultsOnly, Category = "Combat")
  float FireRange{700.0};

  FTimerHandle FireRateTimerHandle;
  float FireRate{2.0};

  ATank* Tank{nullptr};
};
