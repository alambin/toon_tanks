// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
// This file should always be included last - rule of UE
#include "Projectile.generated.h"

class UProjectileMovementComponent;
class UParticleSystem;
class UParticleSystemComponent;
class USoundBase;
class UCameraShakeBase;

UCLASS()
class TOONTANKS_API AProjectile : public AActor {
  GENERATED_BODY()

 public:
  // Sets default values for this actor's properties
  AProjectile();
  // Called every frame
  virtual void Tick(float DeltaTime) override;

 protected:
  // Called when the game starts or when spawned
  virtual void BeginPlay() override;
  UFUNCTION(BlueprintImplementableEvent)
  void ApplyExplosionForceBPEvent(UPrimitiveComponent* Component, const FVector& Location);

 private:
  UFUNCTION()
  void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
             FVector NormalImpulse, const FHitResult& Hit);

  UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
  UStaticMeshComponent* Mesh{nullptr};

  UPROPERTY(VisibleAnywhere)
  UParticleSystemComponent* SmokeTrail{nullptr};

  UPROPERTY(VisibleAnywhere)
  UProjectileMovementComponent* MovementComponent{nullptr};

  UPROPERTY(EditAnywhere)
  UParticleSystem* HitParticles{nullptr};

  UPROPERTY(EditAnywhere)
  USoundBase* LaunchSound{nullptr};

  UPROPERTY(EditAnywhere)
  USoundBase* HitSound{nullptr};

  UPROPERTY(EditAnywhere)
  TSubclassOf<UCameraShakeBase> HitCameraShakeClass;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
  float ExplosionForce{1000000.0};

  UPROPERTY(EditAnywhere)
  float StartingSpeed{1000.0};

  UPROPERTY(EditAnywhere)
  float MaximumSpeed{1000.0};

  UPROPERTY(EditAnywhere)
  float Damage{50.0};
};
