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

private:
	void EnsureSandbox();

	UPROPERTY()
	TObjectPtr<ATBWDevSandbox> DevSandbox;
};
