// The Betrayed Will / وصية الغدر
// Copyright (c) 2026. All rights reserved.

#include "Core/TBWGameMode.h"
#include "Core/TBWVersion.h"
#include "Player/TBWPlayerCharacter.h"
#include "Player/TBWPlayerController.h"
#include "World/TBWDevSandbox.h"
#include "UI/TBWHUD.h"
#include "EngineUtils.h"
#include "TBW.h"

ATBWGameMode::ATBWGameMode()
{
	DefaultPawnClass = ATBWPlayerCharacter::StaticClass();
	PlayerControllerClass = ATBWPlayerController::StaticClass();
	HUDClass = ATBWHUD::StaticClass();
}

void ATBWGameMode::EnsureSandbox()
{
	if (DevSandbox)
	{
		return;
	}

	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	for (TActorIterator<ATBWDevSandbox> It(World); It; ++It)
	{
		DevSandbox = *It;
		return;
	}

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	DevSandbox = World->SpawnActor<ATBWDevSandbox>(FVector::ZeroVector, FRotator::ZeroRotator, Params);
}

void ATBWGameMode::StartPlay()
{
	EnsureSandbox();
	UE_LOG(LogTBW, Display, TEXT("The Betrayed Will %s starting. Engine lock %s. L_Dev_Sandbox %s."),
		TBW_VERSION_STRING,
		TBW_ENGINE_LOCK,
		DevSandbox ? TEXT("ready") : TEXT("MISSING"));
	Super::StartPlay();
}

void ATBWGameMode::RestartPlayer(AController* NewPlayer)
{
	EnsureSandbox();
	if (DevSandbox)
	{
		RestartPlayerAtTransform(NewPlayer, DevSandbox->GetPlayerSpawnTransform());
		return;
	}
	Super::RestartPlayer(NewPlayer);
}
