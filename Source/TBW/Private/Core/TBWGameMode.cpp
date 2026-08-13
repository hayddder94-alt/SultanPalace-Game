// The Betrayed Will / وصية الغدر
// Copyright (c) 2026. All rights reserved.

#include "Core/TBWGameMode.h"
#include "Player/TBWPlayerCharacter.h"
#include "Player/TBWPlayerController.h"

ATBWGameMode::ATBWGameMode()
{
	DefaultPawnClass = ATBWPlayerCharacter::StaticClass();
	PlayerControllerClass = ATBWPlayerController::StaticClass();
}
