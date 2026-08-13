// The Betrayed Will / وصية الغدر
// Copyright (c) 2026. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TBWEastWingSandbox.generated.h"

class UStaticMesh;
class UMaterialInterface;
class ATBWInteractableActor;

/**
 * Runtime greybox of the East Ceremonial Wing.
 * Exists so Phase 1 is playable without an authored .umap.
 * Phase 9 replaces this with modular art in L_VS_Palace.
 */
UCLASS()
class TBW_API ATBWEastWingSandbox : public AActor
{
	GENERATED_BODY()

public:
	ATBWEastWingSandbox();

	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void BeginPlay() override;

	FTransform GetEvanSpawnTransform() const { return EvanSpawn; }

	UFUNCTION(BlueprintCallable, Category = "TBW|Sandbox")
	void Rebuild();

protected:
	UPROPERTY()
	TObjectPtr<UStaticMesh> CubeMesh;

	UPROPERTY()
	TArray<TObjectPtr<AActor>> Spawned;

	FTransform EvanSpawn;

private:
	void ClearSpawned();
	void BuildWing();
	void AddLights();

	AActor* AddBox(const FVector& Location, const FVector& ScaleMeters, const FLinearColor& Color, bool bCollision = true);
	void AddLabel(const FVector& Location, const FString& Text, const FColor& Color = FColor(220, 200, 160));
	void AddRoom(const FString& Name, const FVector& Center, const FVector2D& SizeMeters, float HeightMeters, const FLinearColor& Color, bool bDoorN, bool bDoorS, bool bDoorE, bool bDoorW);
	ATBWInteractableActor* AddPickup(const FVector& Location, const FVector& ScaleMeters, const FLinearColor& Color, const FText& Prompt, const FText& Examine, FName Flag);

	UMaterialInstanceDynamic* MakeColor(const FLinearColor& Color);
};
