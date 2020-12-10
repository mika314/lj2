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
  class UStaticMeshComponent *mesh2 = nullptr;

  UFUNCTION(BlueprintCallable)
  void setSettingsUi(class UUserWidget *val) { settingsUi = val; }

  UFUNCTION(BlueprintCallable)
  void setHudUi(class UUserWidget *val) { hudUi = val; }

  UFUNCTION(BlueprintCallable)
  void settingsUiClose();

  UFUNCTION(BlueprintCallable)
  class ASatellite *getLockedSatellite() const;

  UFUNCTION(BlueprintCallable)
  bool getIsHacking() const { return isHacking; }

  UFUNCTION(BlueprintCallable)
  bool isOnTheStargate() const;

  UFUNCTION(BlueprintCallable)
  float getMouseSensitivity() const;

  UFUNCTION(BlueprintCallable)
  void setMouseSensitivity(float val);

  UPROPERTY(EditAnywhere)
  class UAudioComponent *hackingSound = nullptr;

  UPROPERTY(EditAnywhere)
  class UAudioComponent *landingSound = nullptr;

  auto SetupPlayerInputComponent(class UInputComponent *) -> void override;
  auto Tick(float DeltaTime) -> void override;

private:
  auto BeginPlay() -> void override;
  auto frwd(float) -> void;
  auto hackOff() -> void;
  auto hackOn() -> void;
  auto land() -> void;
  auto landingAnimation(float dt) -> void;
  auto lookUp(float) -> void;
  auto sRight(float) -> void;
  auto settings() -> void;
  auto turnRight(float) -> void;
  auto updateRot() -> void;

  UUserWidget *settingsUi = nullptr;
  UUserWidget *hudUi = nullptr;
  class ASatellite *lockedSatellite;
  bool isHacking = false;
  bool wasHacking = false;
  bool isLanding = false;
  float rotZ = 0;
  float rotY = 0;
  float mouseSensitivity = 1;
};
