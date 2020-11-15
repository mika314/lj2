#include "PrjPawn.h"
#include "log.hpp"
#include <Components/CapsuleComponent.h>
#include <GameFramework/FloatingPawnMovement.h>
#include <GameFramework/PlayerController.h>
#include <GameFramework/PlayerInput.h>

APrjPawn::APrjPawn()
  : capsule(CreateDefaultSubobject<UCapsuleComponent>("capsule")),
    movement(CreateDefaultSubobject<UFloatingPawnMovement>("movement"))
{
  SetRootComponent(capsule);

  PrimaryActorTick.bCanEverTick = true;
}

void APrjPawn::BeginPlay()
{
  Super::BeginPlay();
}

void APrjPawn::Tick(float DeltaTime)
{
  Super::Tick(DeltaTime);
  auto viewRot = GetViewRotation();
  auto actRot = GetActorRotation();
  actRot.Yaw = viewRot.Yaw;
  SetActorRotation(actRot);
}

auto APrjPawn::SetupPlayerInputComponent(UInputComponent *in) -> void
{
  Super::SetupPlayerInputComponent(in);

  GetWorld()->GetFirstPlayerController()->PlayerInput->SetMouseSensitivity(0.032f);

  in->BindAxis("Frwd", this, &APrjPawn::frwd);
  in->BindAxis("LookUp", this, &APrjPawn::AddControllerPitchInput);
  in->BindAxis("SRight", this, &APrjPawn::sRight);
  in->BindAxis("TurnRight", this, &APrjPawn::AddControllerYawInput);}


auto APrjPawn::frwd(float val) -> void
{
  if (val == 0)
    return;
  AddMovementInput(GetActorForwardVector(), val);
}

auto APrjPawn::sRight(float val) -> void
{
  if (val == 0)
    return;
  AddMovementInput(GetActorRightVector(), val);
}
