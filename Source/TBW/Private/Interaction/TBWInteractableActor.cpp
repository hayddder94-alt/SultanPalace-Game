// The Betrayed Will / وصية الغدر
// Copyright (c) 2026. All rights reserved.

#include "Interaction/TBWInteractableActor.h"
#include "Core/TBWWorldStateSubsystem.h"
#include "Narrative/TBWDialogueSubsystem.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/CollisionProfile.h"
#include "Engine/Engine.h"
#include "TBW.h"

ATBWInteractableActor::ATBWInteractableActor()
{
	PrimaryActorTick.bCanEverTick = false;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(MeshComponent);
	MeshComponent->SetCollisionProfileName(UCollisionProfile::BlockAll_ProfileName);
	MeshComponent->SetGenerateOverlapEvents(false);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeFinder(TEXT("/Engine/BasicShapes/Cube.Cube"));
	if (CubeFinder.Succeeded())
	{
		MeshComponent->SetStaticMesh(CubeFinder.Object);
	}
}

void ATBWInteractableActor::Configure(const FText& InPrompt, const FText& InExamine, FName InFlag, int32 InFlagValue)
{
	PromptText = InPrompt;
	ExamineText = InExamine;
	SetsFlag = InFlag;
	FlagValue = InFlagValue;
}

bool ATBWInteractableActor::CanInteract_Implementation(APawn* InstigatorPawn) const
{
	return InstigatorPawn != nullptr && (!bOnce || !bConsumed);
}

FText ATBWInteractableActor::GetPrompt_Implementation(APawn* InstigatorPawn) const
{
	return PromptText.IsEmpty() ? NSLOCTEXT("TBW", "Examine", "Examine") : PromptText;
}

void ATBWInteractableActor::Interact_Implementation(APawn* InstigatorPawn)
{
	if (!CanInteract_Implementation(InstigatorPawn))
	{
		return;
	}

	if (bOnce)
	{
		bConsumed = true;
	}

	if (UWorld* World = GetWorld())
	{
		if (!PlaysDialogueScene.IsNone())
		{
			if (UTBWDialogueSubsystem* Dialogue = World->GetSubsystem<UTBWDialogueSubsystem>())
			{
				Dialogue->PlayScene(PlaysDialogueScene);
			}
		}

		if (UTBWWorldStateSubsystem* State = World->GetSubsystem<UTBWWorldStateSubsystem>())
		{
			if (!SetsFlag.IsNone())
			{
				State->SetFlag(SetsFlag, FlagValue);
			}
		}
	}

	if (!ExamineText.IsEmpty())
	{
		UE_LOG(LogTBW, Display, TEXT("[Examine] %s"), *ExamineText.ToString());
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.5f, FColor(255, 236, 180), ExamineText.ToString());
		}
	}
}
