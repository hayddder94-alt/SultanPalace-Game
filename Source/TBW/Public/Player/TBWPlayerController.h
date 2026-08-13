// The Betrayed Will / وصية الغدر
// Copyright (c) 2026. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TBWPlayerController.generated.h"

class UInputMappingContext;

/**
 * Input routing only in Phase 0. Dialogue / cinematic / UI contexts arrive
 * in later phases. Enhanced Input is the only input path.
 */
UCLASS()
class TBW_API ATBWPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ATBWPlayerController();

	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TBW|Input")
	TObjectPtr<UInputMappingContext> DefaultMappingContext;
};
