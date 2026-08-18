// The Betrayed Will / وصية الغدر
// Copyright (c) 2026. All rights reserved.

#include "Narrative/TBWDialogueSubsystem.h"
#include "Core/TBWWorldStateSubsystem.h"
#include "Core/TBWWorldFlags.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Misc/Paths.h"
#include "Misc/FileHelper.h"
#include "HAL/FileManager.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"
#include "Dom/JsonObject.h"
#include "TBW.h"

namespace
{
	FString DialogueDir()
	{
		return FPaths::ProjectContentDir() / TEXT("TBW/Data/Dialogue");
	}
}

void UTBWDialogueSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	const int32 Count = ReloadScenes();
	UE_LOG(LogTBW, Log, TEXT("Dialogue subsystem ready. %d scene(s) loaded."), Count);
}

void UTBWDialogueSubsystem::Deinitialize()
{
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(LineTimer);
	}
	Super::Deinitialize();
}

int32 UTBWDialogueSubsystem::ReloadScenes()
{
	Scenes.Reset();

	const FString Dir = DialogueDir();
	TArray<FString> Files;
	IFileManager::Get().FindFilesRecursive(Files, *Dir, TEXT("*.json"), true, false);

	for (const FString& File : Files)
	{
		LoadSceneFile(File);
	}

	if (Scenes.Num() == 0)
	{
		UE_LOG(LogTBW, Warning, TEXT("No dialogue scenes found under %s"), *Dir);
	}
	return Scenes.Num();
}

bool UTBWDialogueSubsystem::LoadSceneFile(const FString& FilePath)
{
	FString Raw;
	// UTF-8 is decoded here, not by the compiler. Arabic survives intact.
	if (!FFileHelper::LoadFileToString(Raw, *FilePath))
	{
		UE_LOG(LogTBW, Warning, TEXT("Dialogue: cannot read %s"), *FilePath);
		return false;
	}

	TSharedPtr<FJsonObject> Root;
	const TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Raw);
	if (!FJsonSerializer::Deserialize(Reader, Root) || !Root.IsValid())
	{
		UE_LOG(LogTBW, Error, TEXT("Dialogue: malformed JSON in %s"), *FilePath);
		return false;
	}

	FTBWDialogueScene Scene;
	Scene.SceneId = FName(*Root->GetStringField(TEXT("scene_id")));
	Root->TryGetStringField(TEXT("title"), Scene.Title);

	FString SegmentText;
	if (Root->TryGetStringField(TEXT("segment"), SegmentText))
	{
		Scene.Segment = FName(*SegmentText);
	}

	FString FlagText;
	if (Root->TryGetStringField(TEXT("completion_flag"), FlagText) && !FlagText.IsEmpty())
	{
		Scene.CompletionFlag = FName(*FlagText);
		ETBWWorldFlag Typed = ETBWWorldFlag::None;
		if (!FTBWWorldFlags::TryParse(FlagText, Typed))
		{
			UE_LOG(LogTBW, Warning,
				TEXT("Dialogue %s: completion flag '%s' is not in ETBWWorldFlag."),
				*Scene.SceneId.ToString(), *FlagText);
		}
	}

	Root->TryGetBoolField(TEXT("skippable_first_time"), Scene.bSkippableFirstTime);

	const TArray<TSharedPtr<FJsonValue>>* Lines = nullptr;
	if (!Root->TryGetArrayField(TEXT("lines"), Lines) || !Lines)
	{
		UE_LOG(LogTBW, Error, TEXT("Dialogue %s: no lines array."), *Scene.SceneId.ToString());
		return false;
	}

	for (const TSharedPtr<FJsonValue>& Value : *Lines)
	{
		const TSharedPtr<FJsonObject>* Obj = nullptr;
		if (!Value->TryGetObject(Obj) || !Obj)
		{
			continue;
		}

		FTBWDialogueLine Line;
		FString Speaker;
		if ((*Obj)->TryGetStringField(TEXT("speaker"), Speaker))
		{
			Line.Speaker = FName(*Speaker);
		}
		(*Obj)->TryGetStringField(TEXT("en"), Line.TextEN);
		(*Obj)->TryGetStringField(TEXT("ar"), Line.TextAR);
		(*Obj)->TryGetBoolField(TEXT("action"), Line.bAction);

		double Seconds = 0.0;
		if ((*Obj)->TryGetNumberField(TEXT("seconds"), Seconds) && Seconds > 0.0)
		{
			Line.Duration = static_cast<float>(Seconds);
		}
		else
		{
			// Reading pace, floored so a two-word line still registers.
			const int32 Chars = FMath::Max(Line.TextEN.Len(), Line.TextAR.Len());
			Line.Duration = FMath::Clamp(1.4f + Chars * 0.045f, 1.6f, 9.0f);
		}

		Scene.Lines.Add(Line);
	}

	if (!Scene.IsValid())
	{
		UE_LOG(LogTBW, Error, TEXT("Dialogue: %s produced no usable scene."), *FilePath);
		return false;
	}

	Scenes.Add(Scene.SceneId, Scene);
	return true;
}

void UTBWDialogueSubsystem::GetSceneIds(TArray<FName>& OutIds) const
{
	OutIds.Reset();
	Scenes.GetKeys(OutIds);
	OutIds.Sort(FNameLexicalLess());
}

bool UTBWDialogueSubsystem::PlayScene(FName SceneId)
{
	const FTBWDialogueScene* Found = Scenes.Find(SceneId);
	if (!Found)
	{
		UE_LOG(LogTBW, Warning, TEXT("Dialogue: no scene '%s'."), *SceneId.ToString());
		return false;
	}

	if (bPlaying)
	{
		Stop(false);
	}

	ActiveScene = *Found;
	bPlaying = true;
	OnSceneStarted.Broadcast(ActiveScene.SceneId);
	UE_LOG(LogTBW, Display, TEXT("Dialogue: playing %s (%d lines)"),
		*ActiveScene.SceneId.ToString(), ActiveScene.Lines.Num());

	StartLine(0);
	return true;
}

void UTBWDialogueSubsystem::StartLine(int32 Index)
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	if (!ActiveScene.Lines.IsValidIndex(Index))
	{
		Stop(true);
		return;
	}

	LineIndex = Index;
	const FTBWDialogueLine& Line = ActiveScene.Lines[LineIndex];
	LineEndTime = World->GetTimeSeconds() + Line.Duration;

	World->GetTimerManager().ClearTimer(LineTimer);
	World->GetTimerManager().SetTimer(
		LineTimer, this, &UTBWDialogueSubsystem::OnLineTimer, Line.Duration, false);

	OnLineChanged.Broadcast(Line);
}

void UTBWDialogueSubsystem::OnLineTimer()
{
	StartLine(LineIndex + 1);
}

void UTBWDialogueSubsystem::Advance()
{
	if (!bPlaying)
	{
		return;
	}
	StartLine(LineIndex + 1);
}

bool UTBWDialogueSubsystem::CanSkip() const
{
	if (!bPlaying)
	{
		return false;
	}
	// VS-01 must be watched once. After that the player owns their time.
	return ActiveScene.bSkippableFirstTime || ScenesSeen.Contains(ActiveScene.SceneId);
}

void UTBWDialogueSubsystem::Stop(bool bSetCompletionFlag)
{
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(LineTimer);

		if (bSetCompletionFlag && !ActiveScene.CompletionFlag.IsNone())
		{
			if (UTBWWorldStateSubsystem* State = World->GetSubsystem<UTBWWorldStateSubsystem>())
			{
				State->SetFlag(ActiveScene.CompletionFlag, 1);
			}
		}
	}

	if (bPlaying)
	{
		ScenesSeen.Add(ActiveScene.SceneId);
		OnSceneFinished.Broadcast(ActiveScene.SceneId);
		UE_LOG(LogTBW, Display, TEXT("Dialogue: finished %s"), *ActiveScene.SceneId.ToString());
	}

	bPlaying = false;
	LineIndex = INDEX_NONE;
	ActiveScene = FTBWDialogueScene();
}

FTBWDialogueLine UTBWDialogueSubsystem::GetCurrentLine() const
{
	if (bPlaying && ActiveScene.Lines.IsValidIndex(LineIndex))
	{
		return ActiveScene.Lines[LineIndex];
	}
	return FTBWDialogueLine();
}

FString UTBWDialogueSubsystem::GetSubtitle() const
{
	if (!bPlaying || !ActiveScene.Lines.IsValidIndex(LineIndex))
	{
		return FString();
	}

	const FTBWDialogueLine& Line = ActiveScene.Lines[LineIndex];
	const FString Body = (bArabic && !Line.TextAR.IsEmpty()) ? Line.TextAR : Line.TextEN;

	if (Line.bAction || Line.Speaker.IsNone())
	{
		return Body;
	}
	return FString::Printf(TEXT("%s: %s"), *Line.Speaker.ToString(), *Body);
}

float UTBWDialogueSubsystem::GetLineRemaining() const
{
	const UWorld* World = GetWorld();
	if (!bPlaying || !World)
	{
		return 0.f;
	}
	return FMath::Max(0.f, LineEndTime - World->GetTimeSeconds());
}
