// The Betrayed Will / وصية الغدر
// Copyright (c) 2026. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "TBWDialogueTypes.generated.h"

/**
 * One spoken line.
 *
 * Text lives in JSON under Content/TBW/Data/Dialogue, never in a C++ literal.
 * That is deliberate: MSVC reads source files as CP1252 unless forced otherwise,
 * which is exactly what mojibaked the Arabic HUD title in Phase 1. UTF-8 JSON
 * read through FFileHelper has no such ambiguity, so Arabic is safe by design.
 */
USTRUCT(BlueprintType)
struct TBW_API FTBWDialogueLine
{
	GENERATED_BODY()

	/** Speaker id, matched against docs/CHARACTER_BIBLE.md: Orin, Raynor, Evan... */
	UPROPERTY(BlueprintReadOnly, Category = "TBW|Dialogue")
	FName Speaker;

	/** English line. Always present - it is the production language. */
	UPROPERTY(BlueprintReadOnly, Category = "TBW|Dialogue")
	FString TextEN;

	/** Arabic line. Empty until translated; the runtime falls back to English. */
	UPROPERTY(BlueprintReadOnly, Category = "TBW|Dialogue")
	FString TextAR;

	/** Seconds this line holds before auto-advancing. */
	UPROPERTY(BlueprintReadOnly, Category = "TBW|Dialogue")
	float Duration = 3.0f;

	/** Stage direction rather than speech - rendered in italics, no speaker. */
	UPROPERTY(BlueprintReadOnly, Category = "TBW|Dialogue")
	bool bAction = false;
};

/** A whole scene: VS-01, VS-02, or a two-line exchange in a corridor. */
USTRUCT(BlueprintType)
struct TBW_API FTBWDialogueScene
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "TBW|Dialogue")
	FName SceneId;

	UPROPERTY(BlueprintReadOnly, Category = "TBW|Dialogue")
	FString Title;

	/** Segment this belongs to, e.g. VS-01. Used by the objective layer. */
	UPROPERTY(BlueprintReadOnly, Category = "TBW|Dialogue")
	FName Segment;

	/** World flag set when the scene finishes. Must exist in ETBWWorldFlag. */
	UPROPERTY(BlueprintReadOnly, Category = "TBW|Dialogue")
	FName CompletionFlag;

	/** Can the player skip before having seen it once? VS-01 says no. */
	UPROPERTY(BlueprintReadOnly, Category = "TBW|Dialogue")
	bool bSkippableFirstTime = true;

	UPROPERTY(BlueprintReadOnly, Category = "TBW|Dialogue")
	TArray<FTBWDialogueLine> Lines;

	bool IsValid() const { return !SceneId.IsNone() && Lines.Num() > 0; }
};
