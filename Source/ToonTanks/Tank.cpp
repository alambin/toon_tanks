// Fill out your copyright notice in the Description page of Project Settings.

#include "Tank.h"

#include "Camera/CameraComponent.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/InputSettings.h"
#include "GameFramework/PlayerInput.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"

ATank::ATank()
    : SpringArm{CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"))},
      Camera{CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"))} {
  SpringArm->SetupAttachment(GetRootComponent());
  Camera->SetupAttachment(SpringArm);
}

void ATank::HandleDestruction() {
  Super::HandleDestruction();
  SetActorHiddenInGame(true);
  SetActorTickEnabled(false);
  mIsAlive = false;
}

bool ATank::IsAlive() const { return mIsAlive; }

// Called when the game starts or when spawned
void ATank::BeginPlay() {
  Super::BeginPlay();
  PlayerController = Cast<APlayerController>(GetController());
  MinCameraPitch = SpringArm->GetComponentRotation().Pitch;

  if (PlayerController) {
    TArray<FInputAxisKeyMapping> moveForwardAxisMappings;
    UInputSettings::GetInputSettings()->GetAxisMappingByName(TEXT("MoveForward"), moveForwardAxisMappings);
    for (auto const& mapping : moveForwardAxisMappings) {
      if (mapping.Scale < 0.0) {
        MoveBackwardKeys.Push(mapping.Key);
      }
    }
  }
}

// Called to bind functionality to input
void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
  Super::SetupPlayerInputComponent(PlayerInputComponent);

  PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ATank::Move);
  PlayerInputComponent->BindAxis(TEXT("BaseTurn"), this, &ATank::BaseTurn);
  PlayerInputComponent->BindAxis(TEXT("RotateCameraLeftRight"), this, &ATank::TurnCameraLeftRight);
  PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &ATank::LookUp);

  PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &ABasePawn::Fire);
}

// Called every frame
void ATank::Tick(float DeltaTime) {
  Super::Tick(DeltaTime);

  if (SpringArm && SpringArm->bEnableCameraRotationLag) {
    AlignTurretYawWithCamera();
  }

  // NOTE: This code is from original tutorial. It shows how to get point in the world, where cursor is currently
  // pointing to
  // if (PlayerController) {
  //   FHitResult hitResult;
  //   if (PlayerController->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, hitResult)) {
  //     // DrawDebugSphere(GetWorld(), hitResult.ImpactPoint, 20, 12, FColor::Red, false, -1);
  //     RotateTurret(hitResult.ImpactPoint);
  //   }
  // }
}

void ATank::Move(float Value) {
  auto deltaTime = UGameplayStatics::GetWorldDeltaSeconds(this);
  AddActorLocalOffset({Value * Speed * deltaTime, 0, 0}, true);
}

void ATank::BaseTurn(float Value) {
  // Inverse direction of turn in case we are moving backwards
  if (PlayerController && !MoveBackwardKeys.IsEmpty()) {
    for (auto const& key : MoveBackwardKeys) {
      if (PlayerController->IsInputKeyDown(key)) {
        Value = -Value;
        break;
      }
    }
  }

  auto turretWorldRotation = TurretMesh->GetComponentRotation();
  auto springArmWorldRotation = SpringArm->GetComponentRotation();
  AddActorLocalRotation({0, Value * RotationSpeed * UGameplayStatics::GetWorldDeltaSeconds(this), 0}, true);
  // Do not change rotation of other components - it should depend not on Actor rotation, but only on mouse position
  TurretMesh->SetWorldRotation(turretWorldRotation);
  SpringArm->SetWorldRotation(springArmWorldRotation);
}

void ATank::TurnCameraLeftRight(float Value) {
  SpringArm->AddRelativeRotation({0, Value * RotationSpeed * UGameplayStatics::GetWorldDeltaSeconds(this), 0}, true);
  AlignTurretYawWithCamera();
}

void ATank::LookUp(float Value) {
  // Clamp pitch angle in range [OriginalPitch; HorisontalPitch]
  auto cameraWorldRotation = Camera->GetComponentRotation();
  cameraWorldRotation.Pitch += Value * RotationSpeed * UGameplayStatics::GetWorldDeltaSeconds(this);
  cameraWorldRotation.Pitch = FMath::Clamp(cameraWorldRotation.Pitch, MinCameraPitch, 0.0);
  Camera->SetWorldRotation(cameraWorldRotation);

  AlignTurretPitchWithTarget();
}

void ATank::AlignTurretYawWithCamera() {
  // NOTE: Do NOT take rotation of spring arm, because camera is lagging behind real rotation of sprint arm
  auto cameraWorldYaw = Camera->GetComponentRotation().Yaw;
  // The same could be made by
  // auto cameraWorldYaw = PlayerController->PlayerCameraManager->GetCameraRotation().Yaw;
  auto turretWorldRotation = TurretMesh->GetComponentRotation();
  turretWorldRotation.Yaw = cameraWorldYaw;
  TurretMesh->SetWorldRotation(turretWorldRotation, true);

  // It can happen that during turning of camera left/right cursor hovers over near object. In this case pitch of turret
  // should change as well
  AlignTurretPitchWithTarget();
}

void ATank::AlignTurretPitchWithTarget() {
  if (auto hitResult = GetHitResultAtScreenCenter(); hitResult.has_value()) {
    DrawDebugSphere(GetWorld(), hitResult.value().ImpactPoint, 20, 12, FColor::Red, false, -1);
    DrawDebugSphere(GetWorld(), TurretMesh->GetComponentLocation(), 20, 12, FColor::Green, false, -1);
    DrawDebugLine(GetWorld(), TurretMesh->GetComponentLocation(), hitResult.value().ImpactPoint, FColor::Red);

    auto ToTarget = hitResult.value().ImpactPoint - TurretMesh->GetComponentLocation();
    auto ToTargetRotation = ToTarget.Rotation();

    auto turretWorldRotation = TurretMesh->GetComponentRotation();
    turretWorldRotation.Pitch = ToTargetRotation.Pitch;
    TurretMesh->SetWorldRotation(turretWorldRotation, false);
  }
}

std::optional<FHitResult> ATank::GetHitResultAtScreenCenter() const {
  int32 sx;
  int32 sy;
  PlayerController->GetViewportSize(sx, sy);
  FHitResult result;
  if (!PlayerController->GetHitResultAtScreenPosition({sx / 2.0, sy / 2.0}, ECollisionChannel::ECC_Visibility, false,
                                                      result)) {
    return {};
  }
  return {result};
}
