// The Betrayed Will / وصية الغدر
// Copyright (c) 2026. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "TBWHUD.generated.h"

class SWidget;

/**
 * Phase 1 HUD. Shipping strips the debug overlay.
 * Arabic title is drawn with Slate (ICU/HarfBuzz) and a DejaVu face,
 * not Canvas + DroidSansFallback.
 */
UCLASS()
class TBW_API ATBWHUD : public AHUD
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void DrawHUD() override;

	void SetPausedBanner(bool bPaused);
	void SetDebugVisible(bool bVisible);
	bool IsDebugVisible() const { return bDebugVisible; }

private:
	void AddArabicTitleWidget();
	void RemoveArabicTitleWidget();

	bool bShowPaused = false;

#if !UE_BUILD_SHIPPING
	bool bDebugVisible = true;
	TSharedPtr<SWidget> ArabicTitleHost;
#else
	bool bDebugVisible = false;
#endif
};
