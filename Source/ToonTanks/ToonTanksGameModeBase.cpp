// Fill out your copyright notice in the Description page of Project Settings.

#include "ToonTanksGameModeBase.h"

#include "BaseDestructable.h"
#include "BasePawn.h"
#include "Kismet/GameplayStatics.h"
#include "Tank.h"
#include "TimerManager.h"
#include "ToonTanksPlayerController.h"
#include "Tower.h"

void AToonTanksGameModeBase::ActorKilled(AActor* DeadActor, FVector HitLocation, FVector ShotFromDirection) {
  if (DeadActor == Tank) {
    Tank->HandleDestruction();
    if (ToonTanksPlayerController) {
      ToonTanksPlayerController->SetPlayerEnabledState(false);
    }
    GameOverBPEvent(false);
  } else if (auto tower = Cast<ATower>(DeadActor)) {
    tower->HandleDestruction();
    --NumOfTowers;
    if (NumOfTowers == 0) {
      GameOverBPEvent(true);
    }
  } else if (auto destructable = Cast<ABaseDestructable>(DeadActor)) {
    destructable->HandleDestruction(HitLocation, ShotFromDirection);
  }

  // Another option:
  // auto basePawn = Cast<ABasePawn>(DeadActor);
  // if (basePawn) {
  //   basePawn->HandleDestruction();
  //   if (auto toonTanksController = Cast<AToonTanksPlayerController>(basePawn->GetController())) {
  //     // Turrets don't have controller, so this code will be executed only for Tank
  //     toonTanksController->SetPlayerEnabledState(false);
  //   }
  // }
}

// Called when the game starts or when spawned
void AToonTanksGameModeBase::BeginPlay() {
  Super::BeginPlay();

  // TODO(alambin): uncomment
  // if (IntroSound) {
  //   UGameplayStatics::PlaySound2D(this, IntroSound);
  // }

  // TODO(alambin): uncomment
  // StartGameBPEvent();

  Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));
  NumOfTowers = GetTargetTowerCount();

  ToonTanksPlayerController = Cast<AToonTanksPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
  if (ToonTanksPlayerController) {
    ToonTanksPlayerController->SetPlayerEnabledState(false);

    // TODO(alambin): uncomment
    ToonTanksPlayerController->SetPlayerEnabledState(true);
    // FTimerHandle PlayerStartTimerHandle;
    // FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(
    //     ToonTanksPlayerController, &AToonTanksPlayerController::SetPlayerEnabledState, true);
    // GetWorldTimerManager().SetTimer(PlayerStartTimerHandle, TimerDelegate, StartDelay, false);
  }
}

uint32_t AToonTanksGameModeBase::GetTargetTowerCount() {
  TArray<AActor*> result;
  UGameplayStatics::GetAllActorsOfClass(this, ATower::StaticClass(), result);
  return result.Num();
}
