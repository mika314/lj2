#pragma once

#include <CoreMinimal.h>
#include <GameFramework/GameStateBase.h>

#include "PrjGameState.generated.h"

UCLASS()
class LJ2_API APrjGameState : public AGameStateBase
{
  GENERATED_BODY()
public:
  auto sateliteIsHacked() -> void { ++hackedSatellitesCount; }

  UFUNCTION(BlueprintCallable)
  int getHackedSatellitesCount() const { return hackedSatellitesCount; }

  UFUNCTION(BlueprintCallable)
  bool isGateOpen() const { return hackedSatellitesCount >= 10; }

private:
  int hackedSatellitesCount = 0;
};
