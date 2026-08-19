// The Betrayed Will / وصية الغدر
// Copyright (c) 2026. All rights reserved.

#include "Save/TBWSaveSubsystem.h"
#include "Save/TBWSaveGame.h"
#include "Core/TBWWorldStateSubsystem.h"
#include "Core/TBWVersion.h"
#include "Narrative/TBWObjectiveSubsystem.h"
#include "Player/TBWPlayerCharacter.h"
#include "Player/TBWPlayerIdentityComponent.h"
#include "Player/TBWIdentityData.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Misc/ScopeExit.h"
#include "TBW.h"

const FString UTBWSaveSubsystem::AutoSlot = TEXT("TBW_Auto");
const FString UTBWSaveSubsystem::QuickSlot = TEXT("TBW_Quick");

namespace
{
	constexpr int32 UserIndex = 0;
}

float UTBWSaveSubsystem::GetSessionSeconds() const
{
	const UWorld* World = GetGameInstance() ? GetGameInstance()->GetWorld() : nullptr;
	return AccumulatedSeconds + (World ? World->GetTimeSeconds() : 0.f);
}

UTBWSaveGame* UTBWSaveSubsystem::Capture() const
{
	UWorld* World = GetGameInstance() ? GetGameInstance()->GetWorld() : nullptr;
	if (!World)
	{
		return nullptr;
	}

	UTBWSaveGame* Save = Cast<UTBWSaveGame>(
		UGameplayStatics::CreateSaveGameObject(UTBWSaveGame::StaticClass()));
	if (!Save)
	{
		return nullptr;
	}

	Save->SaveVersion = UTBWSaveGame::CurrentVersion;
	Save->GameVersion = TBW_VERSION_STRING;
	Save->SavedAtUtc = FDateTime::UtcNow();
	Save->LevelName = World->GetMapName();
	Save->PlaytimeSeconds = GetSessionSeconds();

	if (UTBWWorldStateSubsystem* State = World->GetSubsystem<UTBWWorldStateSubsystem>())
	{
		Save->Flags = State->GetAllFlags();
	}

	if (APlayerController* PC = World->GetFirstPlayerController())
	{
		if (ATBWPlayerCharacter* Pawn = Cast<ATBWPlayerCharacter>(PC->GetPawn()))
		{
			Save->PlayerLocation = Pawn->GetActorLocation();
			Save->PlayerRotation = PC->GetControlRotation();
			if (UTBWPlayerIdentityComponent* Identity = Pawn->GetIdentityComponent())
			{
				if (const UTBWIdentityData* Data = Identity->GetIdentity())
				{
					Save->IdentityId = Data->IdentityId;
				}
			}
		}
	}

	FString Segment = TEXT("-");
	FString Objective;
	if (UTBWObjectiveSubsystem* Objectives = World->GetSubsystem<UTBWObjectiveSubsystem>())
	{
		Segment = Objectives->GetCurrentSegment().ToString();
		Objective = Objectives->GetCurrentObjective(false);
	}
	Save->Summary = FString::Printf(TEXT("%s  |  %s  |  %s  |  %.0f min"),
		*Segment,
		Objective.IsEmpty() ? TEXT("(no objective)") : *Objective,
		*Save->SavedAtUtc.ToString(TEXT("%Y-%m-%d %H:%M")),
		Save->PlaytimeSeconds / 60.f);

	return Save;
}

bool UTBWSaveSubsystem::SaveToSlot(const FString& SlotName)
{
	UTBWSaveGame* Save = Capture();
	if (!Save)
	{
		UE_LOG(LogTBWSave, Error, TEXT("Save failed: no world to capture."));
		return false;
	}

	if (!UGameplayStatics::SaveGameToSlot(Save, SlotName, UserIndex))
	{
		UE_LOG(LogTBWSave, Error, TEXT("Save failed writing slot '%s'."), *SlotName);
		return false;
	}

	UE_LOG(LogTBWSave, Display, TEXT("Saved '%s': %s"), *SlotName, *Save->Summary);
	OnSaved.Broadcast(SlotName);
	return true;
}

bool UTBWSaveSubsystem::Autosave(const FString& Reason)
{
	// Autosave has its own slot on purpose: a bad autosave must never be able to
	// destroy a manual save the player is relying on.
	const bool bOk = SaveToSlot(AutoSlot);
	UE_LOG(LogTBWSave, Display, TEXT("Autosave (%s): %s"), *Reason, bOk ? TEXT("ok") : TEXT("FAILED"));
	return bOk;
}

bool UTBWSaveSubsystem::DoesSlotExist(const FString& SlotName) const
{
	return UGameplayStatics::DoesSaveGameExist(SlotName, UserIndex);
}

bool UTBWSaveSubsystem::PeekSlot(const FString& SlotName, FString& OutSummary) const
{
	OutSummary.Reset();
	if (!DoesSlotExist(SlotName))
	{
		return false;
	}
	UTBWSaveGame* Save = Cast<UTBWSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, UserIndex));
	if (!Save)
	{
		return false;
	}
	OutSummary = Save->Summary;
	return true;
}

bool UTBWSaveSubsystem::LoadFromSlot(const FString& SlotName)
{
	if (!DoesSlotExist(SlotName))
	{
		UE_LOG(LogTBWSave, Warning, TEXT("Load: slot '%s' does not exist."), *SlotName);
		return false;
	}

	UTBWSaveGame* Save = Cast<UTBWSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, UserIndex));
	if (!Save)
	{
		UE_LOG(LogTBWSave, Error, TEXT("Load: slot '%s' is corrupt or not a TBW save."), *SlotName);
		return false;
	}

	if (Save->SaveVersion != UTBWSaveGame::CurrentVersion)
	{
		// Refused, not guessed at. Silently half-loading an old save is how a
		// story game ends up in an impossible state the player cannot escape.
		UE_LOG(LogTBWSave, Error,
			TEXT("Load refused: slot '%s' is save version %d, this build expects %d."),
			*SlotName, Save->SaveVersion, UTBWSaveGame::CurrentVersion);
		return false;
	}

	return Apply(Save);
}

bool UTBWSaveSubsystem::Apply(UTBWSaveGame* Save)
{
	UWorld* World = GetGameInstance() ? GetGameInstance()->GetWorld() : nullptr;
	if (!World || !Save)
	{
		return false;
	}

	// Replaying a save writes dozens of flags. Without this guard every one of
	// them would trigger the story autosave and we would save over the load.
	TGuardValue<bool> ApplyGuard(bApplying, true);

	if (UTBWWorldStateSubsystem* State = World->GetSubsystem<UTBWWorldStateSubsystem>())
	{
		State->ClearAllFlags();
		for (const TPair<FName, int32>& Pair : Save->Flags)
		{
			State->SetFlag(Pair.Key, Pair.Value);
		}
	}

	if (APlayerController* PC = World->GetFirstPlayerController())
	{
		if (APawn* Pawn = PC->GetPawn())
		{
			Pawn->SetActorLocation(Save->PlayerLocation, false, nullptr, ETeleportType::TeleportPhysics);
		}
		PC->SetControlRotation(Save->PlayerRotation);
	}

	AccumulatedSeconds = Save->PlaytimeSeconds - World->GetTimeSeconds();

	UE_LOG(LogTBWSave, Display, TEXT("Loaded: %s"), *Save->Summary);
	OnLoaded.Broadcast(Save->LevelName);
	return true;
}

bool UTBWSaveSubsystem::DeleteSlot(const FString& SlotName)
{
	const bool bOk = UGameplayStatics::DeleteGameInSlot(SlotName, UserIndex);
	UE_LOG(LogTBWSave, Display, TEXT("Delete slot '%s': %s"), *SlotName, bOk ? TEXT("ok") : TEXT("nothing to delete"));
	return bOk;
}
