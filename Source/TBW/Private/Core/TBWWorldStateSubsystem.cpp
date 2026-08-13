// The Betrayed Will / وصية الغدر
// Copyright (c) 2026. All rights reserved.

#include "Core/TBWWorldStateSubsystem.h"
#include "TBW.h"

void UTBWWorldStateSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	UE_LOG(LogTBW, Log, TEXT("WorldStateSubsystem ready."));
}

void UTBWWorldStateSubsystem::SetFlag(FName Flag, int32 Value)
{
	if (Flag.IsNone())
	{
		return;
	}

	const int32* Existing = Flags.Find(Flag);
	if (Existing && *Existing == Value)
	{
		return;
	}

	Flags.Add(Flag, Value);
	UE_LOG(LogTBW, Display, TEXT("Flag %s = %d"), *Flag.ToString(), Value);
	OnFlagChanged.Broadcast(Flag, Value);
}

int32 UTBWWorldStateSubsystem::GetFlag(FName Flag) const
{
	if (const int32* Value = Flags.Find(Flag))
	{
		return *Value;
	}
	return 0;
}

bool UTBWWorldStateSubsystem::HasFlag(FName Flag) const
{
	return Flags.Contains(Flag);
}

void UTBWWorldStateSubsystem::ClearFlag(FName Flag)
{
	if (Flags.Remove(Flag) > 0)
	{
		OnFlagChanged.Broadcast(Flag, 0);
	}
}

void UTBWWorldStateSubsystem::ClearAllFlags()
{
	Flags.Reset();
}

void UTBWWorldStateSubsystem::GetDebugLines(TArray<FString>& OutLines) const
{
	OutLines.Reset();
	for (const TPair<FName, int32>& Pair : Flags)
	{
		OutLines.Add(FString::Printf(TEXT("%s = %d"), *Pair.Key.ToString(), Pair.Value));
	}
	OutLines.Sort();
}

void UTBWWorldStateSubsystem::LogAllFlags() const
{
	UE_LOG(LogTBW, Display, TEXT("WorldState (%d flags):"), Flags.Num());
	TArray<FString> Lines;
	GetDebugLines(Lines);
	for (const FString& Line : Lines)
	{
		UE_LOG(LogTBW, Display, TEXT("  %s"), *Line);
	}
}
