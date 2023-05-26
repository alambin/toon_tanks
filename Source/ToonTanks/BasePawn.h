// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
// This file should always be included last - rule of UE
#include "BasePawn.generated.h"

class UCapsuleComponent;
class AProjectile;
class USoundBase;
class UCameraShakeBase;

UCLASS()
class TOONTANKS_API ABasePawn : public APawn {
  GENERATED_BODY()

 public:
  // Sets default values for this pawn's properties
  ABasePawn();
  virtual void HandleDestruction();

 protected:
  void RotateTurret(FVector lookAtTarget);
  void Fire();

  UPROPERTY(EditDefaultsOnly)
  TSubclassOf<AProjectile> ProjectileClass;

  UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
  UStaticMeshComponent* BaseMesh{nullptr};
  UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
  UStaticMeshComponent* TurretMesh{nullptr};
  UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
  USceneComponent* ProjectileSpawnPoint{nullptr};

 private:
  UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
  UCapsuleComponent* CapsuleComp{nullptr};

  UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
  UParticleSystem* DeathParticles;

  UPROPERTY(EditAnywhere)
  USoundBase* DeathSound{nullptr};

  UPROPERTY(EditAnywhere)
  TSubclassOf<UCameraShakeBase> DeathCameraShakeClass;
};