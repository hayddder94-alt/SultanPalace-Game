// The Betrayed Will / وصية الغدر
// Copyright (c) 2026. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "TBWGameInstance.generated.h"

/**
 * Owns session-level concerns: last-used save slot, language, settings handle.
 * Full save orchestration is Phase 8. This class exists so the project has a
 * real GameInstance from day one.
 */
UCLASS()
class TBW_API UTBWGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	virtual void Init() override;

	/** Drops cached animation lookups so a re-import inside the editor is seen
	 *  on the next Play instead of on the next restart. */
	virtual void Shutdown() override;

	UPROPERTY(BlueprintReadOnly, Category = "TBW|Session")
	int32 LastUsedSaveSlot = INDEX_NONE;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TBW|Session")
	FString UILanguage = TEXT("en");
};
