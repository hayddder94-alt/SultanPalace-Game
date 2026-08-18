// The Betrayed Will / وصية الغدر
// Copyright (c) 2026. All rights reserved.

#include "UI/TBWHUD.h"
#include "Interaction/TBWInteractorComponent.h"
#include "Player/TBWPlayerCharacter.h"
#include "Player/TBWPlayerIdentityComponent.h"
#include "Player/TBWIdentityData.h"
#include "Core/TBWWorldStateSubsystem.h"
#include "Narrative/TBWDialogueSubsystem.h"
#include "Narrative/TBWObjectiveSubsystem.h"
#include "Core/TBWVersion.h"
#include "Engine/Canvas.h"
#include "Engine/Engine.h"
#include "Engine/GameViewportClient.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Misc/Paths.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "Fonts/SlateFontInfo.h"
#include "Styling/CoreStyle.h"

namespace TBWHudText
{
	// ASCII-only source so MSVC cannot mis-decode the Arabic title as CP1252.
	// وصية الغدر
	static const TCHAR* ArabicTitle =
		TEXT("THE BETRAYED WILL  /  \u0648\u0635\u064A\u0629 \u0627\u0644\u063A\u062F\u0631");

	static FString FontFile()
	{
		return FPaths::ConvertRelativePathToFull(
			FPaths::ProjectContentDir() / TEXT("TBW/UI/Fonts/DejaVuSans.ttf"));
	}
}

void ATBWHUD::BeginPlay()
{
	Super::BeginPlay();
	AddArabicTitleWidget();
	AddNarrativeWidgets();
}

void ATBWHUD::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	RemoveNarrativeWidgets();
	RemoveArabicTitleWidget();
	Super::EndPlay(EndPlayReason);
}

void ATBWHUD::SetArabicUI(bool bInArabic)
{
	bArabicUI = bInArabic;
	if (UWorld* World = GetWorld())
	{
		if (UTBWDialogueSubsystem* Dialogue = World->GetSubsystem<UTBWDialogueSubsystem>())
		{
			Dialogue->SetArabic(bArabicUI);
		}
	}
	RefreshNarrativeText();
}

void ATBWHUD::AddNarrativeWidgets()
{
	if (NarrativeHost.IsValid() || !GEngine || !GEngine->GameViewport)
	{
		return;
	}

	const FString FontPath = TBWHudText::FontFile();
	FSlateFontInfo Body = FPaths::FileExists(FontPath)
		? FSlateFontInfo(FontPath, 22)
		: FCoreStyle::GetDefaultFontStyle("Regular", 22);
	FSlateFontInfo Small = FPaths::FileExists(FontPath)
		? FSlateFontInfo(FontPath, 16)
		: FCoreStyle::GetDefaultFontStyle("Regular", 16);

	SAssignNew(SubtitleText, STextBlock)
		.Text(FText::GetEmpty())
		.Font(Body)
		.ColorAndOpacity(FLinearColor(0.96f, 0.94f, 0.88f))
		.Justification(ETextJustify::Center)
		.AutoWrapText(true)
		.TextShapingMethod(ETextShapingMethod::Auto)
		.TextFlowDirection(ETextFlowDirection::Auto);

	SAssignNew(ObjectiveText, STextBlock)
		.Text(FText::GetEmpty())
		.Font(Small)
		.ColorAndOpacity(FLinearColor(0.82f, 0.72f, 0.45f))
		.TextShapingMethod(ETextShapingMethod::Auto)
		.TextFlowDirection(ETextFlowDirection::Auto);

	NarrativeHost = SNew(SVerticalBox)

		+ SVerticalBox::Slot()
		.AutoHeight()
		.HAlign(HAlign_Right)
		.Padding(FMargin(0.f, 46.f, 28.f, 0.f))
		[
			ObjectiveText.ToSharedRef()
		]

		+ SVerticalBox::Slot()
		.FillHeight(1.f)
		.VAlign(VAlign_Bottom)
		.HAlign(HAlign_Center)
		.Padding(FMargin(0.f, 0.f, 0.f, 96.f))
		[
			SNew(SBox)
			.WidthOverride(980.f)
			[
				SubtitleText.ToSharedRef()
			]
		];

	GEngine->GameViewport->AddViewportWidgetContent(NarrativeHost.ToSharedRef(), 9000);
}

void ATBWHUD::RemoveNarrativeWidgets()
{
	if (NarrativeHost.IsValid() && GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->RemoveViewportWidgetContent(NarrativeHost.ToSharedRef());
	}
	NarrativeHost.Reset();
	SubtitleText.Reset();
	ObjectiveText.Reset();
}

void ATBWHUD::RefreshNarrativeText()
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	if (SubtitleText.IsValid())
	{
		FString Line;
		if (UTBWDialogueSubsystem* Dialogue = World->GetSubsystem<UTBWDialogueSubsystem>())
		{
			Line = Dialogue->GetSubtitle();
		}
		SubtitleText->SetText(FText::FromString(Line));
	}

	if (ObjectiveText.IsValid())
	{
		FString Objective;
		if (UTBWObjectiveSubsystem* Objectives = World->GetSubsystem<UTBWObjectiveSubsystem>())
		{
			Objective = Objectives->GetCurrentObjective(bArabicUI);
		}
		ObjectiveText->SetText(FText::FromString(Objective));
	}
}

void ATBWHUD::AddArabicTitleWidget()
{
#if !UE_BUILD_SHIPPING
	if (ArabicTitleHost.IsValid())
	{
		return;
	}
	if (!GEngine || !GEngine->GameViewport)
	{
		return;
	}

	const FString FontPath = TBWHudText::FontFile();
	FSlateFontInfo FontInfo;
	if (FPaths::FileExists(FontPath))
	{
		FontInfo = FSlateFontInfo(FontPath, 16);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("TBWHUD: Arabic font missing at %s"), *FontPath);
		FontInfo = FCoreStyle::GetDefaultFontStyle("Regular", 16);
	}

	TSharedRef<SWidget> Title = SNew(STextBlock)
		.Text(FText::FromString(TBWHudText::ArabicTitle))
		.Font(FontInfo)
		.ColorAndOpacity(FLinearColor(0.75f, 0.82f, 0.95f))
		.TextShapingMethod(ETextShapingMethod::Auto)
		.TextFlowDirection(ETextFlowDirection::Auto);

	ArabicTitleHost = SNew(SHorizontalBox)
		+ SHorizontalBox::Slot()
		.HAlign(HAlign_Left)
		.VAlign(VAlign_Top)
		.Padding(FMargin(24.f, 14.f, 24.f, 0.f))
		[
			Title
		];

	GEngine->GameViewport->AddViewportWidgetContent(ArabicTitleHost.ToSharedRef(), 10000);
#endif
}

void ATBWHUD::RemoveArabicTitleWidget()
{
#if !UE_BUILD_SHIPPING
	if (ArabicTitleHost.IsValid() && GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->RemoveViewportWidgetContent(ArabicTitleHost.ToSharedRef());
	}
	ArabicTitleHost.Reset();
#endif
}

void ATBWHUD::SetPausedBanner(bool bPaused)
{
	bShowPaused = bPaused;
}

void ATBWHUD::SetDebugVisible(bool bVisible)
{
#if !UE_BUILD_SHIPPING
	bDebugVisible = bVisible;
#else
	bDebugVisible = false;
#endif
}

void ATBWHUD::DrawHUD()
{
	Super::DrawHUD();
	if (!Canvas)
	{
		return;
	}

	const float W = Canvas->SizeX;
	const float H = Canvas->SizeY;

	// Slate widgets do not poll; the HUD is the one thing that ticks every frame.
	RefreshNarrativeText();

	// Arabic title is a Slate widget (see BeginPlay). Do not Canvas-draw it
	// through DroidSansFallback — that produced glyph warnings / mojibake.

	DrawText(FString::Printf(TEXT("%s   UE %s   L_Dev_Sandbox"), TBW_VERSION_STRING, TBW_ENGINE_LOCK),
		FLinearColor(0.55f, 0.55f, 0.55f), 24.f, 36.f, nullptr, 0.85f);

	FString IdentityName = TEXT("Evan");
	FVector Location = FVector::ZeroVector;
	float Speed = 0.f;
	ATBWPlayerCharacter* Pawn = Cast<ATBWPlayerCharacter>(GetOwningPawn());
	if (Pawn)
	{
		Location = Pawn->GetActorLocation();
		if (const UCharacterMovementComponent* Move = Pawn->GetCharacterMovement())
		{
			Speed = Move->Velocity.Size2D();
		}
		if (UTBWPlayerIdentityComponent* Id = Pawn->GetIdentityComponent())
		{
			if (const UTBWIdentityData* Data = Id->GetIdentity())
			{
				IdentityName = Data->IdentityId.ToString();
			}
		}
		if (UTBWInteractorComponent* Interactor = Pawn->GetInteractor())
		{
			const FText Prompt = Interactor->GetCurrentPrompt();
			if (!Prompt.IsEmpty())
			{
				DrawText(FString::Printf(TEXT("[E]  %s"), *Prompt.ToString()),
					FLinearColor(0.95f, 0.86f, 0.55f), W * 0.5f - 140.f, H * 0.76f, nullptr, 1.3f);
			}
		}
	}

	DrawText(TEXT("WASD move   Shift sprint   Ctrl crouch   Mouse look   E interact   LMB/RMB reserved   Esc pause"),
		FLinearColor(0.65f, 0.65f, 0.65f), 24.f, H - 28.f, nullptr, 0.85f);

	if (bShowPaused)
	{
		DrawText(TEXT("PAUSED"), FLinearColor(1.f, 0.92f, 0.7f), W * 0.5f - 60.f, H * 0.42f, nullptr, 2.0f);
	}

#if !UE_BUILD_SHIPPING
	if (!bDebugVisible)
	{
		return;
	}

	float Fps = 0.f;
	float FrameMs = 0.f;
	if (GEngine)
	{
		const float Delta = GetWorld() ? GetWorld()->GetDeltaSeconds() : 0.f;
		FrameMs = Delta * 1000.f;
		Fps = Delta > KINDA_SMALL_NUMBER ? (1.f / Delta) : 0.f;
	}

	DrawText(FString::Printf(TEXT("FPS %.0f   frame %.2f ms"), Fps, FrameMs),
		FLinearColor(0.8f, 0.8f, 0.5f), 24.f, 58.f, nullptr, 0.9f);
	DrawText(FString::Printf(TEXT("XYZ  %.0f  %.0f  %.0f   spd %.0f"), Location.X, Location.Y, Location.Z, Speed),
		FLinearColor(0.7f, 0.75f, 0.85f), 24.f, 76.f, nullptr, 0.9f);
	FString MoveName = TEXT("Idle");
	FString InteractStatus = TEXT("focus none");
	if (Pawn)
	{
		MoveName = Pawn->GetMoveStateName();
		if (UTBWInteractorComponent* Interactor = Pawn->GetInteractor())
		{
			InteractStatus = Interactor->GetStatusLine();
		}
	}
	DrawText(FString::Printf(TEXT("Identity  %s   move  %s   %s"), *IdentityName, *MoveName, *InteractStatus),
		FLinearColor(0.7f, 0.8f, 0.7f), 24.f, 94.f, nullptr, 0.9f);
	DrawText(TEXT("system  WorldState+Input+Interact  OK"),
		FLinearColor(0.55f, 0.65f, 0.55f), 24.f, 112.f, nullptr, 0.8f);

	if (UWorld* World = GetWorld())
	{
		if (UTBWWorldStateSubsystem* State = World->GetSubsystem<UTBWWorldStateSubsystem>())
		{
			TArray<FString> Lines;
			State->GetDebugLines(Lines);
			float Y = 58.f;
			DrawText(TEXT("WorldState  (tbw.Debug.Reset)"), FLinearColor(0.5f, 0.65f, 0.5f), W - 320.f, Y, nullptr, 0.85f);
			Y += 18.f;
			if (Lines.Num() == 0)
			{
				DrawText(TEXT("(no flags)"), FLinearColor(0.4f, 0.4f, 0.4f), W - 320.f, Y, nullptr, 0.8f);
			}
			for (const FString& Line : Lines)
			{
				DrawText(Line, FLinearColor(0.65f, 0.8f, 0.65f), W - 320.f, Y, nullptr, 0.8f);
				Y += 16.f;
			}
		}
	}
#endif
}
