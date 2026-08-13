// The Betrayed Will / وصية الغدر
// Copyright (c) 2026. All rights reserved.

#include "Core/TBWWorldStateSubsystem.h"
#include "Core/TBWVersion.h"
#include "Player/TBWPlayerCharacter.h"
#include "Player/TBWPlayerIdentityComponent.h"
#include "Player/TBWIdentityFactory.h"
#include "Player/TBWIdentityData.h"
#include "HAL/IConsoleManager.h"
#include "Engine/World.h"
#include "Engine/Engine.h"
#include "GameFramework/PlayerController.h"
#include "TBW.h"

#if !UE_BUILD_SHIPPING

static UWorld* TBW_CommandWorld(UWorld* World)
{
	if (World)
	{
		return World;
	}
	if (GEngine)
	{
		if (const FWorldContext* Ctx = GEngine->GetWorldContextFromGameViewport(GEngine->GameViewport))
		{
			return Ctx->World();
		}
	}
	return nullptr;
}

static FAutoConsoleCommandWithWorldAndArgs CVarFlagsSet(
	TEXT("tbw.Flags.Set"),
	TEXT("Set a world flag. Usage: tbw.Flags.Set <Name> <Int>"),
	FConsoleCommandWithWorldAndArgsDelegate::CreateLambda([](const TArray<FString>& Args, UWorld* World)
	{
		World = TBW_CommandWorld(World);
		if (!World || Args.Num() < 2)
		{
			UE_LOG(LogTBW, Warning, TEXT("Usage: tbw.Flags.Set <Name> <Int>"));
			return;
		}
		if (UTBWWorldStateSubsystem* State = World->GetSubsystem<UTBWWorldStateSubsystem>())
		{
			State->SetFlag(FName(*Args[0]), FCString::Atoi(*Args[1]));
		}
	}));

static FAutoConsoleCommandWithWorldAndArgs CVarFlagsGet(
	TEXT("tbw.Flags.Get"),
	TEXT("Get a world flag. Usage: tbw.Flags.Get <Name>"),
	FConsoleCommandWithWorldAndArgsDelegate::CreateLambda([](const TArray<FString>& Args, UWorld* World)
	{
		World = TBW_CommandWorld(World);
		if (!World || Args.Num() < 1)
		{
			UE_LOG(LogTBW, Warning, TEXT("Usage: tbw.Flags.Get <Name>"));
			return;
		}
		if (UTBWWorldStateSubsystem* State = World->GetSubsystem<UTBWWorldStateSubsystem>())
		{
			UE_LOG(LogTBW, Display, TEXT("%s = %d"), *Args[0], State->GetFlag(FName(*Args[0])));
		}
	}));

static FAutoConsoleCommandWithWorld CVarFlagsList(
	TEXT("tbw.Flags.List"),
	TEXT("List all world flags."),
	FConsoleCommandWithWorldDelegate::CreateLambda([](UWorld* World)
	{
		World = TBW_CommandWorld(World);
		if (World)
		{
			if (UTBWWorldStateSubsystem* State = World->GetSubsystem<UTBWWorldStateSubsystem>())
			{
				State->LogAllFlags();
			}
		}
	}));

static FAutoConsoleCommandWithWorldAndArgs CVarIdentitySet(
	TEXT("tbw.Identity.Set"),
	TEXT("Development only. Usage: tbw.Identity.Set Evan|Raynor"),
	FConsoleCommandWithWorldAndArgsDelegate::CreateLambda([](const TArray<FString>& Args, UWorld* World)
	{
		World = TBW_CommandWorld(World);
		if (!World || Args.Num() < 1)
		{
			UE_LOG(LogTBWIdentity, Warning, TEXT("Usage: tbw.Identity.Set Evan|Raynor"));
			return;
		}
		APlayerController* PC = World->GetFirstPlayerController();
		ATBWPlayerCharacter* Pawn = PC ? Cast<ATBWPlayerCharacter>(PC->GetPawn()) : nullptr;
		if (!Pawn || !Pawn->GetIdentityComponent())
		{
			return;
		}
		const FString Name = Args[0];
		UTBWIdentityData* Data = Name.Equals(TEXT("Raynor"), ESearchCase::IgnoreCase)
			? FTBWIdentityFactory::MakeRaynor(Pawn)
			: FTBWIdentityFactory::MakeEvan(Pawn);
		Pawn->GetIdentityComponent()->ApplyIdentity(Data);
	}));

static FAutoConsoleCommand CVarVersion(
	TEXT("tbw.Version"),
	TEXT("Print TBW version and engine lock."),
	FConsoleCommandDelegate::CreateLambda([]()
	{
		UE_LOG(LogTBW, Display, TEXT("The Betrayed Will %s  (engine lock %s)"), TBW_VERSION_STRING, TBW_ENGINE_LOCK);
	}));

#endif
