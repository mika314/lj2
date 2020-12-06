#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Satellite.generated.h"

UCLASS()
class LJ2_API ASatellite : public AActor
{
  GENERATED_BODY()

public:
  ASatellite();

  UPROPERTY(EditAnywhere)
  class UStaticMeshComponent *mesh = nullptr;

  UFUNCTION(BlueprintCallable)
  float getHackedPercent() const;

  UPROPERTY(EditAnywhere)
  class UAudioComponent *hackedSound = nullptr;

  auto hack(float dt) -> bool;

private:
  auto Tick(float DeltaTime) -> void override;
  auto BeginPlay() -> void override;
  float hackedPercent = 0;
  FRotator rot;
};
