// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ToonTanksGameModeBase.generated.h"

class ATank;
class AToonTanksPlayerController;

/**
 *
 */
UCLASS()
class TOONTANKS_API AToonTanksGameModeBase : public AGameModeBase {
  GENERATED_BODY()

 public:
  void ActorKilled(AActor* DeadActor, FVector HitLocation, FVector ShotFromDirection);

 protected:
  // Called when the game starts or when spawned
  virtual void BeginPlay() override;

  UFUNCTION(BlueprintImplementableEvent)
  void StartGameBPEvent();

  UFUNCTION(BlueprintImplementableEvent)
  void GameOverBPEvent(bool IsWinGame);

 private:
  uint32_t GetTargetTowerCount();

  UPROPERTY(EditAnywhere)
  USoundBase* IntroSound{nullptr};

  ATank* Tank{nullptr};
  AToonTanksPlayerController* ToonTanksPlayerController{nullptr};

  float StartDelay{3.0};
  uint32_t NumOfTowers{0};
};
