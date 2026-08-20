// The Betrayed Will / وصية الغدر
// Copyright (c) 2026. All rights reserved.

#include "Characters/TBWStoryCharacter.h"
#include "Characters/TBWAnimLibrary.h"
#include "Characters/TBWCharacterLook.h"
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
	ApplyIdleAnimation();

#if UE_BUILD_SHIPPING
	if (NameLabel)
	{
		NameLabel->SetVisibility(false);
	}
#endif

	UE_LOG(LogTBW, Verbose, TEXT("Story character %s staged (%s body)."),
		*CharacterName.ToString(), bUsingSkeletal ? TEXT("skeletal") : TEXT("proxy"));
}

void ATBWStoryCharacter::ApplyIdleAnimation()
{
	// A staged character is not AI and gets none here: no controller, no
	// perception, no navigation. It gets a breathing loop, which is
	// presentation, not behaviour. A room of people holding a reference pose
	// reads as a bug; the same room breathing reads as a room.
	if (!bUsingSkeletal || !SkeletalBody || !SkeletalBody->GetSkeletalMeshAsset())
	{
		return;
	}

	const FTBWLocomotionClips& Clips =
		FTBWAnimLibrary::For(SkeletalBody->GetSkeletalMeshAsset()->GetSkeleton());

	UAnimSequence* Clip = Clips.Idle;
	switch (Pose)
	{
	case ETBWCharacterPose::Seated: Clip = Clips.Sit; break;
	case ETBWCharacterPose::Lying:  Clip = Clips.Lie; break;
	default: break;
	}

	// Seventeen idles started on the same frame at the same rate look like one
	// puppet copied seventeen times. A few percent of drift per character is
	// enough to break that, and it is deterministic - same cast, same look,
	// every run.
	const float Rate = 0.92f + FTBWCharacterLook::Jitter(CharacterName) * 0.16f;
	FTBWAnimLibrary::PlayLooping(SkeletalBody, Clip, Rate);

	if (Pose != ETBWCharacterPose::Standing && Clip == Clips.Idle)
	{
		// Say it. A seated character standing up is a staging error the eye
		// forgives for a whole session because everything else looks fine.
		UE_LOG(LogTBW, Warning,
			TEXT("%s is staged as %s but only an idle clip was found - it will stand."),
			*CharacterName.ToString(),
			Pose == ETBWCharacterPose::Seated ? TEXT("seated") : TEXT("lying"));
	}
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
	FTBWCharacterLook::Apply(SkeletalBody, BodyTint);

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
		// Rotate the PROXY box only.
		//
		// Pitching a skeletal mesh 90 degrees put a character head-down, half
		// inside a wall, with its legs out of the floor - which is what the
		// screenshot showed. The mesh pivot is at the feet and sits 88 cm below
		// the capsule, so rotating it swings the whole body underground.
		//
		// A lying human is an animation, not a transform. ApplyIdleAnimation
		// plays the lie/death clip; if none is found the character stands,
		// which is wrong but is obviously wrong rather than broken.
		if (Body && !bUsingSkeletal)
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
