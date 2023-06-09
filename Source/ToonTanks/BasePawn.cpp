// Fill out your copyright notice in the Description page of Project Settings.

#include "BasePawn.h"

#include "Camera/CameraShakeBase.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "DrawDebugHelpers.h"
#include "HealthComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Projectile.h"
#include "Sound/SoundBase.h"

// Sets default values
ABasePawn::ABasePawn()
    : BaseMesh{CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"))},
      TurretMesh{CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret Mesh"))},
      ProjectileSpawnPoint{CreateDefaultSubobject<USceneComponent>(TEXT("Projectile Spawn Point"))},
      CapsuleComp{CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule collider"))},
      HealthComponent{CreateDefaultSubobject<UHealthComponent>(TEXT("Health"))},
      HealthBarWidget{CreateDefaultSubobject<UWidgetComponent>(TEXT("Health Bar"))} {
  // Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
  PrimaryActorTick.bCanEverTick = true;

  SetRootComponent(CapsuleComp);
  BaseMesh->SetupAttachment(CapsuleComp);
  // TurretMesh->SetupAttachment(BaseMesh);
  TurretMesh->SetupAttachment(CapsuleComp);
  ProjectileSpawnPoint->SetupAttachment(TurretMesh);
  HealthBarWidget->SetupAttachment(CapsuleComp);
}

void ABasePawn::Fire() {
  // DrawDebugSphere(GetWorld(), ProjectileSpawnPoint->GetComponentLocation(), 20, 12, FColor::Red, false, 3);
  if (ProjectileClass != nullptr) {
    auto projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, ProjectileSpawnPoint->GetComponentLocation(),
                                                          ProjectileSpawnPoint->GetComponentRotation());
    projectile->SetOwner(this);
  }
}

void ABasePawn::HandleDestruction() {
  if (DeathSound) {
    UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation());
  }

  if (DeathParticles) {
    UGameplayStatics::SpawnEmitterAtLocation(this, DeathParticles, GetActorLocation(), GetActorRotation());
  }

  if (DeathCameraShakeClass) {
    GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(DeathCameraShakeClass);
  }
}

// Called when the game starts or when spawned
void ABasePawn::BeginPlay() {
  Super::BeginPlay();
  HealthBarWidget->SetWidgetSpace(EWidgetSpace::Screen);
  HealthBarWidget->SetVisibility(false);
}

void ABasePawn::RotateTurret(FVector lookAtTarget) {
  auto ToTarget = lookAtTarget - TurretMesh->GetComponentLocation();
  auto ToTargetRotation = ToTarget.Rotation();
  ToTargetRotation.Pitch = 0;
  ToTargetRotation.Roll = 0;

  constexpr float turretRotationSpeed = 25.0;
  TurretMesh->SetWorldRotation(FMath::RInterpTo(TurretMesh->GetComponentRotation(), ToTargetRotation,
                                                UGameplayStatics::GetWorldDeltaSeconds(this), turretRotationSpeed));
}
