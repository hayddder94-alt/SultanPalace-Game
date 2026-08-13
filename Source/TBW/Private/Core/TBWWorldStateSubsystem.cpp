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
	UE_LOG(LogTBW, Verbose, TEXT("Flag %s = %d"), *Flag.ToString(), Value);
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
