// The Betrayed Will / وصية الغدر
// Copyright (c) 2026. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "TBWHUD.generated.h"

/**
 * Phase 1 debug / foundation HUD. No art. Replace with UMG in later phases.
 */
UCLASS()
class TBW_API ATBWHUD : public AHUD
{
	GENERATED_BODY()

public:
	virtual void DrawHUD() override;

	void SetPausedBanner(bool bPaused);

private:
	bool bShowPaused = false;
};
