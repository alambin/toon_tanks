// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BaseDestructable.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
// This file should always be included last - rule of UE
#include "DisruptableWall.generated.h"

UCLASS()
class TOONTANKS_API ADisruptableWall : public AActor {
  GENERATED_BODY()

 public:
  // Sets default values for this actor's properties
  ADisruptableWall();

 protected:
  // Called when the game starts or when spawned
  virtual void BeginPlay() override;

 private:
  UPROPERTY(EditAnywhere)
  int32 NumOfElements{2};
  UPROPERTY(EditAnywhere)
  float ElementSize{200};

  UPROPERTY(EditAnywhere)
  TSubclassOf<ABaseDestructable> ElementClass;
};
