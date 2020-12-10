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
#include <cmath>

APrjPawn::APrjPawn()
  : mesh(CreateDefaultSubobject<UStaticMeshComponent>("mesh")),
    mesh2(CreateDefaultSubobject<UStaticMeshComponent>("mesh2")),
    hackingSound(CreateDefaultSubobject<UAudioComponent>("hackingSound")),
    landingSound(CreateDefaultSubobject<UAudioComponent>("landingSound"))
{
  SetRootComponent(mesh);
  mesh2->SetupAttachment(RootComponent);
  mesh2->SetRelativeLocation(FVector());
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

  updateRot();

  FVector loc = GetActorLocation();
  FRotator rot = mesh2->GetComponentRotation();

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
  in->BindAxis("LookUp", this, &APrjPawn::lookUp);
  in->BindAxis("SRight", this, &APrjPawn::sRight);
  in->BindAxis("TurnRight", this, &APrjPawn::turnRight);
}

auto APrjPawn::frwd(float val) -> void
{
  if (isLanding)
    return;
  if (val == 0)
    return;

  const auto rot = mesh2->GetComponentRotation();
  auto loc = GetActorLocation();
  loc = loc + 6 * rot.Vector() * val;
  loc = loc * 6000 / loc.Size();
  SetActorLocation(loc);
  updateRot();
}

auto APrjPawn::sRight(float val) -> void
{
  if (isLanding)
    return;
  if (val == 0)
    return;

  const auto vec = FRotationMatrix(mesh2->GetComponentRotation()).GetScaledAxis(EAxis::Y);
  auto loc = GetActorLocation();
  loc = loc + 6 * vec * val;
  loc = loc * 6000 / loc.Size();
  SetActorLocation(loc);
  updateRot();
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

auto APrjPawn::lookUp(float val) -> void
{
  if (val == 0)
    return;
  rotY -= mouseSensitivity * val;
  updateRot();
}

auto APrjPawn::updateRot() -> void
{
  auto loc = GetActorLocation();
  auto z = atan2(loc.Y, loc.X) * 180 / 3.1415926f;
  auto y = -atan2(sqrt(loc.Y * loc.Y + loc.X * loc.X), loc.Z) * 180 / 3.1415926f;

  mesh2->SetWorldRotation(FRotator{FRotator{0, z, 0}.Quaternion() * FRotator{y, 0, 0}.Quaternion() *
                                   FRotator{0, rotZ, 0}.Quaternion() *
                                   FRotator{rotY, 0, 0}.Quaternion()});
}

auto APrjPawn::turnRight(float val) -> void
{
  if (val == 0)
    return;
  rotZ += mouseSensitivity * val;
  updateRot();
}

auto APrjPawn::getMouseSensitivity() const -> float
{
  return mouseSensitivity;
}

auto APrjPawn::setMouseSensitivity(float val) -> void
{
  mouseSensitivity = val;
}
