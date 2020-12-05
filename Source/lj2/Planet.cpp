#include "Planet.h"
#include "log.hpp"
#include <Components/StaticMeshComponent.h>
#include <Kismet/GameplayStatics.h>
#include "PrjPawn.h"

APlanet::APlanet() : mesh(CreateDefaultSubobject<UStaticMeshComponent>("mesh"))
{
  PrimaryActorTick.bCanEverTick = true;
}

auto APlanet::BeginPlay() -> void
{
  Super::BeginPlay();
}

auto APlanet::Tick(float DeltaTime) -> void
{
  Super::Tick(DeltaTime);

  auto pawn = Cast<APrjPawn>(UGameplayStatics::GetPlayerPawn(this, 0));
  if (!pawn)
    return;

  const auto loc = pawn->GetActorLocation();

  const auto K = 0.01f;
  SetActorRotation(FRotator::MakeFromEuler(FVector{-K * loc.Y, K * loc.X, 0}));
  SetActorLocation(FVector{loc.X, loc.Y, 0});
}
