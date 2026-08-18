// The Betrayed Will / وصية الغدر
// Copyright (c) 2026. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "TBWHUD.generated.h"

class SWidget;
class STextBlock;

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

	/** Language for subtitles and objectives. Arabic is the default. */
	UFUNCTION(BlueprintCallable, Category = "TBW|UI")
	void SetArabicUI(bool bInArabic);

	UFUNCTION(BlueprintPure, Category = "TBW|UI")
	bool IsArabicUI() const { return bArabicUI; }

private:
	void AddArabicTitleWidget();
	void RemoveArabicTitleWidget();

	/**
	 * Subtitles and the objective line are Slate, not Canvas DrawText.
	 * Canvas goes through DroidSansFallback and cannot shape Arabic; Slate uses
	 * HarfBuzz and can. This is the same reason the title moved to Slate in Phase 1.
	 */
	void AddNarrativeWidgets();
	void RemoveNarrativeWidgets();
	void RefreshNarrativeText();

	bool bArabicUI = true;

	bool bShowPaused = false;

	TSharedPtr<SWidget> NarrativeHost;
	TSharedPtr<STextBlock> SubtitleText;
	TSharedPtr<STextBlock> ObjectiveText;

#if !UE_BUILD_SHIPPING
	bool bDebugVisible = true;
	TSharedPtr<SWidget> ArabicTitleHost;
#else
	bool bDebugVisible = false;
#endif
};
