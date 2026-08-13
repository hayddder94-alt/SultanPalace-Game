// The Betrayed Will / وصية الغدر
// Copyright (c) 2026. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "TBWHUD.generated.h"

/**
 * Phase 1 HUD. Shipping strips the debug overlay.
 */
UCLASS()
class TBW_API ATBWHUD : public AHUD
{
	GENERATED_BODY()

public:
	virtual void DrawHUD() override;

	void SetPausedBanner(bool bPaused);
	void SetDebugVisible(bool bVisible);
	bool IsDebugVisible() const { return bDebugVisible; }

private:
	bool bShowPaused = false;

#if !UE_BUILD_SHIPPING
	bool bDebugVisible = true;
#else
	bool bDebugVisible = false;
#endif
};
