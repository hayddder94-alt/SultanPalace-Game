// The Betrayed Will / وصية الغدر
// Copyright (c) 2026. All rights reserved.

#include "Player/TBWPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "TBW.h"

ATBWPlayerController::ATBWPlayerController()
{
	bShowMouseCursor = false;
}

void ATBWPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		if (DefaultMappingContext)
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
		else
		{
			UE_LOG(LogTBW, Warning, TEXT("TBWPlayerController: DefaultMappingContext is not set. Assign IMC_TBW_Default in Phase 1."));
		}
	}
}
