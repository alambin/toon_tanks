// Fill out your copyright notice in the Description page of Project Settings.

#include "ToonTanksPlayerController.h"

void AToonTanksPlayerController::SetPlayerEnabledState(bool IsEnabled) {
  bShowMouseCursor = IsEnabled;
  if (IsEnabled) {
    GetPawn()->EnableInput(this);
  } else {
    GetPawn()->DisableInput(this);
  }
}