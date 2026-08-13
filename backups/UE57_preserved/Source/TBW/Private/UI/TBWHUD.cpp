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
#include "GameFramework/CharacterMovementComponent.h"

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

	DrawText(TEXT("THE BETRAYED WILL  /  وصية الغدر"), FLinearColor(0.75f, 0.82f, 0.95f), 24.f, 16.f, nullptr, 1.05f);
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
	DrawText(FString::Printf(TEXT("Identity  %s   system  WorldState+Input+Interact  OK"), *IdentityName),
		FLinearColor(0.7f, 0.8f, 0.7f), 24.f, 94.f, nullptr, 0.9f);

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
