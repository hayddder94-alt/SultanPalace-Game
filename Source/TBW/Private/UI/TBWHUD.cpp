// The Betrayed Will / وصية الغدر
// Copyright (c) 2026. All rights reserved.

#include "UI/TBWHUD.h"
#include "Interaction/TBWInteractorComponent.h"
#include "Player/TBWPlayerCharacter.h"
#include "Player/TBWPlayerIdentityComponent.h"
#include "Player/TBWIdentityData.h"
#include "Core/TBWWorldStateSubsystem.h"
#include "Core/TBWVersion.h"
#include "Engine/Canvas.h"
#include "Engine/Engine.h"

void ATBWHUD::SetPausedBanner(bool bPaused)
{
	bShowPaused = bPaused;
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

	DrawText(TEXT("THE BETRAYED WILL  /  وصية الغدر"), FLinearColor(0.75f, 0.82f, 0.95f), 24.f, 18.f, nullptr, 1.1f);
	DrawText(FString::Printf(TEXT("%s   UE %s"), TBW_VERSION_STRING, TBW_ENGINE_LOCK), FLinearColor(0.55f, 0.55f, 0.55f), 24.f, 40.f, nullptr, 0.9f);

	FString IdentityName = TEXT("Evan");
	if (ATBWPlayerCharacter* Pawn = Cast<ATBWPlayerCharacter>(GetOwningPawn()))
	{
		if (UTBWPlayerIdentityComponent* Id = Pawn->GetIdentityComponent())
		{
			if (const UTBWIdentityData* Data = Id->GetIdentity())
			{
				IdentityName = Data->IdentityId.ToString();
			}
		}

		if (UTBWInteractorComponent* Interactor = Pawn->FindComponentByClass<UTBWInteractorComponent>())
		{
			const FText Prompt = Interactor->GetCurrentPrompt();
			if (!Prompt.IsEmpty())
			{
				const FString Line = FString::Printf(TEXT("[E]  %s"), *Prompt.ToString());
				DrawText(Line, FLinearColor(0.95f, 0.86f, 0.55f), W * 0.5f - 140.f, H * 0.78f, nullptr, 1.35f);
			}
		}
	}

	DrawText(FString::Printf(TEXT("Playing as %s    WASD move   Shift sprint   Ctrl crouch   Mouse look   E examine   Esc pause"), *IdentityName),
		FLinearColor(0.7f, 0.7f, 0.7f), 24.f, H - 36.f, nullptr, 0.95f);

	if (bShowPaused)
	{
		DrawText(TEXT("PAUSED"), FLinearColor(1.f, 0.92f, 0.7f), W * 0.5f - 60.f, H * 0.42f, nullptr, 2.0f);
	}

#if !UE_BUILD_SHIPPING
	if (UWorld* World = GetWorld())
	{
		if (UTBWWorldStateSubsystem* State = World->GetSubsystem<UTBWWorldStateSubsystem>())
		{
			TArray<FString> Lines;
			State->GetDebugLines(Lines);
			float Y = 70.f;
			DrawText(TEXT("WorldState"), FLinearColor(0.5f, 0.65f, 0.5f), W - 280.f, Y, nullptr, 0.9f);
			Y += 18.f;
			if (Lines.Num() == 0)
			{
				DrawText(TEXT("(no flags)"), FLinearColor(0.4f, 0.4f, 0.4f), W - 280.f, Y, nullptr, 0.85f);
			}
			for (const FString& Line : Lines)
			{
				DrawText(Line, FLinearColor(0.65f, 0.8f, 0.65f), W - 280.f, Y, nullptr, 0.85f);
				Y += 16.f;
			}
		}
	}
#endif
}
