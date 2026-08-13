// The Betrayed Will / وصية الغدر
// Copyright (c) 2026. All rights reserved.

#include "World/TBWEastWingSandbox.h"
#include "Interaction/TBWInteractableActor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/DirectionalLightComponent.h"
#include "Components/SkyLightComponent.h"
#include "Components/TextRenderComponent.h"
#include "Engine/StaticMesh.h"
#include "Engine/StaticMeshActor.h"
#include "Engine/DirectionalLight.h"
#include "Engine/SkyLight.h"
#include "Engine/ExponentialHeightFog.h"
#include "Engine/TextRenderActor.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Materials/Material.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/CollisionProfile.h"
#include "TBW.h"

namespace TBWSandbox
{
	static const FLinearColor Mud(0.38f, 0.26f, 0.16f);
	static const FLinearColor MudDark(0.22f, 0.16f, 0.10f);
	static const FLinearColor HallFloor(0.20f, 0.17f, 0.13f);
	static const FLinearColor Glaze(0.07f, 0.16f, 0.32f);
	static const FLinearColor Annex(0.16f, 0.14f, 0.12f);
	static const FLinearColor Water(0.04f, 0.10f, 0.16f);
	static const FLinearColor Soft(0.35f, 0.08f, 0.08f);
	static const FLinearColor Accent(0.55f, 0.38f, 0.16f);
}

ATBWEastWingSandbox::ATBWEastWingSandbox()
{
	PrimaryActorTick.bCanEverTick = false;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeFinder(TEXT("/Engine/BasicShapes/Cube.Cube"));
	if (CubeFinder.Succeeded())
	{
		CubeMesh = CubeFinder.Object;
	}

	EvanSpawn = FTransform(FRotator(0.f, 90.f, 0.f), FVector(-550.f, -1300.f, 110.f));
}

void ATBWEastWingSandbox::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
}

void ATBWEastWingSandbox::BeginPlay()
{
	Super::BeginPlay();
	Rebuild();
}

void ATBWEastWingSandbox::Rebuild()
{
	ClearSpawned();
	if (!CubeMesh)
	{
		UE_LOG(LogTBW, Error, TEXT("EastWingSandbox: engine cube mesh missing."));
		return;
	}
	BuildWing();
	AddLights();
	UE_LOG(LogTBW, Log, TEXT("East Ceremonial Wing greybox spawned (%d actors)."), Spawned.Num());
}

void ATBWEastWingSandbox::ClearSpawned()
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

UMaterialInstanceDynamic* ATBWEastWingSandbox::MakeColor(const FLinearColor& Color)
{
	UMaterial* Base = UMaterial::GetDefaultMaterial(MD_Surface);
	if (!Base)
	{
		return nullptr;
	}
	UMaterialInstanceDynamic* Mid = UMaterialInstanceDynamic::Create(Base, this);
	// Default surface may ignore this; still set for materials that expose it.
	Mid->SetVectorParameterValue(TEXT("BaseColor"), Color);
	Mid->SetVectorParameterValue(TEXT("Color"), Color);
	return Mid;
}

AActor* ATBWEastWingSandbox::AddBox(const FVector& Location, const FVector& ScaleMeters, const FLinearColor& Color, bool bCollision)
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
	if (UMaterialInstanceDynamic* Mid = MakeColor(Color))
	{
		MeshComp->SetMaterial(0, Mid);
	}
	MeshComp->SetCollisionEnabled(bCollision ? ECollisionEnabled::QueryAndPhysics : ECollisionEnabled::NoCollision);
	if (bCollision)
	{
		MeshComp->SetCollisionProfileName(UCollisionProfile::BlockAll_ProfileName);
	}

	Spawned.Add(Box);
	return Box;
}

void ATBWEastWingSandbox::AddLabel(const FVector& Location, const FString& Text, const FColor& Color)
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	FActorSpawnParameters Params;
	Params.Owner = this;
	ATextRenderActor* Label = World->SpawnActor<ATextRenderActor>(Location, FRotator(0.f, 180.f, 0.f), Params);
	if (!Label)
	{
		return;
	}

	if (UTextRenderComponent* TextComp = Label->GetTextRender())
	{
		TextComp->SetText(FText::FromString(Text));
		TextComp->SetTextRenderColor(Color);
		TextComp->SetWorldSize(28.f);
		TextComp->SetHorizontalAlignment(EHTA_Center);
	}
	Spawned.Add(Label);
}

void ATBWEastWingSandbox::AddRoom(
	const FString& Name,
	const FVector& Center,
	const FVector2D& SizeMeters,
	float HeightMeters,
	const FLinearColor& Color,
	bool bDoorN, bool bDoorS, bool bDoorE, bool bDoorW)
{
	const float WallT = 0.25f;
	const float DoorW = 1.8f;
	const float DoorH = 2.8f;
	const float SX = SizeMeters.X;
	const float SY = SizeMeters.Y;
	const float HalfX = SX * 0.5f;
	const float HalfY = SY * 0.5f;
	const float WallZ = HeightMeters * 0.5f * 100.f;
	const FVector C = Center;

	// Floor + ceiling
	AddBox(C + FVector(0.f, 0.f, -10.f), FVector(SX, SY, 0.2f), TBWSandbox::HallFloor);
	AddBox(C + FVector(0.f, 0.f, HeightMeters * 100.f + 10.f), FVector(SX, SY, 0.2f), TBWSandbox::MudDark);

	auto WallWithDoor = [&](const FVector& Loc, const FVector& Scale, bool bDoor, bool bAlongX)
	{
		if (!bDoor)
		{
			AddBox(Loc, Scale, Color);
			return;
		}
		// Split wall around a centered door, plus a lintel.
		if (bAlongX)
		{
			const float Total = Scale.X;
			const float Side = FMath::Max(0.4f, (Total - DoorW) * 0.5f);
			AddBox(Loc + FVector(-(DoorW * 0.5f + Side * 0.5f) * 100.f, 0.f, 0.f), FVector(Side, Scale.Y, Scale.Z), Color);
			AddBox(Loc + FVector((DoorW * 0.5f + Side * 0.5f) * 100.f, 0.f, 0.f), FVector(Side, Scale.Y, Scale.Z), Color);
			const float LintelH = FMath::Max(0.3f, Scale.Z - DoorH);
			AddBox(Loc + FVector(0.f, 0.f, (DoorH * 0.5f) * 100.f), FVector(DoorW, Scale.Y, LintelH), Color);
		}
		else
		{
			const float Total = Scale.Y;
			const float Side = FMath::Max(0.4f, (Total - DoorW) * 0.5f);
			AddBox(Loc + FVector(0.f, -(DoorW * 0.5f + Side * 0.5f) * 100.f, 0.f), FVector(Scale.X, Side, Scale.Z), Color);
			AddBox(Loc + FVector(0.f, (DoorW * 0.5f + Side * 0.5f) * 100.f, 0.f), FVector(Scale.X, Side, Scale.Z), Color);
			const float LintelH = FMath::Max(0.3f, Scale.Z - DoorH);
			AddBox(Loc + FVector(0.f, 0.f, (DoorH * 0.5f) * 100.f), FVector(Scale.X, DoorW, LintelH), Color);
		}
	};

	const FVector North(C.X, C.Y + HalfY * 100.f, C.Z + WallZ);
	const FVector South(C.X, C.Y - HalfY * 100.f, C.Z + WallZ);
	const FVector East(C.X + HalfX * 100.f, C.Y, C.Z + WallZ);
	const FVector West(C.X - HalfX * 100.f, C.Y, C.Z + WallZ);

	WallWithDoor(North, FVector(SX, WallT, HeightMeters), bDoorN, true);
	WallWithDoor(South, FVector(SX, WallT, HeightMeters), bDoorS, true);
	WallWithDoor(East, FVector(WallT, SY, HeightMeters), bDoorE, false);
	WallWithDoor(West, FVector(WallT, SY, HeightMeters), bDoorW, false);

	AddLabel(C + FVector(0.f, 0.f, HeightMeters * 100.f - 40.f), Name);
}

ATBWInteractableActor* ATBWEastWingSandbox::AddPickup(
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
	if (UMaterialInstanceDynamic* Mid = MakeColor(Color))
	{
		Item->MeshComponent->SetMaterial(0, Mid);
	}
	Spawned.Add(Item);
	return Item;
}

void ATBWEastWingSandbox::BuildWing()
{
	// Coordinates in cm. +X east, +Y north, +Z up. Hall at origin.
	// R2 Audience hall
	AddRoom(TEXT("AUDIENCE HALL"), FVector(0.f, 0.f, 0.f), FVector2D(16.f, 18.f), 5.5f, TBWSandbox::Glaze, true, true, false, true);
	AddBox(FVector(0.f, 180.f, 20.f), FVector(1.4f, 1.4f, 0.4f), TBWSandbox::Accent); // empty chair plinth

	AddPickup(
		FVector(0.f, 180.f, 55.f),
		FVector(0.45f, 0.45f, 0.7f),
		TBWSandbox::Accent,
		NSLOCTEXT("TBW", "PromptChair", "Examine empty chair"),
		NSLOCTEXT("TBW", "ExamChair", "Still warm cloth. No one has sat. They laid the celebration anyway."),
		TEXT("EmptyChairExamined"));

	// R3 Terrace (north)
	AddRoom(TEXT("TERRACE"), FVector(0.f, 1300.f, 0.f), FVector2D(10.f, 8.f), 3.2f, TBWSandbox::Mud, false, true, false, false);

	// R12 West arch — blocked
	AddRoom(TEXT("WEST ARCH (CLOSED)"), FVector(-1300.f, 0.f, 0.f), FVector2D(8.f, 8.f), 4.0f, TBWSandbox::Soft, false, false, true, false);
	AddPickup(
		FVector(-1000.f, 0.f, 80.f),
		FVector(0.3f, 1.6f, 2.6f),
		TBWSandbox::Soft,
		NSLOCTEXT("TBW", "PromptWest", "The rest of the house"),
		NSLOCTEXT("TBW", "ExamWest", "The west wing is closed. The slice lives in this house only."),
		TEXT("WestArchTouched"));

	// R4 Family corridor (south of hall)
	AddRoom(TEXT("FAMILY CORRIDOR"), FVector(0.f, -1400.f, 0.f), FVector2D(5.f, 10.f), 3.6f, TBWSandbox::Mud, true, true, true, true);

	// R5 Evan
	AddRoom(TEXT("EVAN"), FVector(-650.f, -1400.f, 0.f), FVector2D(7.f, 6.f), 3.4f, TBWSandbox::Mud, false, false, true, false);
	AddBox(FVector(-800.f, -1500.f, 30.f), FVector(1.8f, 1.0f, 0.4f), TBWSandbox::MudDark); // bed
	EvanSpawn = FTransform(FRotator(0.f, 0.f, 0.f), FVector(-550.f, -1400.f, 110.f));

	// R6 Raynor
	AddRoom(TEXT("RAYNOR"), FVector(650.f, -1400.f, 0.f), FVector2D(7.f, 6.f), 3.4f, TBWSandbox::Mud, false, false, false, true);
	AddPickup(
		FVector(700.f, -1550.f, 40.f),
		FVector(1.8f, 1.0f, 0.35f),
		TBWSandbox::MudDark,
		NSLOCTEXT("TBW", "PromptBed", "Examine unused bed"),
		NSLOCTEXT("TBW", "ExamBed", "The sheet is tight. He did not lie down."),
		TEXT("UnusedBed"));
	AddPickup(
		FVector(520.f, -1280.f, 55.f),
		FVector(0.18f, 0.18f, 0.28f),
		FLinearColor(0.7f, 0.55f, 0.15f),
		NSLOCTEXT("TBW", "PromptOil", "Examine scar-oil"),
		NSLOCTEXT("TBW", "ExamOil", "The stopper is loose. The oil is full. He did not pack this."),
		TEXT("ScarOil"));

	// R7 Study
	AddRoom(TEXT("STUDY"), FVector(0.f, -2200.f, 0.f), FVector2D(8.f, 6.f), 3.4f, TBWSandbox::MudDark, true, true, false, false);
	AddPickup(
		FVector(80.f, -2300.f, 70.f),
		FVector(0.35f, 0.25f, 0.05f),
		FLinearColor(0.55f, 0.42f, 0.22f),
		NSLOCTEXT("TBW", "PromptLetter", "Examine the letter"),
		NSLOCTEXT("TBW", "ExamLetter", "Raynor's words in Malik's patience. The seal pressure is wrong."),
		TEXT("LetterIsFalse"));

	// R8 Kitchen yard edge
	AddRoom(TEXT("KITCHEN YARD"), FVector(200.f, -2900.f, 0.f), FVector2D(12.f, 8.f), 3.0f, TBWSandbox::Mud, true, true, false, true);

	// R1 Canal gate (west of yard)
	AddRoom(TEXT("CANAL GATE"), FVector(-1100.f, -2900.f, 0.f), FVector2D(10.f, 8.f), 3.0f, TBWSandbox::Mud, false, false, true, false);
	AddBox(FVector(-1300.f, -3100.f, -20.f), FVector(8.f, 3.f, 0.15f), TBWSandbox::Water, false);
	AddPickup(
		FVector(-1200.f, -3050.f, 20.f),
		FVector(0.16f, 0.22f, 0.06f),
		FLinearColor(0.45f, 0.28f, 0.08f),
		NSLOCTEXT("TBW", "PromptClasp", "Recover the clasp"),
		NSLOCTEXT("TBW", "ExamClasp", "The lion-hook from his shoulder. Torn, not unpinned."),
		TEXT("ClaspFound"));

	// R9 Storage court
	AddRoom(TEXT("STORAGE COURT"), FVector(200.f, -3700.f, 0.f), FVector2D(12.f, 8.f), 3.2f, TBWSandbox::Mud, true, true, false, false);
	AddBox(FVector(-200.f, -3600.f, 50.f), FVector(1.2f, 0.8f, 1.0f), TBWSandbox::MudDark);
	AddBox(FVector(500.f, -3800.f, 40.f), FVector(0.9f, 0.9f, 0.8f), TBWSandbox::MudDark);

	// R10 Barracks annex
	AddRoom(TEXT("BARRACKS ANNEX"), FVector(200.f, -4600.f, 0.f), FVector2D(10.f, 10.f), 3.4f, TBWSandbox::Annex, true, false, false, false);
	AddPickup(
		FVector(350.f, -4800.f, 80.f),
		FVector(0.5f, 0.08f, 0.7f),
		FLinearColor(0.4f, 0.32f, 0.18f),
		NSLOCTEXT("TBW", "PromptRoster", "Read the night roster"),
		NSLOCTEXT("TBW", "ExamRoster", "Canal posts struck through. Darius's mark. The water had no house eyes."),
		TEXT("RosterAltered"));

	// R11 Upper chamber marker (not a second floor — a raised plinth north-east of hall)
	AddRoom(TEXT("UPPER CHAMBER (CINE)"), FVector(1100.f, 900.f, 0.f), FVector2D(7.f, 7.f), 3.6f, TBWSandbox::MudDark, false, false, false, true);
	AddBox(FVector(1100.f, 900.f, 35.f), FVector(2.0f, 1.2f, 0.45f), TBWSandbox::Accent);

	AddLabel(FVector(0.f, 0.f, 420.f), TEXT("EAST CEREMONIAL WING  /  PHASE 1 GREYBOX"), FColor(180, 200, 230));
}

void ATBWEastWingSandbox::AddLights()
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	FActorSpawnParameters Params;
	Params.Owner = this;

	if (ADirectionalLight* Sun = World->SpawnActor<ADirectionalLight>(FVector(0.f, 0.f, 800.f), FRotator(-42.f, 35.f, 0.f), Params))
	{
		if (UDirectionalLightComponent* Comp = Cast<UDirectionalLightComponent>(Sun->GetLightComponent()))
		{
			Comp->SetIntensity(8.f);
			Comp->SetLightColor(FLinearColor(1.f, 0.93f, 0.82f));
			Comp->SetCastShadows(true);
		}
		Spawned.Add(Sun);
	}

	if (ASkyLight* Sky = World->SpawnActor<ASkyLight>(FVector(0.f, 0.f, 600.f), FRotator::ZeroRotator, Params))
	{
		if (USkyLightComponent* Comp = Sky->GetLightComponent())
		{
			Comp->SetIntensity(1.0f);
			Comp->bRealTimeCapture = true;
		}
		Spawned.Add(Sky);
	}

	if (AExponentialHeightFog* Fog = World->SpawnActor<AExponentialHeightFog>(FVector(0.f, 0.f, 0.f), FRotator::ZeroRotator, Params))
	{
		Spawned.Add(Fog);
	}
}
