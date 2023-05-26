// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseDestructable.h"

// Sets default values
ABaseDestructable::ABaseDestructable() {
  // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
  PrimaryActorTick.bCanEverTick = true;
}

void ABaseDestructable::HandleDestruction(FVector HitLocation, FVector ShotFromDirection) {
  ApplyExplosionForceBPEvent(HitLocation, ShotFromDirection);

  // Destroy() is called in  Blueprint
  // Destroy();
}
