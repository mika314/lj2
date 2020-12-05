#include "PrjPawn.h"
#include "Satellite.h"
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

void APrjPawn::Tick(float dt)
{
  Super::Tick(dt);
  auto viewRot = GetViewRotation();
  auto actRot = GetActorRotation();
  actRot.Yaw = viewRot.Yaw;
  SetActorRotation(actRot);

  FVector loc;
  FRotator rot;
  GetActorEyesViewPoint(loc, rot);

  FCollisionQueryParams params;
  params.AddIgnoredActor(this);

  FHitResult hitResult;
  lockedSatellite = nullptr;
  if (!GetWorld()->LineTraceSingleByChannel(
        hitResult, loc, loc + rot.Vector() * 12000.f, ECollisionChannel::ECC_Visibility, params))
    return;

  lockedSatellite = Cast<ASatellite>(hitResult.Actor.Get());
  if (isHacking && lockedSatellite)
    lockedSatellite->hack(dt);
}

auto APrjPawn::SetupPlayerInputComponent(UInputComponent *in) -> void
{
  Super::SetupPlayerInputComponent(in);

  in->BindAction("Settings", IE_Pressed, this, &APrjPawn::settings);
  in->BindAction("Hack", IE_Pressed, this, &APrjPawn::hackOn);
  in->BindAction("Hack", IE_Released, this, &APrjPawn::hackOff);
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

auto APrjPawn::getLockedSatellite() const -> class ASatellite *
{
  return lockedSatellite;
}

auto APrjPawn::hackOff() -> void
{
  isHacking = false;
}

auto APrjPawn::hackOn() -> void
{
  isHacking = true;
}
