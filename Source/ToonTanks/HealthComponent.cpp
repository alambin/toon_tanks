// Fill out your copyright notice in the Description page of Project Settings.

#include "HealthComponent.h"

#include "Kismet/GameplayStatics.h"
#include "ToonTanksGameModeBase.h"

// Called when the game starts
void UHealthComponent::BeginPlay() {
  Super::BeginPlay();

  Health = MaxHealth;
  GetOwner()->OnTakePointDamage.AddDynamic(this, &UHealthComponent::PointDamageTaken);
}

void UHealthComponent::PointDamageTaken(AActor* DamagedActor, float Damage, class AController* InstigatedBy,
                                        FVector HitLocation, class UPrimitiveComponent* FHitComponent, FName BoneName,
                                        FVector ShotFromDirection, const class UDamageType* DamageType,
                                        AActor* DamageCauser) {
  Health -= Damage;
  if (Health < 0) {
    Health = 0;
  }

  if (FMath::IsNearlyZero(Health)) {
    Cast<AToonTanksGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()))
        ->ActorKilled(DamagedActor, HitLocation, ShotFromDirection);
  }
}
