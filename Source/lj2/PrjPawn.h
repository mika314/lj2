#pragma once

#include <CoreMinimal.h>
#include <GameFramework/Pawn.h>

#include "PrjPawn.generated.h"

UCLASS()
class LJ2_API APrjPawn : public APawn
{
  GENERATED_BODY()

public:
  UPROPERTY(EditAnywhere)
  class UCapsuleComponent *capsule = nullptr;

  UPROPERTY(EditAnywhere)
  class UFloatingPawnMovement *movement = nullptr;

  APrjPawn();

protected:
  auto BeginPlay() -> void override;

public:
  auto Tick(float DeltaTime) -> void override;
  auto SetupPlayerInputComponent(class UInputComponent *PlayerInputComponent) -> void override;

private:
  auto frwd(float) -> void;
  auto sRight(float) -> void;
};
