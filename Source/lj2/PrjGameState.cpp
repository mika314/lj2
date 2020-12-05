#include "PrjGameState.h"
#include <Engine/World.h>
#include <Kismet/GameplayStatics.h>

auto APrjGameState::levelCleared() -> void
{
  UGameplayStatics::OpenLevel(GetWorld(), FName("ClearedLevel"));
}

auto APrjGameState::sateliteIsHacked() -> void
{
  ++hackedSatellitesCount;
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
  return 10;
}
