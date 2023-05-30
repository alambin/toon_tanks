// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseDestructable.h"

#include "Components/WidgetComponent.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "HealthComponent.h"

// Sets default values
ABaseDestructable::ABaseDestructable()
    : GeometryCollectionComponent{CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("Geometry Collection"))},
      HealthComponent{CreateDefaultSubobject<UHealthComponent>(TEXT("Health"))},
      HealthBarWidget{CreateDefaultSubobject<UWidgetComponent>(TEXT("Health Bar"))} {
  // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
  PrimaryActorTick.bCanEverTick = true;
  SetRootComponent(GeometryCollectionComponent);
  HealthBarWidget->SetupAttachment(GeometryCollectionComponent);
}

void ABaseDestructable::HandleDestruction(FVector HitLocation, FVector ShotFromDirection) {
  ApplyExplosionForceBPEvent(HitLocation, ShotFromDirection);

  // Destroy() is called in  Blueprint
  // Destroy();
}

// Called when the game starts or when spawned
void ABaseDestructable::BeginPlay() {
  Super::BeginPlay();
  HealthBarWidget->SetWidgetSpace(EWidgetSpace::Screen);
  HealthBarWidget->SetVisibility(false);
}
