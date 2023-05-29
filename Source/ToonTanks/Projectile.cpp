// Fill out your copyright notice in the Description page of Project Settings.

#include "Projectile.h"

#include "Camera/CameraShakeBase.h"
#include "Field/FieldSystemActor.h"
#include "GameFramework/DamageType.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundBase.h"

// Sets default values
AProjectile::AProjectile()
    : Mesh{CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Static Mesh"))},
      SmokeTrail{CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Smoke Trail"))},
      MovementComponent{CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement Component"))} {
  // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
  PrimaryActorTick.bCanEverTick = false;

  SetRootComponent(Mesh);
  SmokeTrail->SetupAttachment(Mesh);

  // MovementComponent->SetupAttachment(Mesh);
  MovementComponent->InitialSpeed = StartingSpeed;
  MovementComponent->MaxSpeed = MaximumSpeed;

  // Ignore gravity
  MovementComponent->ProjectileGravityScale = 0;
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay() {
  Super::BeginPlay();

  Mesh->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);

  if (LaunchSound) {
    UGameplayStatics::PlaySoundAtLocation(this, LaunchSound, GetActorLocation());
  }
}

void AProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                        FVector NormalImpulse, const FHitResult& Hit) {
  auto owner = GetOwner();
  if (owner == nullptr) {
    Destroy();
    return;
  }

  UE_LOG(LogTemp, Display, TEXT("LAMBIN: AProjectile::OnHit() 1"));

  if (OtherActor && (OtherActor != this) && (OtherActor != owner)) {
    UGameplayStatics::ApplyPointDamage(OtherActor, Damage, GetActorForwardVector(), Hit,
                                       owner->GetInstigatorController(), this, UDamageType::StaticClass());
    if (HitSound) {
      UGameplayStatics::PlaySoundAtLocation(this, HitSound, Hit.Location);
    }
    if (HitParticles) {
      UGameplayStatics::SpawnEmitterAtLocation(this, HitParticles, Hit.Location, GetActorRotation());
    }
    if (HitCameraShakeClass) {
      GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(HitCameraShakeClass);
    }

    UE_LOG(LogTemp, Display, TEXT("LAMBIN: AProjectile::OnHit() 2"));

    // NOTE!
    // Had to use applying of force from Blueprint, as it is described on UE page
    // https://docs.unrealengine.com/5.0/en-US/destruction-quick-start/ Solutions I tried:
    // 1. Radial force.
    //    OtherComp->AddRadialForce(/*Hit.Location*/OtherActor->GetActorLocation(), 500, GetVelocity().Length()*100000,
    //    ERadialImpulseFalloff::RIF_Constant); By some reason should be applied not to hit location (box just slides),
    //    but to actor location. Damage threshold was set to 100, which is low enough Requires enormous force by some
    //    reason. After hit, object is first pushed back a litle bit and then it is destroyed on pieces, when it lands.
    // 2. Simple force
    //    OtherComp->AddForceAtLocation(GetVelocity() * 100000, Hit.Location);
    //    The same - requires enormous force
    //

    // ApplyExplosionForceBPEvent(OtherComp, Hit.Location);

    Destroy();
  }
}
