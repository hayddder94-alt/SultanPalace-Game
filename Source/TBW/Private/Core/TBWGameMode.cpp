// The Betrayed Will / وصية الغدر
// Copyright (c) 2026. All rights reserved.

#include "Core/TBWGameMode.h"
#include "Core/TBWVersion.h"
#include "Player/TBWPlayerCharacter.h"
#include "Player/TBWPlayerController.h"
#include "World/TBWDevSandbox.h"
#include "UI/TBWHUD.h"
#include "Core/TBWWorldStateSubsystem.h"
#include "Core/TBWWorldFlags.h"
#include "Save/TBWSaveSubsystem.h"
#include "Engine/GameInstance.h"
#include "EngineUtils.h"
#include "GameFramework/PlayerStart.h"
#include "TBW.h"

ATBWGameMode::ATBWGameMode()
{
	DefaultPawnClass = ATBWPlayerCharacter::StaticClass();
	PlayerControllerClass = ATBWPlayerController::StaticClass();
	HUDClass = ATBWHUD::StaticClass();
}

bool ATBWGameMode::IsAuthoredLevel() const
{
	// An authored map ships its own geometry and at least one PlayerStart.
	// The runtime greybox must never be spawned into it, or the palace fills
	// with debug boxes and a second floor at Z=0.
	const UWorld* World = GetWorld();
	if (!World)
	{
		return false;
	}
	for (TActorIterator<APlayerStart> It(*const_cast<UWorld*>(World)); It; ++It)
	{
		return true;
	}
	return false;
}

void ATBWGameMode::EnsureSandbox()
{
	if (DevSandbox)
	{
		return;
	}

	if (IsAuthoredLevel())
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

void ATBWGameMode::HandleStoryFlagChanged(FName Flag, int32 NewValue)
{
	UWorld* World = GetWorld();
	if (!World || NewValue == 0)
	{
		return;
	}

	// Only authored story beats are checkpoints. Ad-hoc debug flags are not.
	ETBWWorldFlag Typed = ETBWWorldFlag::None;
	if (!FTBWWorldFlags::TryParse(Flag.ToString(), Typed))
	{
		return;
	}

	UGameInstance* GI = World->GetGameInstance();
	UTBWSaveSubsystem* Saves = GI ? GI->GetSubsystem<UTBWSaveSubsystem>() : nullptr;
	if (!Saves || Saves->IsApplying())
	{
		return;
	}

	// A scene can set several flags at once; one checkpoint is enough.
	const float Now = World->GetTimeSeconds();
	if (Now - LastAutosaveTime < 3.f)
	{
		return;
	}
	LastAutosaveTime = Now;
	Saves->Autosave(Flag.ToString());
}

void ATBWGameMode::StartPlay()
{
	EnsureSandbox();

	if (UWorld* World = GetWorld())
	{
		if (UTBWWorldStateSubsystem* State = World->GetSubsystem<UTBWWorldStateSubsystem>())
		{
			State->OnFlagChanged.AddDynamic(this, &ATBWGameMode::HandleStoryFlagChanged);
		}
	}

	UE_LOG(LogTBW, Display, TEXT("The Betrayed Will %s starting. Engine lock %s. Level %s. Greybox %s."),
		TBW_VERSION_STRING,
		TBW_ENGINE_LOCK,
		IsAuthoredLevel() ? TEXT("AUTHORED") : TEXT("dev"),
		DevSandbox ? TEXT("ready") : (IsAuthoredLevel() ? TEXT("suppressed (authored level)") : TEXT("MISSING")));
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
