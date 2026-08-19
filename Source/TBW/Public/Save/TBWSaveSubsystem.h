// The Betrayed Will / وصية الغدر
// Copyright (c) 2026. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "TBWSaveSubsystem.generated.h"

class UTBWSaveGame;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTBWSaveEvent, const FString&, SlotName);

/**
 * Save and load. Lives on the GameInstance so it survives level travel.
 *
 * Rules that keep saves from rotting:
 *   - a save records its own version; a mismatch is refused, never guessed at
 *   - autosave is a named slot, so a bad autosave can never overwrite a manual one
 *   - saving never blocks on the game thread longer than gathering flags
 */
UCLASS()
class TBW_API UTBWSaveSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	static const FString AutoSlot;
	static const FString QuickSlot;

	UFUNCTION(BlueprintCallable, Category = "TBW|Save")
	bool SaveToSlot(const FString& SlotName);

	UFUNCTION(BlueprintCallable, Category = "TBW|Save")
	bool LoadFromSlot(const FString& SlotName);

	UFUNCTION(BlueprintCallable, Category = "TBW|Save")
	bool DeleteSlot(const FString& SlotName);

	UFUNCTION(BlueprintPure, Category = "TBW|Save")
	bool DoesSlotExist(const FString& SlotName) const;

	/** Reads a slot's header without applying it - for a load menu. */
	UFUNCTION(BlueprintCallable, Category = "TBW|Save")
	bool PeekSlot(const FString& SlotName, FString& OutSummary) const;

	UFUNCTION(BlueprintCallable, Category = "TBW|Save")
	bool Autosave(const FString& Reason);

	UFUNCTION(BlueprintPure, Category = "TBW|Save")
	float GetSessionSeconds() const;

	/** True while a load is writing flags back. Autosave must stand down. */
	UFUNCTION(BlueprintPure, Category = "TBW|Save")
	bool IsApplying() const { return bApplying; }

	UPROPERTY(BlueprintAssignable, Category = "TBW|Save")
	FTBWSaveEvent OnSaved;

	UPROPERTY(BlueprintAssignable, Category = "TBW|Save")
	FTBWSaveEvent OnLoaded;

private:
	UTBWSaveGame* Capture() const;
	bool Apply(UTBWSaveGame* Save);

	float AccumulatedSeconds = 0.f;
	bool bApplying = false;
};
