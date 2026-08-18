// The Betrayed Will / وصية الغدر
// Copyright (c) 2026. All rights reserved.

#include "Core/TBWWorldStateSubsystem.h"
#include "Core/TBWWorldFlags.h"
#include "Narrative/TBWDialogueSubsystem.h"
#include "Narrative/TBWObjectiveSubsystem.h"
#include "UI/TBWHUD.h"
#include "Core/TBWVersion.h"
#include "UI/TBWHUD.h"
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
			const FName FlagName(*Args[0]);
			ETBWWorldFlag Typed = ETBWWorldFlag::None;
			if (!FTBWWorldFlags::TryParse(Args[0], Typed))
			{
				UE_LOG(LogTBW, Warning, TEXT("'%s' is not a known flag. Setting it anyway (ad-hoc). tbw.Flags.List shows the typed set."), *Args[0]);
			}
			State->SetFlag(FlagName, FCString::Atoi(*Args[1]));
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

				TArray<FName> Known;
				FTBWWorldFlags::GetKnownNames(Known);
				UE_LOG(LogTBW, Display, TEXT("Known typed flags (%d):"), Known.Num());
				for (const FName& Name : Known)
				{
					UE_LOG(LogTBW, Display, TEXT("  %s = %d"), *Name.ToString(), State->GetFlag(Name));
				}
			}
		}
	}));

static FAutoConsoleCommandWithWorldAndArgs CVarDialoguePlay(
	TEXT("tbw.Dialogue.Play"),
	TEXT("Play an authored scene. Usage: tbw.Dialogue.Play VS01_OrinLastWords"),
	FConsoleCommandWithWorldAndArgsDelegate::CreateLambda([](const TArray<FString>& Args, UWorld* World)
	{
		World = TBW_CommandWorld(World);
		if (!World)
		{
			return;
		}
		UTBWDialogueSubsystem* Dialogue = World->GetSubsystem<UTBWDialogueSubsystem>();
		if (!Dialogue)
		{
			return;
		}
		if (Args.Num() < 1)
		{
			TArray<FName> Ids;
			Dialogue->GetSceneIds(Ids);
			UE_LOG(LogTBW, Display, TEXT("Scenes (%d):"), Ids.Num());
			for (const FName& Id : Ids)
			{
				UE_LOG(LogTBW, Display, TEXT("  %s"), *Id.ToString());
			}
			return;
		}
		Dialogue->PlayScene(FName(*Args[0]));
	}));

static FAutoConsoleCommandWithWorld CVarDialogueSkip(
	TEXT("tbw.Dialogue.Skip"),
	TEXT("Skip the current line, or the scene if it is skippable."),
	FConsoleCommandWithWorldDelegate::CreateLambda([](UWorld* World)
	{
		World = TBW_CommandWorld(World);
		if (!World)
		{
			return;
		}
		if (UTBWDialogueSubsystem* Dialogue = World->GetSubsystem<UTBWDialogueSubsystem>())
		{
			if (Dialogue->CanSkip())
			{
				Dialogue->Stop(true);
			}
			else
			{
				Dialogue->Advance();
			}
		}
	}));

static FAutoConsoleCommandWithWorld CVarDialogueReload(
	TEXT("tbw.Dialogue.Reload"),
	TEXT("Re-read every dialogue and objective file from disk."),
	FConsoleCommandWithWorldDelegate::CreateLambda([](UWorld* World)
	{
		World = TBW_CommandWorld(World);
		if (!World)
		{
			return;
		}
		int32 Scenes = 0;
		int32 Rules = 0;
		if (UTBWDialogueSubsystem* Dialogue = World->GetSubsystem<UTBWDialogueSubsystem>())
		{
			Scenes = Dialogue->ReloadScenes();
		}
		if (UTBWObjectiveSubsystem* Objectives = World->GetSubsystem<UTBWObjectiveSubsystem>())
		{
			Rules = Objectives->ReloadRules();
		}
		UE_LOG(LogTBW, Display, TEXT("Reloaded %d scene(s), %d objective rule(s)."), Scenes, Rules);
	}));

static FAutoConsoleCommandWithWorld CVarObjective(
	TEXT("tbw.Objective"),
	TEXT("Print the current objective and story progress."),
	FConsoleCommandWithWorldDelegate::CreateLambda([](UWorld* World)
	{
		World = TBW_CommandWorld(World);
		if (!World)
		{
			return;
		}
		if (UTBWObjectiveSubsystem* Objectives = World->GetSubsystem<UTBWObjectiveSubsystem>())
		{
			UE_LOG(LogTBW, Display, TEXT("[%s]  %s   (%.0f%% of authored rules)"),
				*Objectives->GetCurrentSegment().ToString(),
				*Objectives->GetCurrentObjective(false),
				Objectives->GetProgress() * 100.f);
		}
	}));

static FAutoConsoleCommandWithWorldAndArgs CVarLanguage(
	TEXT("tbw.Lang"),
	TEXT("Subtitle language. Usage: tbw.Lang ar | en"),
	FConsoleCommandWithWorldAndArgsDelegate::CreateLambda([](const TArray<FString>& Args, UWorld* World)
	{
		World = TBW_CommandWorld(World);
		if (!World || Args.Num() < 1)
		{
			UE_LOG(LogTBW, Warning, TEXT("Usage: tbw.Lang ar | en"));
			return;
		}
		const bool bArabic = Args[0].StartsWith(TEXT("ar"));
		if (APlayerController* PC = World->GetFirstPlayerController())
		{
			if (ATBWHUD* Hud = Cast<ATBWHUD>(PC->GetHUD()))
			{
				Hud->SetArabicUI(bArabic);
			}
		}
		UE_LOG(LogTBW, Display, TEXT("Subtitle language: %s"), bArabic ? TEXT("Arabic") : TEXT("English"));
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

static FAutoConsoleCommandWithWorldAndArgs CVarFlagsCheck(
	TEXT("tbw.Flags.Check"),
	TEXT("Return 1/0 whether a flag is set. Usage: tbw.Flags.Check <Name>"),
	FConsoleCommandWithWorldAndArgsDelegate::CreateLambda([](const TArray<FString>& Args, UWorld* World)
	{
		World = TBW_CommandWorld(World);
		if (!World || Args.Num() < 1)
		{
			UE_LOG(LogTBW, Warning, TEXT("Usage: tbw.Flags.Check <Name>"));
			return;
		}
		if (UTBWWorldStateSubsystem* State = World->GetSubsystem<UTBWWorldStateSubsystem>())
		{
			UE_LOG(LogTBW, Display, TEXT("%s -> %s"), *Args[0], State->HasFlag(FName(*Args[0])) ? TEXT("true") : TEXT("false"));
		}
	}));

static FAutoConsoleCommandWithWorldAndArgs CVarFlagsClear(
	TEXT("tbw.Flags.Clear"),
	TEXT("Clear one flag. Usage: tbw.Flags.Clear <Name>"),
	FConsoleCommandWithWorldAndArgsDelegate::CreateLambda([](const TArray<FString>& Args, UWorld* World)
	{
		World = TBW_CommandWorld(World);
		if (!World || Args.Num() < 1)
		{
			UE_LOG(LogTBW, Warning, TEXT("Usage: tbw.Flags.Clear <Name>"));
			return;
		}
		if (UTBWWorldStateSubsystem* State = World->GetSubsystem<UTBWWorldStateSubsystem>())
		{
			State->ClearFlag(FName(*Args[0]));
		}
	}));

static FAutoConsoleCommandWithWorld CVarDebugReset(
	TEXT("tbw.Debug.Reset"),
	TEXT("Clear all world flags (test state reset)."),
	FConsoleCommandWithWorldDelegate::CreateLambda([](UWorld* World)
	{
		World = TBW_CommandWorld(World);
		if (World)
		{
			if (UTBWWorldStateSubsystem* State = World->GetSubsystem<UTBWWorldStateSubsystem>())
			{
				State->ResetTestState();
			}
		}
	}));

static FAutoConsoleCommandWithWorldAndArgs CVarDebugHud(
	TEXT("tbw.Debug.Hud"),
	TEXT("Show or hide the Phase 1 debug overlay. Usage: tbw.Debug.Hud 0|1"),
	FConsoleCommandWithWorldAndArgsDelegate::CreateLambda([](const TArray<FString>& Args, UWorld* World)
	{
		World = TBW_CommandWorld(World);
		if (!World)
		{
			return;
		}
		const bool bShow = Args.Num() == 0 || Args[0] != TEXT("0");
		if (APlayerController* PC = World->GetFirstPlayerController())
		{
			if (ATBWHUD* HUD = Cast<ATBWHUD>(PC->GetHUD()))
			{
				HUD->SetDebugVisible(bShow);
			}
		}
	}));

#endif
