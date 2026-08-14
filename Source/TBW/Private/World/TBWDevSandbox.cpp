// The Betrayed Will / وصية الغدر
// Copyright (c) 2026. All rights reserved.

#include "World/TBWDevSandbox.h"
#include "Interaction/TBWInteractableActor.h"
#include "Interaction/TBWTestDoor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/DirectionalLightComponent.h"
#include "Components/SkyLightComponent.h"
#include "Components/TextRenderComponent.h"
#include "Engine/StaticMesh.h"
#include "Engine/StaticMeshActor.h"
#include "Engine/DirectionalLight.h"
#include "Engine/SkyLight.h"
#include "Engine/TextRenderActor.h"
#include "Engine/CollisionProfile.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Materials/Material.h"
#include "UObject/ConstructorHelpers.h"
#include "TBW.h"

ATBWDevSandbox::ATBWDevSandbox()
{
	PrimaryActorTick.bCanEverTick = false;
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeFinder(TEXT("/Engine/BasicShapes/Cube.Cube"));
	if (CubeFinder.Succeeded())
	{
		CubeMesh = CubeFinder.Object;
	}
	PlayerSpawn = FTransform(FRotator(0.f, 0.f, 0.f), FVector(0.f, -400.f, 110.f));
}

void ATBWDevSandbox::BeginPlay()
{
	Super::BeginPlay();
	Rebuild();
}

void ATBWDevSandbox::ClearSpawned()
{
	for (AActor* Actor : Spawned)
	{
		if (IsValid(Actor))
		{
			Actor->Destroy();
		}
	}
	Spawned.Reset();
}

AActor* ATBWDevSandbox::AddBox(const FVector& Location, const FVector& ScaleMeters, const FLinearColor& Color)
{
	UWorld* World = GetWorld();
	if (!World || !CubeMesh)
	{
		return nullptr;
	}

	FActorSpawnParameters Params;
	Params.Owner = this;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	AStaticMeshActor* Box = World->SpawnActor<AStaticMeshActor>(Location, FRotator::ZeroRotator, Params);
	if (!Box)
	{
		return nullptr;
	}

	UStaticMeshComponent* MeshComp = Box->GetStaticMeshComponent();
	MeshComp->SetMobility(EComponentMobility::Static);
	MeshComp->SetStaticMesh(CubeMesh);
	MeshComp->SetWorldScale3D(ScaleMeters);
	if (UMaterial* Base = UMaterial::GetDefaultMaterial(MD_Surface))
	{
		if (UMaterialInstanceDynamic* Mid = UMaterialInstanceDynamic::Create(Base, this))
		{
			Mid->SetVectorParameterValue(TEXT("BaseColor"), Color);
			Mid->SetVectorParameterValue(TEXT("Color"), Color);
			MeshComp->SetMaterial(0, Mid);
		}
	}
	MeshComp->SetCollisionProfileName(UCollisionProfile::BlockAll_ProfileName);
	Spawned.Add(Box);
	return Box;
}

void ATBWDevSandbox::AddLabel(const FVector& Location, const FString& Text)
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}
	FActorSpawnParameters Params;
	Params.Owner = this;
	if (ATextRenderActor* Label = World->SpawnActor<ATextRenderActor>(Location, FRotator(0.f, 180.f, 0.f), Params))
	{
		if (UTextRenderComponent* TextComp = Label->GetTextRender())
		{
			TextComp->SetText(FText::FromString(Text));
			TextComp->SetTextRenderColor(FColor(220, 200, 160));
			TextComp->SetWorldSize(26.f);
			TextComp->SetHorizontalAlignment(EHTA_Center);
		}
		Spawned.Add(Label);
	}
}

ATBWInteractableActor* ATBWDevSandbox::AddPickup(
	const FVector& Location,
	const FVector& ScaleMeters,
	const FLinearColor& Color,
	const FText& Prompt,
	const FText& Examine,
	FName Flag)
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return nullptr;
	}
	FActorSpawnParameters Params;
	Params.Owner = this;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	ATBWInteractableActor* Item = World->SpawnActor<ATBWInteractableActor>(Location, FRotator::ZeroRotator, Params);
	if (!Item)
	{
		return nullptr;
	}
	Item->Configure(Prompt, Examine, Flag, 1);
	Item->SetActorScale3D(ScaleMeters);
	Spawned.Add(Item);
	return Item;
}

void ATBWDevSandbox::AddCrouchGate()
{
	// Lintel at 140 cm: standing capsule (~176) blocked, crouched (~116) passes.
	AddBox(FVector(-700.f, -200.f, 70.f), FVector(0.3f, 0.3f, 1.4f), FLinearColor(0.18f, 0.18f, 0.2f));
	AddBox(FVector(-700.f, 200.f, 70.f), FVector(0.3f, 0.3f, 1.4f), FLinearColor(0.18f, 0.18f, 0.2f));
	AddBox(FVector(-700.f, 0.f, 155.f), FVector(0.3f, 4.2f, 0.25f), FLinearColor(0.28f, 0.16f, 0.12f));
	AddLabel(FVector(-700.f, 0.f, 200.f), TEXT("CROUCH GATE"));
}

void ATBWDevSandbox::AddTestDoor()
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}
	FActorSpawnParameters Params;
	Params.Owner = this;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	if (ATBWTestDoor* Door = World->SpawnActor<ATBWTestDoor>(FVector(500.f, -50.f, 110.f), FRotator::ZeroRotator, Params))
	{
		Spawned.Add(Door);
	}
	AddBox(FVector(500.f, -130.f, 110.f), FVector(0.25f, 0.25f, 2.2f), FLinearColor(0.2f, 0.2f, 0.2f));
	AddBox(FVector(500.f, 80.f, 110.f), FVector(0.25f, 0.25f, 2.2f), FLinearColor(0.2f, 0.2f, 0.2f));
	AddLabel(FVector(500.f, -20.f, 240.f), TEXT("TEST DOOR"));
}

void ATBWDevSandbox::AddLights()
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}
	FActorSpawnParameters Params;
	Params.Owner = this;
	if (ADirectionalLight* Sun = World->SpawnActor<ADirectionalLight>(FVector(0.f, 0.f, 600.f), FRotator(-50.f, 30.f, 0.f), Params))
	{
		if (UDirectionalLightComponent* Comp = Cast<UDirectionalLightComponent>(Sun->GetLightComponent()))
		{
			Comp->SetIntensity(6.f);
			Comp->SetCastShadows(false);
		}
		Spawned.Add(Sun);
	}
	if (ASkyLight* Sky = World->SpawnActor<ASkyLight>(FVector(0.f, 0.f, 400.f), FRotator::ZeroRotator, Params))
	{
		Spawned.Add(Sky);
	}
}

void ATBWDevSandbox::Rebuild()
{
	ClearSpawned();
	if (!CubeMesh)
	{
		UE_LOG(LogTBW, Error, TEXT("L_Dev_Sandbox: engine cube missing."));
		return;
	}

	// Floor 20m x 20m
	AddBox(FVector(0.f, 0.f, -10.f), FVector(20.f, 20.f, 0.2f), FLinearColor(0.22f, 0.22f, 0.22f));

	// Low boundary so the player cannot walk off the world
	AddBox(FVector(0.f, 1000.f, 80.f), FVector(20.f, 0.2f, 1.6f), FLinearColor(0.15f, 0.15f, 0.15f));
	AddBox(FVector(0.f, -1000.f, 80.f), FVector(20.f, 0.2f, 1.6f), FLinearColor(0.15f, 0.15f, 0.15f));
	AddBox(FVector(1000.f, 0.f, 80.f), FVector(0.2f, 20.f, 1.6f), FLinearColor(0.15f, 0.15f, 0.15f));
	AddBox(FVector(-1000.f, 0.f, 80.f), FVector(0.2f, 20.f, 1.6f), FLinearColor(0.15f, 0.15f, 0.15f));

	// Marker pillar
	AddBox(FVector(0.f, 200.f, 100.f), FVector(0.4f, 0.4f, 2.0f), FLinearColor(0.35f, 0.28f, 0.16f));

	AddPickup(
		FVector(250.f, 200.f, 50.f),
		FVector(0.4f, 0.4f, 0.4f),
		FLinearColor(0.7f, 0.55f, 0.2f),
		NSLOCTEXT("TBW", "DevClue", "Examine test clue"),
		NSLOCTEXT("TBW", "DevClueBody", "World-state test: ClueFound_01 is now set."),
		TEXT("ClueFound_01"));

	AddPickup(
		FVector(-250.f, 200.f, 50.f),
		FVector(0.5f, 0.2f, 0.7f),
		FLinearColor(0.2f, 0.25f, 0.4f),
		NSLOCTEXT("TBW", "DevWill", "Read the test will"),
		NSLOCTEXT("TBW", "DevWillBody", "World-state test: WillWasRead is now set."),
		TEXT("WillWasRead"));

	AddLabel(FVector(0.f, 0.f, 280.f), TEXT("L_DEV_SANDBOX  /  PHASE 1 SYSTEMS TEST"));
	AddLights();

	PlayerSpawn = FTransform(FRotator(0.f, 0.f, 0.f), FVector(0.f, -400.f, 110.f));
	UE_LOG(LogTBW, Log, TEXT("L_Dev_Sandbox greybox ready (%d actors)."), Spawned.Num());
}
