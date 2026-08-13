// The Betrayed Will / وصية الغدر
// Copyright (c) 2026. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "TBWWorldStateSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FTBWFlagChanged, FName, Flag, int32, NewValue);

/**
 * The flag bus. Quest, Dialogue, and World listen here.
 * Phase 0 ships a name→int map only. Phase 1 adds console cheats and
 * GameplayTag aliases. Do not serialize the entire actor graph.
 */
UCLASS()
class TBW_API UTBWWorldStateSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	UFUNCTION(BlueprintCallable, Category = "TBW|WorldState")
	void SetFlag(FName Flag, int32 Value);

	UFUNCTION(BlueprintPure, Category = "TBW|WorldState")
	int32 GetFlag(FName Flag) const;

	UFUNCTION(BlueprintPure, Category = "TBW|WorldState")
	bool HasFlag(FName Flag) const;

	UPROPERTY(BlueprintAssignable, Category = "TBW|WorldState")
	FTBWFlagChanged OnFlagChanged;

private:
	UPROPERTY()
	TMap<FName, int32> Flags;
};
