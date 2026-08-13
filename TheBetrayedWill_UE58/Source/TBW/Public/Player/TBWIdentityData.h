// The Betrayed Will / وصية الغدر
// Copyright (c) 2026. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "TBWIdentityData.generated.h"

class USkeletalMesh;
class UAnimInstance;

/**
 * Data-only identity. Evan and Raynor share one pawn.
 * Differences live here, not in duplicated gameplay classes.
 * Vertical Slice ships DA_Identity_Evan only. Raynor data is authored
 * so the architecture is proven, but is not playable in the slice.
 */
UCLASS(BlueprintType)
class TBW_API UTBWIdentityData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TBW|Identity")
	FName IdentityId;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TBW|Identity")
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TBW|Presentation")
	TSoftObjectPtr<USkeletalMesh> Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TBW|Presentation")
	TSubclassOf<UAnimInstance> AnimClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TBW|Movement")
	float MaxWalkSpeed = 400.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TBW|Movement")
	float MaxCrouchSpeed = 150.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TBW|Movement")
	float MaxSprintSpeed = 600.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TBW|Stealth")
	float NoiseMultiplier = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TBW|Stealth")
	float VisibilityMultiplier = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TBW|Combat")
	float LightDamageMultiplier = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TBW|Combat")
	float HeavyDamageMultiplier = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TBW|Combat")
	float Poise = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TBW|Combat")
	bool bCanChargeHeavy = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TBW|Combat")
	float MeleeReach = 140.f;
};
