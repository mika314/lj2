#include "PrjPawn.h"

APrjPawn::APrjPawn()
{
  PrimaryActorTick.bCanEverTick = true;
}

void APrjPawn::BeginPlay()
{
  Super::BeginPlay();
}

void APrjPawn::Tick(float DeltaTime)
{
  Super::Tick(DeltaTime);
}

void APrjPawn::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent)
{
  Super::SetupPlayerInputComponent(PlayerInputComponent);
}
