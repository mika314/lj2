#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Planet.generated.h"

UCLASS()
class LJ2_API APlanet : public AActor
{
  GENERATED_BODY()

public:
  APlanet();

  UPROPERTY(EditAnywhere)
  class UStaticMeshComponent *mesh = nullptr;

private:
  auto BeginPlay() -> void override;
  auto Tick(float DeltaTime) -> void override;
};
