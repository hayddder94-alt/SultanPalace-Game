// The Betrayed Will / وصية الغدر
// Copyright (c) 2026. All rights reserved.

#include "Player/TBWPlayerController.h"
#include "Input/TBWInputConfig.h"
#include "UI/TBWHUD.h"
#include "EnhancedInputSubsystems.h"
#include "TBW.h"

ATBWPlayerController::ATBWPlayerController()
{
	bShowMouseCursor = false;
	bEnableClickEvents = false;
	bEnableMouseOverEvents = false;
}

void ATBWPlayerController::EnsureInputConfig()
{
	if (!InputConfig)
	{
		InputConfig = NewObject<UTBWInputConfig>(this, TEXT("TBWInputConfig"));
		InputConfig->Build();
	}
}

void ATBWPlayerController::PushMapping()
{
	EnsureInputConfig();
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		UInputMappingContext* Context = OverrideMappingContext
			? OverrideMappingContext.Get()
			: (InputConfig ? InputConfig->MappingContext.Get() : nullptr);
		if (Context)
		{
			Subsystem->AddMappingContext(Context, 0);
		}
	}
}

void ATBWPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	PushMapping();
}

void ATBWPlayerController::BeginPlay()
{
	Super::BeginPlay();
	PushMapping();
	if (PlayerCameraManager)
	{
		PlayerCameraManager->ViewPitchMin = -55.f;
		PlayerCameraManager->ViewPitchMax = 70.f;
	}
	UE_LOG(LogTBW, Log, TEXT("PlayerController ready. Input bootstrapped. Pitch clamped."));
}

void ATBWPlayerController::TogglePauseMenu()
{
	bGameplayPaused = !bGameplayPaused;
	SetPause(bGameplayPaused);
	if (ATBWHUD* TBWHUD = Cast<ATBWHUD>(GetHUD()))
	{
		TBWHUD->SetPausedBanner(bGameplayPaused);
	}
	UE_LOG(LogTBW, Log, TEXT("Pause = %s"), bGameplayPaused ? TEXT("true") : TEXT("false"));
}
