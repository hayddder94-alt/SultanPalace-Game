// The Betrayed Will / وصية الغدر
// Copyright (c) 2026. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TBWDevSandbox.generated.h"

class UStaticMesh;
class ATBWInteractableActor;

/**
 * L_Dev_Sandbox stand-in. Primitive floor, light, one interaction,
 * one world-state test. Not art. Not the palace.
 */
UCLASS()
class TBW_API ATBWDevSandbox : public AActor
{
	GENERATED_BODY()

public:
	ATBWDevSandbox();

	virtual void BeginPlay() override;

	FTransform GetPlayerSpawnTransform() const { return PlayerSpawn; }

	UFUNCTION(BlueprintCallable, Category = "TBW|Sandbox")
	void Rebuild();

private:
	void ClearSpawned();
	AActor* AddBox(const FVector& Location, const FVector& ScaleMeters, const FLinearColor& Color);
	void AddLabel(const FVector& Location, const FString& Text);
	ATBWInteractableActor* AddPickup(const FVector& Location, const FVector& ScaleMeters, const FLinearColor& Color, const FText& Prompt, const FText& Examine, FName Flag);
	void AddLights();

	UPROPERTY()
	TObjectPtr<UStaticMesh> CubeMesh;

	UPROPERTY()
	TArray<TObjectPtr<AActor>> Spawned;

	FTransform PlayerSpawn;
};
