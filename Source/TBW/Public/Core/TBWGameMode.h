// The Betrayed Will / وصية الغدر
// Copyright (c) 2026. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TBWGameMode.generated.h"

class ATBWEastWingSandbox;

UCLASS()
class TBW_API ATBWGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ATBWGameMode();

	virtual void StartPlay() override;
	virtual void RestartPlayer(AController* NewPlayer) override;

	UFUNCTION(BlueprintPure, Category = "TBW|Sandbox")
	ATBWEastWingSandbox* GetSandbox() const { return Sandbox; }

private:
	void EnsureSandbox();

	UPROPERTY()
	TObjectPtr<ATBWEastWingSandbox> Sandbox;
};
