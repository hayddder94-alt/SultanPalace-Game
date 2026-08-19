// The Betrayed Will / وصية الغدر
// Copyright (c) 2026. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "TBWSaveGame.generated.h"

/**
 * One save slot.
 *
 * Everything the story needs to resume lives in world flags, so the save is
 * mostly a snapshot of those plus where the player was standing. No actor
 * serialisation, no object graph: an authored linear game does not need it, and
 * a save format that serialises live actors breaks every time a class changes.
 */
UCLASS()
class TBW_API UTBWSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	/** Bumped whenever the layout of this class changes. Old saves are refused. */
	static constexpr int32 CurrentVersion = 1;

	UPROPERTY(BlueprintReadOnly, Category = "TBW|Save")
	int32 SaveVersion = CurrentVersion;

	UPROPERTY(BlueprintReadOnly, Category = "TBW|Save")
	FString GameVersion;

	UPROPERTY(BlueprintReadOnly, Category = "TBW|Save")
	FDateTime SavedAtUtc;

	UPROPERTY(BlueprintReadOnly, Category = "TBW|Save")
	FString LevelName;

	UPROPERTY(BlueprintReadOnly, Category = "TBW|Save")
	FName IdentityId;

	UPROPERTY(BlueprintReadOnly, Category = "TBW|Save")
	FVector PlayerLocation = FVector::ZeroVector;

	UPROPERTY(BlueprintReadOnly, Category = "TBW|Save")
	FRotator PlayerRotation = FRotator::ZeroRotator;

	UPROPERTY(BlueprintReadOnly, Category = "TBW|Save")
	TMap<FName, int32> Flags;

	/** Scenes already watched - drives 'skippable after the first time'. */
	UPROPERTY(BlueprintReadOnly, Category = "TBW|Save")
	TArray<FName> ScenesSeen;

	UPROPERTY(BlueprintReadOnly, Category = "TBW|Save")
	float PlaytimeSeconds = 0.f;

	/** Human-readable line for a load menu: segment, objective, timestamp. */
	UPROPERTY(BlueprintReadOnly, Category = "TBW|Save")
	FString Summary;
};
