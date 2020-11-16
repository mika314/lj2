#include "PrjPawn.h"
#include "log.hpp"
#include <Blueprint/UserWidget.h>
#include <Components/StaticMeshComponent.h>
#include <GameFramework/FloatingPawnMovement.h>
#include <GameFramework/PlayerController.h>
#include <GameFramework/PlayerInput.h>

APrjPawn::APrjPawn()
  : mesh(CreateDefaultSubobject<UStaticMeshComponent>("mesh")),
    movement(CreateDefaultSubobject<UFloatingPawnMovement>("movement"))
{
  SetRootComponent(mesh);

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

  in->BindAction("Settings", IE_Pressed, this, &APrjPawn::settings);
  in->BindAxis("Frwd", this, &APrjPawn::frwd);
  in->BindAxis("LookUp", this, &APrjPawn::AddControllerPitchInput);
  in->BindAxis("SRight", this, &APrjPawn::sRight);
  in->BindAxis("TurnRight", this, &APrjPawn::AddControllerYawInput);
}

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

auto APrjPawn::settings() -> void
{
  auto playerController = GetWorld()->GetFirstPlayerController();
  settingsUi->SetVisibility(ESlateVisibility::Visible);
  hudUi->SetVisibility(ESlateVisibility::Hidden);
  playerController->SetInputMode(FInputModeUIOnly{});
  playerController->bShowMouseCursor = true;
}

auto APrjPawn::settingsUiClose() -> void
{
  auto playerController = GetWorld()->GetFirstPlayerController();
  settingsUi->SetVisibility(ESlateVisibility::Hidden);
  hudUi->SetVisibility(ESlateVisibility::Visible);
  playerController->SetInputMode(FInputModeGameOnly{});
  playerController->bShowMouseCursor = false;
}
