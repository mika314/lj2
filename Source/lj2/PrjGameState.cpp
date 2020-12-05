#include "PrjGameState.h"
#include <Engine/World.h>
#include <Kismet/GameplayStatics.h>

auto APrjGameState::levelCleared() -> void
{
  UGameplayStatics::OpenLevel(GetWorld(), FName("MenuLevel"));
}
