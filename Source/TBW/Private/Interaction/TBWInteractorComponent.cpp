// The Betrayed Will / وصية الغدر
// Copyright (c) 2026. All rights reserved.

#include "Interaction/TBWInteractorComponent.h"
#include "Interaction/TBWInteractable.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Pawn.h"
#include "Engine/World.h"
#include "CollisionQueryParams.h"
#include "TBW.h"

UTBWInteractorComponent::UTBWInteractorComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickGroup = TG_PostUpdateWork;
}

void UTBWInteractorComponent::BeginPlay()
{
	Super::BeginPlay();
}

AActor* UTBWInteractorComponent::ResolveInteractable(AActor* HitActor)
{
	for (AActor* Cursor = HitActor; Cursor; Cursor = Cursor->GetOwner())
	{
		if (Cursor->Implements<UTBWInteractable>())
		{
			return Cursor;
		}
	}
	return nullptr;
}

void UTBWInteractorComponent::RefreshFocus()
{
	FocusedActor = nullptr;
	CurrentPrompt = FText::GetEmpty();

	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	UWorld* World = GetWorld();
	if (!OwnerPawn || !World)
	{
		return;
	}

	FVector Start;
	FVector Direction;
	if (const UCameraComponent* Camera = OwnerPawn->FindComponentByClass<UCameraComponent>())
	{
		Start = Camera->GetComponentLocation();
		Direction = Camera->GetForwardVector();
	}
	else
	{
		Start = OwnerPawn->GetActorLocation() + FVector(0.f, 0.f, 60.f);
		Direction = OwnerPawn->GetActorForwardVector();
	}

	const FVector End = Start + Direction * TraceDistance;

	FCollisionQueryParams Params(SCENE_QUERY_STAT(TBWInteract), false, OwnerPawn);
	FHitResult Hit;
	const bool bHit = World->SweepSingleByChannel(
		Hit,
		Start,
		End,
		FQuat::Identity,
		ECC_Visibility,
		FCollisionShape::MakeSphere(TraceRadius),
		Params);

	AActor* Candidate = bHit ? ResolveInteractable(Hit.GetActor()) : nullptr;
	if (!Candidate)
	{
		return;
	}

	if (!ITBWInteractable::Execute_CanInteract(Candidate, OwnerPawn))
	{
		return;
	}

	FocusedActor = Candidate;
	CurrentPrompt = ITBWInteractable::Execute_GetPrompt(Candidate, OwnerPawn);
}

FString UTBWInteractorComponent::GetStatusLine() const
{
	if (AActor* Target = FocusedActor.Get())
	{
		return FString::Printf(TEXT("focus %s"), *Target->GetName());
	}
	return TEXT("focus none");
}

void UTBWInteractorComponent::TryInteract()
{
	RefreshFocus();
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	AActor* Target = FocusedActor.Get();
	if (!OwnerPawn || !Target)
	{
		return;
	}

	const float Now = GetWorld() ? GetWorld()->GetTimeSeconds() : 0.f;
	if (Now - LastInteractTime < InteractCooldown)
	{
		return;
	}
	LastInteractTime = Now;

	UE_LOG(LogTBW, Log, TEXT("Interact: %s"), *GetNameSafe(Target));
	ITBWInteractable::Execute_Interact(Target, OwnerPawn);
}
