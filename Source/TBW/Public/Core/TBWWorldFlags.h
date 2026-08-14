// The Betrayed Will / وصية الغدر
// Copyright (c) 2026. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "TBWWorldFlags.generated.h"

/**
 * Typed foundation flags. The subsystem still accepts arbitrary FName
 * so later quests do not require a C++ rebuild.
 * This is NOT a quest system.
 */
UENUM(BlueprintType)
enum class ETBWWorldFlag : uint8
{
	None = 0,
	WillWasRead,
	RaynorDisappeared,
	EvanInvestigating,
	ClueFound_01,
	GuardAlerted,
	EmptyChairExamined,
	UnusedBed,
	ScarOil,
	ClaspFound,
	RosterAltered,
	LetterIsFalse,
	WestArchTouched,
	TestDoorOpen,
	COUNT UMETA(Hidden)
};

struct TBW_API FTBWWorldFlags
{
	static FName ToName(ETBWWorldFlag Flag);
	static bool TryParse(const FString& Text, ETBWWorldFlag& OutFlag);
	static void GetKnownNames(TArray<FName>& OutNames);
};
