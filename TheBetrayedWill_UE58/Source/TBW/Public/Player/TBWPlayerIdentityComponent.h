// The Betrayed Will / وصية الغدر
// Copyright (c) 2026. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TBWPlayerIdentityComponent.generated.h"

class UTBWIdentityData;

/**
 * Swaps mesh, animation, and tunables on the shared player pawn.
 * Vertical Slice applies Evan only. Do not possess a second pawn.
 */
UCLASS(ClassGroup = (TBW), meta = (BlueprintSpawnableComponent))
class TBW_API UTBWPlayerIdentityComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UTBWPlayerIdentityComponent();

	UFUNCTION(BlueprintCallable, Category = "TBW|Identity")
	void ApplyIdentity(UTBWIdentityData* NewIdentity);

	UFUNCTION(BlueprintPure, Category = "TBW|Identity")
	UTBWIdentityData* GetIdentity() const { return CurrentIdentity; }

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TBW|Identity")
	TObjectPtr<UTBWIdentityData> DefaultIdentity;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, Category = "TBW|Identity")
	TObjectPtr<UTBWIdentityData> CurrentIdentity;
};
