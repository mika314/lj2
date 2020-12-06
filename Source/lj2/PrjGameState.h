#pragma once

#include <CoreMinimal.h>
#include <GameFramework/GameStateBase.h>

#include "PrjGameState.generated.h"

UCLASS()
class LJ2_API APrjGameState : public AGameStateBase
{
  GENERATED_BODY()
public:
  APrjGameState();

  UFUNCTION(BlueprintCallable)
  int getHackedSatellitesCount() const;

  UFUNCTION(BlueprintCallable)
  bool isGateOpen() const;

  UFUNCTION(BlueprintCallable)
  int numberOfSatellitesNeededToClearLevel() const;

  UPROPERTY(EditAnywhere)
  class UAudioComponent *gateSound = nullptr;

  auto sateliteIsHacked() -> void;
  auto levelCleared() -> void;

private:
  int hackedSatellitesCount = 0;
};
