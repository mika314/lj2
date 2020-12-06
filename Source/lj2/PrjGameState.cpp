#include "PrjGameState.h"
#include <Components/AudioComponent.h>
#include <Engine/World.h>
#include <Kismet/GameplayStatics.h>

APrjGameState::APrjGameState() : gateSound(CreateDefaultSubobject<UAudioComponent>("hackingSound"))
{
  gateSound->SetupAttachment(RootComponent);
  gateSound->SetRelativeLocation(FVector());
}

auto APrjGameState::levelCleared() -> void
{
  UGameplayStatics::OpenLevel(GetWorld(), FName("ClearedLevel"));
}

auto APrjGameState::sateliteIsHacked() -> void
{
  const auto wasOpen = isGateOpen();
  ++hackedSatellitesCount;
  if (isGateOpen() && !wasOpen)
    gateSound->Play(0);
}

auto APrjGameState::getHackedSatellitesCount() const -> int
{
  return hackedSatellitesCount;
}

auto APrjGameState::isGateOpen() const -> bool
{
  return hackedSatellitesCount >= numberOfSatellitesNeededToClearLevel();
}

auto APrjGameState::numberOfSatellitesNeededToClearLevel() const -> int
{
  return 3;
}
