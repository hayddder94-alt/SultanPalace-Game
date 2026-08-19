// The Betrayed Will / وصية الغدر
// Copyright (c) 2026. All rights reserved.

#include "Core/TBWSelfTest.h"
#include "Core/TBWWorldStateSubsystem.h"
#include "Core/TBWWorldFlags.h"
#include "Narrative/TBWDialogueSubsystem.h"
#include "Narrative/TBWObjectiveSubsystem.h"
#include "Save/TBWSaveSubsystem.h"
#include "Characters/TBWStoryCharacter.h"
#include "Interaction/TBWInteractableActor.h"
#include "Player/TBWPlayerCharacter.h"
#include "UI/TBWHUD.h"
#include "Engine/World.h"
#include "Engine/GameInstance.h"
#include "EngineUtils.h"
#include "GameFramework/PlayerController.h"
#include "TBW.h"

namespace
{
	const FString TestSlot = TEXT("TBW_SelfTest");
	const FName ProbeFlag = TEXT("ClueFound_01");
}

void UTBWSelfTest::Check(const FString& Name, bool bCondition, const FString& Detail)
{
	if (bCondition)
	{
		++Passed;
		Report.Add(FString::Printf(TEXT("  PASS  %s%s"), *Name,
			Detail.IsEmpty() ? TEXT("") : *FString::Printf(TEXT("  (%s)"), *Detail)));
	}
	else
	{
		++Failed;
		Report.Add(FString::Printf(TEXT("  FAIL  %s%s"), *Name,
			Detail.IsEmpty() ? TEXT("") : *FString::Printf(TEXT("  (%s)"), *Detail)));
	}
}

void UTBWSelfTest::TestWorldState()
{
	UWorld* World = GetWorld();
	UTBWWorldStateSubsystem* State = World ? World->GetSubsystem<UTBWWorldStateSubsystem>() : nullptr;
	Check(TEXT("world state subsystem exists"), State != nullptr);
	if (!State)
	{
		return;
	}

	const int32 Before = State->GetFlag(ProbeFlag);
	State->SetFlag(ProbeFlag, 7);
	Check(TEXT("flag write then read"), State->GetFlag(ProbeFlag) == 7);
	Check(TEXT("HasFlag agrees with GetFlag"), State->HasFlag(ProbeFlag));
	State->ClearFlag(ProbeFlag);
	Check(TEXT("flag clear"), !State->HasFlag(ProbeFlag));
	if (Before != 0)
	{
		State->SetFlag(ProbeFlag, Before);
	}

	TArray<FName> Known;
	FTBWWorldFlags::GetKnownNames(Known);
	Check(TEXT("typed flag table populated"), Known.Num() >= 10,
		FString::Printf(TEXT("%d flags"), Known.Num()));

	ETBWWorldFlag Parsed = ETBWWorldFlag::None;
	Check(TEXT("flag name parses back to the enum"),
		FTBWWorldFlags::TryParse(TEXT("WillWasRead"), Parsed));
}

void UTBWSelfTest::TestDialogue()
{
	UWorld* World = GetWorld();
	UTBWDialogueSubsystem* Dialogue = World ? World->GetSubsystem<UTBWDialogueSubsystem>() : nullptr;
	Check(TEXT("dialogue subsystem exists"), Dialogue != nullptr);
	if (!Dialogue)
	{
		return;
	}

	TArray<FName> Ids;
	Dialogue->GetSceneIds(Ids);
	Check(TEXT("dialogue scenes loaded from disk"), Ids.Num() > 0,
		FString::Printf(TEXT("%d scene(s)"), Ids.Num()));
	if (Ids.Num() == 0)
	{
		return;
	}

	const FName First = Ids[0];
	Check(TEXT("scene starts"), Dialogue->PlayScene(First), First.ToString());
	Check(TEXT("subtitle is not empty while playing"), !Dialogue->GetSubtitle().IsEmpty());

	// Arabic must survive the JSON round trip. If the file said Arabic and the
	// subtitle has no Arabic codepoints, the text pipeline is broken again.
	const FString Subtitle = Dialogue->GetSubtitle();
	bool bHasArabic = false;
	for (const TCHAR C : Subtitle)
	{
		if (C >= 0x0600 && C <= 0x06FF)
		{
			bHasArabic = true;
			break;
		}
	}
	Check(TEXT("Arabic survives JSON to subtitle"), bHasArabic || !Dialogue->IsArabic(),
		bHasArabic ? TEXT("Arabic codepoints present") : TEXT("no Arabic in the active line"));

	const FString FirstLine = Dialogue->GetSubtitle();
	Dialogue->Advance();
	Check(TEXT("advance moves to the next line"), Dialogue->GetSubtitle() != FirstLine);

	Dialogue->Stop(false);
	Check(TEXT("stop ends playback"), !Dialogue->IsPlaying());
	Check(TEXT("subtitle clears when idle"), Dialogue->GetSubtitle().IsEmpty());
}

void UTBWSelfTest::TestObjectives()
{
	UWorld* World = GetWorld();
	UTBWObjectiveSubsystem* Objectives = World ? World->GetSubsystem<UTBWObjectiveSubsystem>() : nullptr;
	Check(TEXT("objective subsystem exists"), Objectives != nullptr);
	if (!Objectives)
	{
		return;
	}

	Check(TEXT("objective rules loaded"), Objectives->GetRuleCount() > 0,
		FString::Printf(TEXT("%d rule(s)"), Objectives->GetRuleCount()));
	Check(TEXT("an objective is active at story start"),
		!Objectives->GetCurrentObjective(false).IsEmpty(),
		Objectives->GetCurrentObjective(false));

	// Setting a flag must move the story on. If it does not, the rule chain is
	// broken and the player would be stuck on one objective forever.
	if (UTBWWorldStateSubsystem* State = World->GetSubsystem<UTBWWorldStateSubsystem>())
	{
		const FString Before = Objectives->GetCurrentObjective(false);
		const bool bHad = State->HasFlag(TEXT("OrinLastWords"));
		State->SetFlag(TEXT("OrinLastWords"), 1);
		const FString After = Objectives->GetCurrentObjective(false);
		Check(TEXT("objective advances when a story flag is set"), Before != After,
			FString::Printf(TEXT("'%s' -> '%s'"), *Before, *After));
		if (!bHad)
		{
			State->ClearFlag(TEXT("OrinLastWords"));
		}
	}
}

void UTBWSelfTest::TestSaveRoundTrip()
{
	UWorld* World = GetWorld();
	UGameInstance* GI = World ? World->GetGameInstance() : nullptr;
	UTBWSaveSubsystem* Saves = GI ? GI->GetSubsystem<UTBWSaveSubsystem>() : nullptr;
	Check(TEXT("save subsystem exists"), Saves != nullptr);

	UTBWWorldStateSubsystem* State = World ? World->GetSubsystem<UTBWWorldStateSubsystem>() : nullptr;
	if (!Saves || !State)
	{
		return;
	}

	const FName Marker = TEXT("ScarOil");
	const bool bHad = State->HasFlag(Marker);

	State->SetFlag(Marker, 1);
	Check(TEXT("save writes a slot"), Saves->SaveToSlot(TestSlot));

	State->ClearFlag(Marker);
	Check(TEXT("flag really cleared before load"), !State->HasFlag(Marker));

	Check(TEXT("load reads the slot back"), Saves->LoadFromSlot(TestSlot));
	Check(TEXT("flag survived the save/load round trip"), State->HasFlag(Marker));

	FString Summary;
	Check(TEXT("slot exposes a readable summary"), Saves->PeekSlot(TestSlot, Summary),
		Summary.Left(60));

	Saves->DeleteSlot(TestSlot);
	Check(TEXT("test slot cleaned up"), !Saves->DoesSlotExist(TestSlot));

	if (!bHad)
	{
		State->ClearFlag(Marker);
	}
}

void UTBWSelfTest::TestLevelContent()
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	int32 Interactables = 0;
	for (TActorIterator<ATBWInteractableActor> It(World); It; ++It)
	{
		++Interactables;
	}

	int32 Cast = 0;
	int32 Speaking = 0;
	for (TActorIterator<ATBWStoryCharacter> It(World); It; ++It)
	{
		++Cast;
		if (!It->PlaysDialogueScene.IsNone())
		{
			++Speaking;
		}
	}

	// A greybox sandbox legitimately has neither, so these are reported rather
	// than failed when the level is clearly not the authored wing.
	const FString MapName = World->GetMapName();
	const bool bAuthored = MapName.Contains(TEXT("Palace"));

	Report.Add(FString::Printf(TEXT("  INFO  map '%s', %d interactable(s), %d cast member(s)"),
		*MapName, Interactables, Cast));

	if (bAuthored)
	{
		Check(TEXT("authored level has its story beats"), Interactables >= 8,
			FString::Printf(TEXT("%d found"), Interactables));
		Check(TEXT("authored level has its cast"), Cast >= 10,
			FString::Printf(TEXT("%d found"), Cast));
		Check(TEXT("at least one character can speak"), Speaking >= 1);
	}
}

void UTBWSelfTest::TestPlayer()
{
	UWorld* World = GetWorld();
	APlayerController* PC = World ? World->GetFirstPlayerController() : nullptr;
	if (!PC)
	{
		Report.Add(TEXT("  INFO  no player controller (headless run) - player checks skipped"));
		return;
	}

	ATBWPlayerCharacter* Pawn = Cast<ATBWPlayerCharacter>(PC->GetPawn());
	Check(TEXT("player pawn is a TBW character"), Pawn != nullptr);
	if (Pawn)
	{
		Check(TEXT("pawn has an identity component"), Pawn->GetIdentityComponent() != nullptr);
		Check(TEXT("pawn has an interactor"), Pawn->GetInteractor() != nullptr);
		Report.Add(FString::Printf(TEXT("  INFO  body: %s"),
			Pawn->IsUsingRealCharacterMesh() ? TEXT("skeletal mesh") : TEXT("placeholder cube")));
	}

	Check(TEXT("HUD is the TBW HUD"), Cast<ATBWHUD>(PC->GetHUD()) != nullptr);

	if (PC->PlayerCameraManager)
	{
		Check(TEXT("camera pitch is clamped"),
			PC->PlayerCameraManager->ViewPitchMin <= -50.f && PC->PlayerCameraManager->ViewPitchMax >= 60.f,
			FString::Printf(TEXT("%.0f to %.0f"),
				PC->PlayerCameraManager->ViewPitchMin, PC->PlayerCameraManager->ViewPitchMax));
	}
}

int32 UTBWSelfTest::RunAll()
{
	Passed = 0;
	Failed = 0;
	Report.Reset();

	TestWorldState();
	TestDialogue();
	TestObjectives();
	TestSaveRoundTrip();
	TestLevelContent();
	TestPlayer();

	UE_LOG(LogTBW, Display, TEXT("================ TBW SELFTEST ================"));
	for (const FString& Line : Report)
	{
		UE_LOG(LogTBW, Display, TEXT("%s"), *Line);
	}
	UE_LOG(LogTBW, Display, TEXT("----------------------------------------------"));
	UE_LOG(LogTBW, Display, TEXT("TBW SELFTEST RESULT: %d passed, %d failed"), Passed, Failed);
	UE_LOG(LogTBW, Display, TEXT("=============================================="));

	LastFailures = Failed;
	return Failed;
}
