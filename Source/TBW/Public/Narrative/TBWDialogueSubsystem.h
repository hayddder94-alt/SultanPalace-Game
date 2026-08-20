// The Betrayed Will / وصية الغدر
// Copyright (c) 2026. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Narrative/TBWDialogueTypes.h"
#include "TBWDialogueSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTBWDialogueLineChanged, const FTBWDialogueLine&, Line);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTBWDialogueSceneEvent, FName, SceneId);

/**
 * Plays authored dialogue scenes loaded from Content/TBW/Data/Dialogue/*.json.
 *
 * Deliberately NOT a conversation system: there are no choices, no branching and
 * no NPC state here. The Betrayed Will is a linear told story, and a branching
 * engine we do not need is a branching engine we would have to debug.
 */
UCLASS()
class TBW_API UTBWDialogueSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	/** Re-reads every scene file. Safe to call at runtime while authoring. */
	UFUNCTION(BlueprintCallable, Category = "TBW|Dialogue")
	int32 ReloadScenes();

	UFUNCTION(BlueprintCallable, Category = "TBW|Dialogue")
	bool PlayScene(FName SceneId);

	/**
	 * Play a scene the first time only.
	 *
	 * "First time" means two things and both matter: not already played in this
	 * session, and its completion flag not already set - otherwise loading a
	 * save replays the father's death the moment you move.
	 */
	UFUNCTION(BlueprintCallable, Category = "TBW|Dialogue")
	bool PlaySceneOnce(FName SceneId);

	UFUNCTION(BlueprintCallable, Category = "TBW|Dialogue")
	void Advance();

	UFUNCTION(BlueprintCallable, Category = "TBW|Dialogue")
	void Stop(bool bSetCompletionFlag);

	UFUNCTION(BlueprintPure, Category = "TBW|Dialogue")
	bool IsPlaying() const { return bPlaying; }

	UFUNCTION(BlueprintPure, Category = "TBW|Dialogue")
	FName GetActiveSceneId() const { return ActiveScene.SceneId; }

	UFUNCTION(BlueprintPure, Category = "TBW|Dialogue")
	FTBWDialogueLine GetCurrentLine() const;

	/** Speaker plus text, already language-resolved. Empty when idle. */
	UFUNCTION(BlueprintPure, Category = "TBW|Dialogue")
	FString GetSubtitle() const;

	UFUNCTION(BlueprintPure, Category = "TBW|Dialogue")
	float GetLineRemaining() const;

	UFUNCTION(BlueprintPure, Category = "TBW|Dialogue")
	bool CanSkip() const;

	UFUNCTION(BlueprintCallable, Category = "TBW|Dialogue")
	void SetArabic(bool bInArabic) { bArabic = bInArabic; }

	UFUNCTION(BlueprintPure, Category = "TBW|Dialogue")
	bool IsArabic() const { return bArabic; }

	UFUNCTION(BlueprintPure, Category = "TBW|Dialogue")
	void GetSceneIds(TArray<FName>& OutIds) const;

	UPROPERTY(BlueprintAssignable, Category = "TBW|Dialogue")
	FTBWDialogueLineChanged OnLineChanged;

	UPROPERTY(BlueprintAssignable, Category = "TBW|Dialogue")
	FTBWDialogueSceneEvent OnSceneStarted;

	UPROPERTY(BlueprintAssignable, Category = "TBW|Dialogue")
	FTBWDialogueSceneEvent OnSceneFinished;

private:
	/** Scenes already played this session, for PlaySceneOnce. */
	TSet<FName> PlayedScenes;
	bool LoadSceneFile(const FString& FilePath);
	void StartLine(int32 Index);
	void OnLineTimer();

	TMap<FName, FTBWDialogueScene> Scenes;
	TSet<FName> ScenesSeen;

	FTBWDialogueScene ActiveScene;
	int32 LineIndex = INDEX_NONE;
	bool bPlaying = false;
	bool bArabic = true;
	float LineEndTime = 0.f;
	FTimerHandle LineTimer;
};
