// The Betrayed Will / وصية الغدر
// Copyright (c) 2026. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interaction/TBWInteractable.h"
#include "TBWTestDoor.generated.h"

class UStaticMeshComponent;

/**
 * Technical test door. Not a story door. Toggles open/closed on E.
 */
UCLASS()
class TBW_API ATBWTestDoor : public AActor, public ITBWInteractable
{
	GENERATED_BODY()

public:
	ATBWTestDoor();

	virtual void Tick(float DeltaSeconds) override;

	virtual bool CanInteract_Implementation(APawn* InstigatorPawn) const override;
	virtual FText GetPrompt_Implementation(APawn* InstigatorPawn) const override;
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

	UPROPERTY(VisibleAnywhere, Category = "TBW|Test")
	TObjectPtr<UStaticMeshComponent> DoorMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TBW|Test")
	bool bOpen = false;

private:
	float ClosedYaw = 0.f;
	float OpenYaw = -90.f;
};
