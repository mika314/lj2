#include "PrjPawn.h"
#include "PrjGameState.h"
#include "Satellite.h"
#include "log.hpp"
#include <Blueprint/UserWidget.h>
#include <Components/AudioComponent.h>
#include <Components/StaticMeshComponent.h>
#include <GameFramework/FloatingPawnMovement.h>
#include <GameFramework/PlayerController.h>
#include <GameFramework/PlayerInput.h>

APrjPawn::APrjPawn()
  : mesh(CreateDefaultSubobject<UStaticMeshComponent>("mesh")),
    movement(CreateDefaultSubobject<UFloatingPawnMovement>("movement")),
    hackingSound(CreateDefaultSubobject<UAudioComponent>("hackingSound")),
    landingSound(CreateDefaultSubobject<UAudioComponent>("landingSound"))
{
  SetRootComponent(mesh);
  hackingSound->SetupAttachment(RootComponent);
  hackingSound->SetRelativeLocation(FVector());
  landingSound->SetupAttachment(RootComponent);
  landingSound->SetRelativeLocation(FVector());

  PrimaryActorTick.bCanEverTick = true;
}

void APrjPawn::BeginPlay()
{
  Super::BeginPlay();
}

void APrjPawn::Tick(float dt)
{
  Super::Tick(dt);

  if (isLanding)
  {
    landingAnimation(dt);
    return;
  }

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
  auto isSatelliteHacked = false;
  if (isHacking && lockedSatellite)
    isSatelliteHacked = lockedSatellite->hack(dt);

  auto isHackingSnd = (isHacking && lockedSatellite && !isSatelliteHacked);
  if (isHackingSnd != wasHacking)
  {
    if (isHackingSnd)
      hackingSound->Play(rand() % 13);
    else
      hackingSound->Stop();
  }
  wasHacking = isHackingSnd;
}

auto APrjPawn::SetupPlayerInputComponent(UInputComponent *in) -> void
{
  Super::SetupPlayerInputComponent(in);

  in->BindAction("Settings", IE_Pressed, this, &APrjPawn::settings);
  in->BindAction("Hack", IE_Pressed, this, &APrjPawn::hackOn);
  in->BindAction("Hack", IE_Released, this, &APrjPawn::hackOff);
  in->BindAction("Land", IE_Pressed, this, &APrjPawn::land);
  in->BindAxis("Frwd", this, &APrjPawn::frwd);
  in->BindAxis("LookUp", this, &APrjPawn::AddControllerPitchInput);
  in->BindAxis("SRight", this, &APrjPawn::sRight);
  in->BindAxis("TurnRight", this, &APrjPawn::AddControllerYawInput);
}

auto APrjPawn::frwd(float val) -> void
{
  if (isLanding)
    return;
  if (val == 0)
    return;
  AddMovementInput(GetActorForwardVector(), val);
}

auto APrjPawn::sRight(float val) -> void
{
  if (isLanding)
    return;
  if (val == 0)
    return;
  AddMovementInput(GetActorRightVector(), val);
}

auto APrjPawn::settings() -> void
{
  if (isLanding)
    return;
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
  if (isLanding)
    return;
  isHacking = false;
}

auto APrjPawn::hackOn() -> void
{
  if (isLanding)
    return;
  isHacking = true;
}

auto APrjPawn::isOnTheStargate() const -> bool
{
  auto loc = GetActorLocation();
  const auto K = 0.01f;
  while (loc.X > 360 / K * 3 / 2)
    loc.X -= 360 / K;
  while (loc.X < 360 / K / 2)
    loc.X += 360 / K;
  while (loc.Y > 360 / K * 3 / 2)
    loc.Y -= 360 / K;
  while (loc.Y < 360 / K / 2)
    loc.Y += 360 / K;
  return (loc.X < 360 / K * 1.02 && loc.X > 360 / K * 0.98 && //
          loc.Y < 360 / K * 1.02 && loc.Y > 360 / K * 0.98);
}

auto APrjPawn::land() -> void
{
  if (isLanding)
    return;
  if (!isOnTheStargate())
    return;
  auto gs = Cast<APrjGameState>(GetWorld()->GetGameState());
  if (!gs)
    return;
  if (!gs->isGateOpen())
    return;
  isLanding = true;
  landingSound->Play(0);
}

auto APrjPawn::landingAnimation(float dt) -> void
{
  auto loc = GetActorLocation();
  const auto K = 0.01f;
  while (loc.X > 360 / K * 3 / 2)
    loc.X -= 360 / K;
  while (loc.X < 360 / K / 2)
    loc.X += 360 / K;
  while (loc.Y > 360 / K * 3 / 2)
    loc.Y -= 360 / K;
  while (loc.Y < 360 / K / 2)
    loc.Y += 360 / K;

  loc.X -= 360 / K;
  loc.Y -= 360 / K;

  auto d = FVector{0, 0, 4770} - loc;
  if (d.Size() < 10)
  {
    auto gs = Cast<APrjGameState>(GetWorld()->GetGameState());
    if (!gs)
      return;

    gs->levelCleared();
    return;
  }

  SetActorLocation(GetActorLocation() + (6000 - 4770) * d.GetSafeNormal() * dt / 15);
}
