#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

#include "PrjPawn.generated.h"

UCLASS()
class LJ2_API APrjPawn : public APawn
{
  GENERATED_BODY()

public:
  APrjPawn();

protected:
  virtual void BeginPlay() override;

public:
  virtual void Tick(float DeltaTime) override;
  virtual void SetupPlayerInputComponent(class UInputComponent *PlayerInputComponent) override;
};
