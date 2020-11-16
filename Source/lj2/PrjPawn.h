#pragma once

#include <CoreMinimal.h>
#include <GameFramework/Pawn.h>

#include "PrjPawn.generated.h"

UCLASS()
class LJ2_API APrjPawn : public APawn
{
  GENERATED_BODY()

public:
  APrjPawn();

  UPROPERTY(EditAnywhere)
  class UStaticMeshComponent *mesh = nullptr;

  UPROPERTY(EditAnywhere)
  class UFloatingPawnMovement *movement = nullptr;

  UFUNCTION(BlueprintCallable)
  void setSettingsUi(class UUserWidget *val) { settingsUi = val; }

  UFUNCTION(BlueprintCallable)
  void setHudUi(class UUserWidget *val) { hudUi = val; }

  UFUNCTION(BlueprintCallable)
  void settingsUiClose();

  auto SetupPlayerInputComponent(class UInputComponent *) -> void override;
  auto Tick(float DeltaTime) -> void override;

private:
  auto BeginPlay() -> void override;
  auto frwd(float) -> void;
  auto sRight(float) -> void;
  auto settings() -> void;

  UUserWidget *settingsUi = nullptr;
  UUserWidget *hudUi = nullptr;
};
