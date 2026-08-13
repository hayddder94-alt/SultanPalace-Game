// The Betrayed Will / وصية الغدر
// Copyright (c) 2026. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Core/TBWWorldFlags.h"
#include "TBWWorldStateSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FTBWFlagChanged, FName, Flag, int32, NewValue);

/**
 * Persistent world-flag foundation. Not a quest system.
 * Supports typed ETBWWorldFlag and arbitrary FName for later data-driven content.
 */
UCLASS()
class TBW_API UTBWWorldStateSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	UFUNCTION(BlueprintCallable, Category = "TBW|WorldState")
	void SetFlag(FName Flag, int32 Value = 1);

	UFUNCTION(BlueprintPure, Category = "TBW|WorldState")
	int32 GetFlag(FName Flag) const;

	UFUNCTION(BlueprintPure, Category = "TBW|WorldState")
	bool HasFlag(FName Flag) const;

	UFUNCTION(BlueprintCallable, Category = "TBW|WorldState")
	void ClearFlag(FName Flag);

	UFUNCTION(BlueprintCallable, Category = "TBW|WorldState")
	void ClearAllFlags();

	UFUNCTION(BlueprintCallable, Category = "TBW|WorldState")
	void SetWorldFlag(ETBWWorldFlag Flag, int32 Value = 1);

	UFUNCTION(BlueprintPure, Category = "TBW|WorldState")
	int32 GetWorldFlag(ETBWWorldFlag Flag) const;

	UFUNCTION(BlueprintPure, Category = "TBW|WorldState")
	bool CheckWorldFlag(ETBWWorldFlag Flag) const;

	UFUNCTION(BlueprintCallable, Category = "TBW|WorldState")
	void ClearWorldFlag(ETBWWorldFlag Flag);

	void GetDebugLines(TArray<FString>& OutLines) const;
	void LogAllFlags() const;
	void ResetTestState();

	const TMap<FName, int32>& GetAllFlags() const { return Flags; }

	UPROPERTY(BlueprintAssignable, Category = "TBW|WorldState")
	FTBWFlagChanged OnFlagChanged;

private:
	UPROPERTY()
	TMap<FName, int32> Flags;
};
