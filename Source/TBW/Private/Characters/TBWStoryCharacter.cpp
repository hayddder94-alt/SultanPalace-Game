// The Betrayed Will / وصية الغدر
// Copyright (c) 2026. All rights reserved.

#include "Characters/TBWStoryCharacter.h"
#include "Narrative/TBWDialogueSubsystem.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/TextRenderComponent.h"
#include "Engine/SkeletalMesh.h"
#include "Engine/StaticMesh.h"
#include "Engine/CollisionProfile.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Materials/Material.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/World.h"
#include "TBW.h"

ATBWStoryCharacter::ATBWStoryCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	SetRootComponent(Capsule);
	Capsule->InitCapsuleSize(36.f, 88.f);
	Capsule->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);

	SkeletalBody = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalBody"));
	SkeletalBody->SetupAttachment(Capsule);
	SkeletalBody->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -88.f), FRotator(0.f, -90.f, 0.f));
	SkeletalBody->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SkeletalBody->SetVisibility(false);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeFinder(TEXT("/Engine/BasicShapes/Cube.Cube"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereFinder(TEXT("/Engine/BasicShapes/Sphere.Sphere"));

	// Proxy: a body block and a head. Crude on purpose - it reads as a person at
	// gameplay distance, and nobody can mistake it for finished art.
	ProxyBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProxyBody"));
	ProxyBody->SetupAttachment(Capsule);
	ProxyBody->SetRelativeLocation(FVector(0.f, 0.f, -88.f));
	ProxyBody->SetRelativeScale3D(FVector(0.40f, 0.32f, 1.45f));
	ProxyBody->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	if (CubeFinder.Succeeded())
	{
		ProxyBody->SetStaticMesh(CubeFinder.Object);
	}

	ProxyHead = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProxyHead"));
	ProxyHead->SetupAttachment(Capsule);
	ProxyHead->SetRelativeLocation(FVector(0.f, 0.f, 62.f));
	ProxyHead->SetRelativeScale3D(FVector(0.22f, 0.22f, 0.26f));
	ProxyHead->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	if (SphereFinder.Succeeded())
	{
		ProxyHead->SetStaticMesh(SphereFinder.Object);
	}

	NameLabel = CreateDefaultSubobject<UTextRenderComponent>(TEXT("NameLabel"));
	NameLabel->SetupAttachment(Capsule);
	NameLabel->SetRelativeLocation(FVector(0.f, 0.f, 104.f));
	NameLabel->SetHorizontalAlignment(EHTA_Center);
	NameLabel->SetWorldSize(18.f);
	NameLabel->SetTextRenderColor(FColor(226, 206, 160));
	NameLabel->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ATBWStoryCharacter::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	ApplyPose();
	if (NameLabel)
	{
		NameLabel->SetText(FText::FromName(CharacterName));
		NameLabel->SetVisibility(bShowNameLabel);
	}
}

void ATBWStoryCharacter::BeginPlay()
{
	Super::BeginPlay();
	ResolveBody();
	ApplyPose();

#if UE_BUILD_SHIPPING
	if (NameLabel)
	{
		NameLabel->SetVisibility(false);
	}
#endif

	UE_LOG(LogTBW, Verbose, TEXT("Story character %s staged (%s body)."),
		*CharacterName.ToString(), bUsingSkeletal ? TEXT("skeletal") : TEXT("proxy"));
}

void ATBWStoryCharacter::ResolveBody()
{
	// Same ladder the player uses: our own art first, then the free Third Person
	// feature pack, then nothing - in which case the proxy stays visible.
	static const TCHAR* Candidates[] =
	{
		TEXT("/Game/TBW/Characters/SKM_Cast.SKM_Cast"),
		TEXT("/Game/Characters/Mannequins/Meshes/SKM_Quinn.SKM_Quinn"),
		TEXT("/Game/Characters/Mannequins/Meshes/SKM_Manny.SKM_Manny"),
		TEXT("/Game/Characters/Mannequins/Meshes/SKM_Quinn_Simple.SKM_Quinn_Simple"),
		TEXT("/Game/Characters/Mannequins/Meshes/SKM_Manny_Simple.SKM_Manny_Simple"),
		TEXT("/MoverTests/Characters/Mannequins/Meshes/SKM_Manny.SKM_Manny"),
		TEXT("/MoverExamples/Characters/Mannequins/Meshes/SKM_Manny_Simple.SKM_Manny_Simple"),
		TEXT("/AnimToTexture/Characters/Mannequin/Meshes/SKM_Mannequin.SKM_Mannequin")
	};

	USkeletalMesh* Mesh = nullptr;
	for (const TCHAR* Path : Candidates)
	{
		Mesh = LoadObject<USkeletalMesh>(nullptr, Path, nullptr, LOAD_NoWarn | LOAD_Quiet);
		if (Mesh)
		{
			break;
		}
	}

	if (!Mesh || !SkeletalBody)
	{
		bUsingSkeletal = false;
		return;
	}

	SkeletalBody->SetSkeletalMesh(Mesh);
	SkeletalBody->SetVisibility(true);
	bUsingSkeletal = true;

	if (ProxyBody)
	{
		ProxyBody->SetVisibility(false);
	}
	if (ProxyHead)
	{
		ProxyHead->SetVisibility(false);
	}
}

void ATBWStoryCharacter::ApplyPose()
{
	// Poses are staged with the capsule, not with animation. Real animation comes
	// with the animation pass; until then the silhouette must still read correctly
	// from the camera, because blocking a scene is a camera decision.
	USceneComponent* Body = bUsingSkeletal ? Cast<USceneComponent>(SkeletalBody) : Cast<USceneComponent>(ProxyBody);

	switch (Pose)
	{
	case ETBWCharacterPose::Seated:
		if (Capsule)
		{
			Capsule->SetCapsuleSize(36.f, 60.f);
		}
		if (ProxyBody)
		{
			ProxyBody->SetRelativeLocation(FVector(0.f, 0.f, -60.f));
			ProxyBody->SetRelativeScale3D(FVector(0.40f, 0.32f, 0.95f));
		}
		if (ProxyHead)
		{
			ProxyHead->SetRelativeLocation(FVector(0.f, 0.f, 30.f));
		}
		if (NameLabel)
		{
			NameLabel->SetRelativeLocation(FVector(0.f, 0.f, 74.f));
		}
		break;

	case ETBWCharacterPose::Lying:
		if (Capsule)
		{
			Capsule->SetCapsuleSize(36.f, 36.f);
		}
		if (ProxyBody)
		{
			ProxyBody->SetRelativeLocation(FVector(0.f, 0.f, -24.f));
			ProxyBody->SetRelativeRotation(FRotator(90.f, 0.f, 0.f));
			ProxyBody->SetRelativeScale3D(FVector(0.40f, 0.32f, 1.45f));
		}
		if (ProxyHead)
		{
			ProxyHead->SetRelativeLocation(FVector(-62.f, 0.f, -8.f));
		}
		if (NameLabel)
		{
			NameLabel->SetRelativeLocation(FVector(0.f, 0.f, 44.f));
		}
		if (Body)
		{
			Body->SetRelativeRotation(FRotator(90.f, -90.f, 0.f));
		}
		break;

	default:
		break;
	}
}

bool ATBWStoryCharacter::CanInteract_Implementation(APawn* InstigatorPawn) const
{
	return InstigatorPawn != nullptr && !PlaysDialogueScene.IsNone();
}

FText ATBWStoryCharacter::GetPrompt_Implementation(APawn* InstigatorPawn) const
{
	return FText::FromString(FString::Printf(TEXT("Speak to %s"), *CharacterName.ToString()));
}

void ATBWStoryCharacter::Interact_Implementation(APawn* InstigatorPawn)
{
	if (PlaysDialogueScene.IsNone())
	{
		return;
	}
	if (UWorld* World = GetWorld())
	{
		if (UTBWDialogueSubsystem* Dialogue = World->GetSubsystem<UTBWDialogueSubsystem>())
		{
			Dialogue->PlayScene(PlaysDialogueScene);
		}
	}
}
