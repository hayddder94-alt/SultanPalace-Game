// The Betrayed Will / وصية الغدر
// Copyright (c) 2026. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "TBWObjectiveSubsystem.generated.h"

/**
 * Tells the player what to do next, derived from world flags.
 *
 * There is no quest graph, no objective assets and no designer-facing editor.
 * A rule is: "if these flags are set and these are not, the objective is X."
 * The Betrayed Will is linear; a quest system would be scaffolding around a
 * straight line. Load-bearing simplicity, kept until the story disproves it.
 */
USTRUCT(BlueprintType)
struct TBW_API FTBWObjectiveRule
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "TBW|Objective")
	FName Segment;

	UPROPERTY(BlueprintReadOnly, Category = "TBW|Objective")
	FString TextEN;

	UPROPERTY(BlueprintReadOnly, Category = "TBW|Objective")
	FString TextAR;

	/** All of these must be set. */
	UPROPERTY(BlueprintReadOnly, Category = "TBW|Objective")
	TArray<FName> RequiresFlags;

	/** None of these may be set - this is what retires an objective. */
	UPROPERTY(BlueprintReadOnly, Category = "TBW|Objective")
	TArray<FName> BlockedByFlags;
};

UCLASS()
class TBW_API UTBWObjectiveSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	/** Reads Content/TBW/Data/Objectives.json. Returns the rule count. */
	UFUNCTION(BlueprintCallable, Category = "TBW|Objective")
	int32 ReloadRules();

	/** First rule whose conditions hold. Empty when the story has run out. */
	UFUNCTION(BlueprintPure, Category = "TBW|Objective")
	FString GetCurrentObjective(bool bArabic) const;

	UFUNCTION(BlueprintPure, Category = "TBW|Objective")
	FName GetCurrentSegment() const;

	UFUNCTION(BlueprintPure, Category = "TBW|Objective")
	int32 GetRuleCount() const { return Rules.Num(); }

	/** How far through the authored rules the player is, 0..1. */
	UFUNCTION(BlueprintPure, Category = "TBW|Objective")
	float GetProgress() const;

private:
	const FTBWObjectiveRule* FindActiveRule() const;

	TArray<FTBWObjectiveRule> Rules;
};
