#include "log.hpp"

#include "CoreMinimal.h"

namespace Internal
{
  auto dumpLog(const char *value) -> void
  {
    UE_LOG(LogTemp, Warning, TEXT("%s"), ANSI_TO_TCHAR(value));
  }

  auto dumpLogErr(const char *value) -> void
  {
    UE_LOG(LogTemp, Error, TEXT("%s"), ANSI_TO_TCHAR(value));
  }
} // namespace Internal
