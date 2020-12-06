#include "Satellite.h"
#include "PrjGameState.h"
#include "PrjPawn.h"
#include "log.hpp"
#include <Components/AudioComponent.h>
#include <Components/StaticMeshComponent.h>
#include <Kismet/GameplayStatics.h>
#include <Particles/ParticleSystemComponent.h>

ASatellite::ASatellite()
  : mesh(CreateDefaultSubobject<UStaticMeshComponent>("mesh")),
    hackedSound(CreateDefaultSubobject<UAudioComponent>("hackedSound")),
    fire(CreateDefaultSubobject<UParticleSystemComponent>("fire")),
    rot(rand() % 360, rand() % 360, rand() % 360)
{
  SetRootComponent(mesh);
  hackedSound->SetupAttachment(RootComponent);
  hackedSound->SetRelativeLocation(FVector());
  fire->SetupAttachment(RootComponent);
  fire->SetRelativeLocation(FVector());

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
  auto pawn = Cast<APrjPawn>(UGameplayStatics::GetPlayerPawn(this, 0));
  if (!pawn)
    return;

  const auto pawnLoc = pawn->GetActorLocation();
  auto loc = rot.RotateVector(FVector(0, 6000 * sin(0.1 * time), 6000 * cos(0.1 * time)));
  const auto K = 0.01f;
  SetActorLocation(
    FRotator::MakeFromEuler(FVector{-K * pawnLoc.Y, K * pawnLoc.X, 0}).RotateVector(loc) +
    FVector{pawnLoc.X, pawnLoc.Y, 0});
}

auto ASatellite::getHackedPercent() const -> float
{
  return hackedPercent;
}

auto ASatellite::hack(float dt) -> bool
{
  if (hackedPercent >= 100)
    return true;
  hackedPercent += 16 * dt;
  if (hackedPercent >= 100)
  {
    auto gs = Cast<APrjGameState>(GetWorld()->GetGameState());
    if (!gs)
      return false;
    hackedSound->Play(0);
    gs->sateliteIsHacked();
    fire->ActivateSystem(true);
    return true;
  }
  return false;
}
