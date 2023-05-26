// Fill out your copyright notice in the Description page of Project Settings.

#include "HealthComponent.h"

#include "Kismet/GameplayStatics.h"
#include "ToonTanksGameModeBase.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent() {
  // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these
  // features off to improve performance if you don't need them.
  PrimaryComponentTick.bCanEverTick = true;

  // ...
}

// Called when the game starts
void UHealthComponent::BeginPlay() {
  Super::BeginPlay();

  Health = MaxHealth;
  // GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::DamageTaken);
  GetOwner()->OnTakePointDamage.AddDynamic(this, &UHealthComponent::PointDamageTaken);
}

// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                     FActorComponentTickFunction* ThisTickFunction) {
  Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

  // ...
}

void UHealthComponent::PointDamageTaken(AActor* DamagedActor, float Damage, class AController* InstigatedBy,
                                        FVector HitLocation, class UPrimitiveComponent* FHitComponent, FName BoneName,
                                        FVector ShotFromDirection, const class UDamageType* DamageType,
                                        AActor* DamageCauser) {
  // void UHealthComponent::DamageTaken(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
  //                                    AController* InstigatedBy, AActor* DamageCauser) {
  UE_LOG(LogTemp, Display, TEXT("LAMBIN: UHealthComponent::DamageTaken() 1 Damage = '%f'"), Damage);

  Health -= Damage;
  if (Health < 0) {
    Health = 0;
  }

  if (FMath::IsNearlyZero(Health)) {
    UE_LOG(LogTemp, Display, TEXT("LAMBIN: UHealthComponent::DamageTaken() 2"));
    Cast<AToonTanksGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()))
        ->ActorKilled(DamagedActor, HitLocation, ShotFromDirection);
  }
}
