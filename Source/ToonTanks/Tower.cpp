// Fill out your copyright notice in the Description page of Project Settings.

#include "Tower.h"

#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Tank.h"
#include "TimerManager.h"

void ATower::HandleDestruction() {
  Super::HandleDestruction();
  Destroy();
}

// Called every frame
void ATower::Tick(float DeltaTime) {
  Super::Tick(DeltaTime);

  if (IsTankInFireRange()) {
    RotateTurret(Tank->GetActorLocation());
  }
}

// Called when the game starts or when spawned
void ATower::BeginPlay() {
  Super::BeginPlay();

  Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

  GetWorldTimerManager().SetTimer(FireRateTimerHandle, this, &ATower::CheckFireConditions, FireRate, true);
}

void ATower::CheckFireConditions() {
  if (IsTankInFireRange()) {
    Fire();
  }
}

bool ATower::IsTankInFireRange() const {
  if (Tank && (FVector::Dist(GetActorLocation(), Tank->GetActorLocation()) <= FireRange) && Tank->IsAlive()) {
    return true;
  }
  return false;
}
