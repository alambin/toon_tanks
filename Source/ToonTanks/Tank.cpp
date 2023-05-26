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
  PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ATank::Turn);
  PlayerInputComponent->BindAxis(TEXT("RotateCamera"), this, &ATank::TurnCamera);

  PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &ATank::Fire);
}

// Called every frame
void ATank::Tick(float DeltaTime) {
  Super::Tick(DeltaTime);

  if (PlayerController) {
    FHitResult hitResult;
    if (PlayerController->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, hitResult)) {
      // DrawDebugSphere(GetWorld(), hitResult.ImpactPoint, 20, 12, FColor::Red, false, -1);
      RotateTurret(hitResult.ImpactPoint);
    }
  }
}

void ATank::Move(float Value) {
  auto deltaTime = UGameplayStatics::GetWorldDeltaSeconds(this);
  AddActorLocalOffset({Value * Speed * deltaTime, 0, 0}, true);
}

void ATank::Turn(float Value) {
  auto deltaTime = UGameplayStatics::GetWorldDeltaSeconds(this);

  // TODO(alambin): implement normal camera behavior:
  // 1. There should be no limits in rotation from left to right
  // 2. Camera should always look in the same direction as turret ? May be only when RMB is pressed?

  AddActorLocalRotation({0, Value * RotationSpeed * deltaTime, 0}, true);
  if (PlayerController->IsInputKeyDown(EKeys::RightMouseButton)) {
    SpringArm->AddRelativeRotation({0, -Value * RotationSpeed * deltaTime, 0}, true);
  }
}

void ATank::TurnCamera(float Value) {
  auto deltaTime = UGameplayStatics::GetWorldDeltaSeconds(this);

  UE_LOG(LogTemp, Display, TEXT("LAMBIN: ATank::TurnCamera() 1"));

  static bool rmbPressed{false};
  if (PlayerController->IsInputKeyDown(EKeys::RightMouseButton)) {
    rmbPressed = true;
    UE_LOG(LogTemp, Display, TEXT("LAMBIN: ATank::TurnCamera() RMB"));
    SpringArm->AddRelativeRotation({0, Value * RotationSpeed * deltaTime, 0}, true);
  } else {
    if (rmbPressed) {
      // We just released RMB
      auto rotation = SpringArm->GetRelativeRotation();
      rotation.Yaw = 0;
      SpringArm->SetRelativeRotation(rotation, true);
    }
    rmbPressed = false;
  }
}
