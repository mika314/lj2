#include "Satellite.h"
#include <Components/StaticMeshComponent.h>

ASatellite::ASatellite() : mesh(CreateDefaultSubobject<UStaticMeshComponent>("mesh"))
{
  SetRootComponent(mesh);

  PrimaryActorTick.bCanEverTick = true;
}

auto ASatellite::BeginPlay() -> void
{
  Super::BeginPlay();
}

auto ASatellite::Tick(float DeltaTime) -> void
{
  Super::Tick(DeltaTime);
  const auto time = GetWorld()->GetTimeSeconds();
  SetActorLocation(FVector(0, 6000 * sin(0.1 * time), 6000 * cos(0.1 * time)));
}

auto ASatellite::getHackedPercent() const -> float
{
  return hackedPercent;
}

auto ASatellite::hack(float dt) -> void
{
  if (hackedPercent >= 100)
    return;
  hackedPercent += 16 * dt;
}
