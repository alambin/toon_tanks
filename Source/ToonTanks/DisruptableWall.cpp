// Fill out your copyright notice in the Description page of Project Settings.

#include "DisruptableWall.h"

// Sets default values
ADisruptableWall::ADisruptableWall() {
  // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
  PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ADisruptableWall::BeginPlay() {
  Super::BeginPlay();

  UE_LOG(LogTemp, Display, TEXT("LAMBIN: ADisruptableWall::BeginPlay() 1 spawning wall element"));

  for (int i = 0; i < NumOfElements; ++i) {
    auto elementLocation = GetActorLocation() + FVector(i * ElementSize, 0, 0);
    auto wallElement = GetWorld()->SpawnActor<ABaseDestructable>(ElementClass, elementLocation, GetActorRotation());
    wallElement->SetOwner(this);
    if (wallElement == nullptr) {
      UE_LOG(LogTemp, Display, TEXT("LAMBIN: ADisruptableWall::BeginPlay() 3 ERROR - nullptr"));
      return;
    }
  }

  UE_LOG(LogTemp, Display, TEXT("LAMBIN: ADisruptableWall::BeginPlay() 2"));
}
