#include "Planet.h"
#include "PrjGameState.h"
#include "PrjPawn.h"
#include "log.hpp"
#include <Components/StaticMeshComponent.h>
#include <Kismet/GameplayStatics.h>

APlanet::APlanet()
  : mesh(CreateDefaultSubobject<UStaticMeshComponent>("mesh")),
    starGate(CreateDefaultSubobject<UStaticMeshComponent>("starGate"))
{
  SetRootComponent(mesh);
  starGate->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
  PrimaryActorTick.bCanEverTick = true;
}

auto APlanet::BeginPlay() -> void
{
  Super::BeginPlay();
}

auto APlanet::Tick(float DeltaTime) -> void
{
  Super::Tick(DeltaTime);

  {
    auto gs = Cast<APrjGameState>(GetWorld()->GetGameState());
    if (!gs)
      return;
    starGate->SetVisibility(gs->isGateOpen());
  }

  auto pawn = Cast<APrjPawn>(UGameplayStatics::GetPlayerPawn(this, 0));
  if (!pawn)
    return;

  const auto loc = pawn->GetActorLocation();

  const auto K = 0.01f;
  SetActorRotation(FRotator::MakeFromEuler(FVector{-K * loc.Y, K * loc.X, 0}));
  SetActorLocation(FVector{loc.X, loc.Y, 0});
}
