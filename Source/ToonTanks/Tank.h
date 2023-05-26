// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BasePawn.h"
#include "CoreMinimal.h"
// This file should always be included last - rule of UE
#include "Tank.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputComponent;
class APlayerController;
struct FInputAxisKeyMapping;

/**
 *
 */
UCLASS()
class TOONTANKS_API ATank : public ABasePawn {
  GENERATED_BODY()

 public:
  ATank();
  void HandleDestruction() override;
  bool IsAlive() const;

 protected:
  // Called when the game starts or when spawned
  virtual void BeginPlay() override;

 private:
  // Called to bind functionality to input
  virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

  // Called every frame
  virtual void Tick(float DeltaTime) override;

  void Move(float Value);
  void BaseTurn(float Value);
  void TurnCameraLeftRight(float Value);
  void LookUp(float Value);
  void AlignTurretWithCameraYaw();

  UPROPERTY(EditAnywhere)
  USpringArmComponent* SpringArm{nullptr};
  UPROPERTY(EditAnywhere)
  UCameraComponent* Camera{nullptr};

  UPROPERTY(EditAnywhere, Category = "Movement")
  float Speed{600.0};
  UPROPERTY(EditAnywhere, Category = "Movement")
  float RotationSpeed{150.0};

  APlayerController* PlayerController{nullptr};
  float MinCameraPitch{-90.0};
  TArray<FKey> MoveBackwardKeys;

  bool mIsAlive{true};
};
