// The Betrayed Will / وصية الغدر
// Copyright (c) 2026. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TBWGameMode.generated.h"

class ATBWDevSandbox;

UCLASS()
class TBW_API ATBWGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ATBWGameMode();

	virtual void StartPlay() override;
	virtual void RestartPlayer(AController* NewPlayer) override;

	UFUNCTION(BlueprintPure, Category = "TBW|Sandbox")
	ATBWDevSandbox* GetDevSandbox() const { return DevSandbox; }

	/** Autosaves when a story flag turns on. Bound to the world state delegate. */
	UFUNCTION()
	void HandleStoryFlagChanged(FName Flag, int32 NewValue);

	/** True when the current map ships its own geometry and PlayerStart. */
	UFUNCTION(BlueprintPure, Category = "TBW|Sandbox")
	bool IsAuthoredLevel() const;

private:
	void EnsureSandbox();

	UPROPERTY()
	TObjectPtr<ATBWDevSandbox> DevSandbox;

	float LastAutosaveTime = -1000.f;
};
