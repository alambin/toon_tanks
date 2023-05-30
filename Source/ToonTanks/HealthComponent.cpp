// Fill out your copyright notice in the Description page of Project Settings.

#include "HealthComponent.h"

#include "Components/WidgetComponent.h"
#include "HealthWidget.h"
#include "Kismet/GameplayStatics.h"
#include "ToonTanksGameModeBase.h"

// Called when the game starts
void UHealthComponent::BeginPlay() {
  Super::BeginPlay();

  Health = MaxHealth;
  GetOwner()->OnTakePointDamage.AddDynamic(this, &UHealthComponent::PointDamageTaken);
}

void UHealthComponent::PointDamageTaken(AActor* DamagedActor, float Damage, class AController* InstigatedBy,
                                        FVector HitLocation, class UPrimitiveComponent* FHitComponent, FName BoneName,
                                        FVector ShotFromDirection, const class UDamageType* DamageType,
                                        AActor* DamageCauser) {
  Health -= Damage;
  if (Health < 0) {
    Health = 0;
  }

  if (FMath::IsNearlyZero(Health)) {
    Cast<AToonTanksGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()))
        ->ActorKilled(DamagedActor, HitLocation, ShotFromDirection);
  }

  // Check if owner has health bar widget component and if that component holds reference to health bar widget (ex.
  // Tank doesn't have this widget, because it displays its health on HUD)
  if (auto healthBarWidgetComponent =
          Cast<UWidgetComponent>(GetOwner()->GetDefaultSubobjectByName(TEXT("Health Bar")))) {
    if (auto widget = healthBarWidgetComponent->GetWidget()) {
      if (auto healthWidget = Cast<UHealthWidget>(widget)) {
        healthBarWidgetComponent->SetVisibility(!FMath::IsNearlyZero(Health));
        healthWidget->HealthProgressBar->SetPercent(Health / MaxHealth);
      }
    }
  }
}
