// The Betrayed Will / وصية الغدر
// Copyright (c) 2026. All rights reserved.

#include "Core/TBWGameMode.h"
#include "Core/TBWVersion.h"
#include "Player/TBWPlayerCharacter.h"
#include "Player/TBWPlayerController.h"
#include "World/TBWEastWingSandbox.h"
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
	if (Sandbox)
	{
		return;
	}

	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	for (TActorIterator<ATBWEastWingSandbox> It(World); It; ++It)
	{
		Sandbox = *It;
		return;
	}

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	Sandbox = World->SpawnActor<ATBWEastWingSandbox>(FVector::ZeroVector, FRotator::ZeroRotator, Params);
}

void ATBWGameMode::StartPlay()
{
	EnsureSandbox();
	UE_LOG(LogTBW, Display, TEXT("The Betrayed Will %s starting. Engine lock %s. East Wing sandbox %s."),
		TBW_VERSION_STRING,
		TBW_ENGINE_LOCK,
		Sandbox ? TEXT("ready") : TEXT("MISSING"));
	Super::StartPlay();
}

void ATBWGameMode::RestartPlayer(AController* NewPlayer)
{
	EnsureSandbox();
	if (Sandbox)
	{
		RestartPlayerAtTransform(NewPlayer, Sandbox->GetEvanSpawnTransform());
		return;
	}
	Super::RestartPlayer(NewPlayer);
}
