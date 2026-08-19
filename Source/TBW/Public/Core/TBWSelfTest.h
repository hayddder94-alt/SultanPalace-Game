// The Betrayed Will / وصية الغدر
// Copyright (c) 2026. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "TBWSelfTest.generated.h"

/**
 * In-engine self test.
 *
 * Ten commits were written without a compile because verifying them by hand meant
 * a thirty-step checklist someone had to sit through. This turns that into one
 * command that runs inside a real Unreal process and prints a verdict.
 *
 * It does NOT replace human judgement - nothing here can tell you whether the
 * hall feels imposing. It replaces the mechanical half: did the data load, do the
 * flags work, does a save survive a load, is the cast actually in the level.
 *
 * Headless friendly: everything it touches works under -nullrhi, so it can be run
 * from a script with no window and no GPU.
 */
UCLASS()
class TBW_API UTBWSelfTest : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	/** Runs every check and logs a report. Returns the number of failures. */
	UFUNCTION(BlueprintCallable, Category = "TBW|Test")
	int32 RunAll();

	UFUNCTION(BlueprintPure, Category = "TBW|Test")
	int32 GetLastFailureCount() const { return LastFailures; }

private:
	void Check(const FString& Name, bool bCondition, const FString& Detail = FString());

	void TestWorldState();
	void TestDialogue();
	void TestObjectives();
	void TestSaveRoundTrip();
	void TestLevelContent();
	void TestPlayer();

	int32 Passed = 0;
	int32 Failed = 0;
	int32 LastFailures = 0;
	TArray<FString> Report;
};
