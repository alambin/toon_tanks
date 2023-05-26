// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
// This file should always be included last - rule of UE
#include "HealthComponent.generated.h"

class UDamageType;
class AController;
class AToonTanksGameModeBase;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TOONTANKS_API UHealthComponent : public UActorComponent {
  GENERATED_BODY()

 public:
  // Sets default values for this component's properties
  UHealthComponent();
  // Called every frame
  virtual void TickComponent(float DeltaTime, ELevelTick TickType,
                             FActorComponentTickFunction* ThisTickFunction) override;

 protected:
  // Called when the game starts
  virtual void BeginPlay() override;

 private:
  UFUNCTION()
  // void DamageTaken(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy,
  //                  AActor* DamageCauser);
  void PointDamageTaken(AActor* DamagedActor, float Damage, class AController* InstigatedBy, FVector HitLocation,
                        class UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection,
                        const class UDamageType* DamageType, AActor* DamageCauser);

  UPROPERTY(EditAnywhere)
  float MaxHealth{100.0};
  float Health{0};
  AToonTanksGameModeBase* ToonTanksGameModeBase{nullptr};
};
