// The Betrayed Will / وصية الغدر
// Copyright (c) 2026. All rights reserved.

#include "Interaction/TBWTestDoor.h"
#include "Core/TBWWorldStateSubsystem.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/CollisionProfile.h"
#include "TBW.h"

ATBWTestDoor::ATBWTestDoor()
{
	PrimaryActorTick.bCanEverTick = true;

	DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
	SetRootComponent(DoorMesh);
	DoorMesh->SetCollisionProfileName(UCollisionProfile::BlockAll_ProfileName);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeFinder(TEXT("/Engine/BasicShapes/Cube.Cube"));
	if (CubeFinder.Succeeded())
	{
		DoorMesh->SetStaticMesh(CubeFinder.Object);
	}
	DoorMesh->SetWorldScale3D(FVector(0.12f, 1.2f, 2.2f));
}

void ATBWTestDoor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	const float TargetYaw = bOpen ? OpenYaw : ClosedYaw;
	const FRotator Now = GetActorRotation();
	const FRotator Goal(Now.Pitch, TargetYaw, Now.Roll);
	SetActorRotation(FMath::RInterpTo(Now, Goal, DeltaSeconds, 8.f));
}

bool ATBWTestDoor::CanInteract_Implementation(APawn* InstigatorPawn) const
{
	return InstigatorPawn != nullptr;
}

FText ATBWTestDoor::GetPrompt_Implementation(APawn* InstigatorPawn) const
{
	return bOpen
		? NSLOCTEXT("TBW", "CloseTestDoor", "Close test door")
		: NSLOCTEXT("TBW", "OpenTestDoor", "Open test door");
}

void ATBWTestDoor::Interact_Implementation(APawn* InstigatorPawn)
{
	bOpen = !bOpen;
	if (UWorld* World = GetWorld())
	{
		if (UTBWWorldStateSubsystem* State = World->GetSubsystem<UTBWWorldStateSubsystem>())
		{
			State->SetFlag(TEXT("TestDoorOpen"), bOpen ? 1 : 0);
		}
	}
	UE_LOG(LogTBW, Display, TEXT("Test door %s"), bOpen ? TEXT("OPEN") : TEXT("CLOSED"));
}
