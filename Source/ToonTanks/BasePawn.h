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
class UActorComponent;
class UWidgetComponent;

UCLASS()
class TOONTANKS_API ABasePawn : public APawn {
  GENERATED_BODY()

 public:
  // Sets default values for this pawn's properties
  ABasePawn();
  void Fire();
  virtual void HandleDestruction();

 protected:
 protected:
  // Called when the game starts or when spawned
  virtual void BeginPlay() override;
  void RotateTurret(FVector lookAtTarget);

  UPROPERTY(EditAnywhere)
  UStaticMeshComponent* BaseMesh{nullptr};
  UPROPERTY(EditAnywhere)
  UStaticMeshComponent* TurretMesh{nullptr};
  UPROPERTY(EditAnywhere)
  USceneComponent* ProjectileSpawnPoint{nullptr};

 private:
  UPROPERTY(EditDefaultsOnly)
  TSubclassOf<AProjectile> ProjectileClass;

  UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
  UCapsuleComponent* CapsuleComp{nullptr};

  UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
  UParticleSystem* DeathParticles;

  UPROPERTY(EditAnywhere)
  USoundBase* DeathSound{nullptr};

  UPROPERTY(EditAnywhere)
  TSubclassOf<UCameraShakeBase> DeathCameraShakeClass;

  UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
  UActorComponent* HealthComponent{nullptr};
  UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
  UWidgetComponent* HealthBarWidget{nullptr};
};
