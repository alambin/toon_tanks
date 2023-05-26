// Fill out your copyright notice in the Description page of Project Settings.

#include "Tank.h"

#include "Camera/CameraComponent.h"
#include "DrawDebugHelpers.h"
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
}

// Called to bind functionality to input
void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
  Super::SetupPlayerInputComponent(PlayerInputComponent);

  PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ATank::Move);
  PlayerInputComponent->BindAxis(TEXT("BaseTurn"), this, &ATank::BaseTurn);
  PlayerInputComponent->BindAxis(TEXT("RotateCameraLeftRight"), this, &ATank::TurnCameraLeftRight);

  PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &ATank::Fire);
}

// Called every frame
void ATank::Tick(float DeltaTime) {
  Super::Tick(DeltaTime);

  // Rotate turret
  // NOTE: Do NOT take rotation of spring arm, because camera is lagging behind real rotation of sprint arm
  auto cameraWorldYaw = PlayerController->PlayerCameraManager->GetCameraRotation().Yaw;
  auto turretWorldRotation = TurretMesh->GetComponentRotation();
  turretWorldRotation.Yaw = cameraWorldYaw;
  TurretMesh->SetWorldRotation(turretWorldRotation, true);

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
  auto deltaTime = UGameplayStatics::GetWorldDeltaSeconds(this);

  auto turretWorldRotation = TurretMesh->GetComponentRotation();
  auto springArmWorldRotation = SpringArm->GetComponentRotation();

  AddActorLocalRotation({0, Value * RotationSpeed * deltaTime, 0}, true);

  // Do not change rotation of other components - it should depend not on Actor rotation, but only on mouse position
  TurretMesh->SetWorldRotation(turretWorldRotation);
  SpringArm->SetWorldRotation(springArmWorldRotation);
}

void ATank::TurnCameraLeftRight(float Value) {
  SpringArm->AddRelativeRotation({0, Value * RotationSpeed * UGameplayStatics::GetWorldDeltaSeconds(this), 0}, true);
}
