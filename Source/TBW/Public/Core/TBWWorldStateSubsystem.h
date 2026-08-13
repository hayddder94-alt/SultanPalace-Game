// The Betrayed Will / وصية الغدر
// Copyright (c) 2026. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "TBWWorldStateSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FTBWFlagChanged, FName, Flag, int32, NewValue);

/**
 * The flag bus. Quest, Dialogue, and World listen here.
 * Compact name→int map. Do not serialize the entire actor graph.
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

	UFUNCTION(BlueprintCallable, Category = "TBW|WorldState")
	void ClearFlag(FName Flag);

	UFUNCTION(BlueprintCallable, Category = "TBW|WorldState")
	void ClearAllFlags();

	void GetDebugLines(TArray<FString>& OutLines) const;
	void LogAllFlags() const;

	const TMap<FName, int32>& GetAllFlags() const { return Flags; }

	UPROPERTY(BlueprintAssignable, Category = "TBW|WorldState")
	FTBWFlagChanged OnFlagChanged;

private:
	UPROPERTY()
	TMap<FName, int32> Flags;
};
