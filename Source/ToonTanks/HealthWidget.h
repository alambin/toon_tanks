// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "CoreMinimal.h"
// This file should always be included last - rule of UE
#include "HealthWidget.generated.h"

/**
 *
 */
UCLASS()
class TOONTANKS_API UHealthWidget : public UUserWidget {
  GENERATED_BODY()

 public:
  UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
  class UProgressBar* HealthProgressBar;
};
