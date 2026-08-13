// The Betrayed Will / وصية الغدر
// Copyright (c) 2026. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TBWGameMode.generated.h"

/**
 * Default game mode. Death → checkpoint is Phase 2/8.
 * No respawn-as-a-service. Single player only.
 */
UCLASS()
class TBW_API ATBWGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ATBWGameMode();
};
