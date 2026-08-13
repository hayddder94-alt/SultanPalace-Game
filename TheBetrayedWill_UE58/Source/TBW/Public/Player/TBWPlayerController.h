// The Betrayed Will / وصية الغدر
// Copyright (c) 2026. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TBWPlayerController.generated.h"

class UInputMappingContext;
class UTBWInputConfig;

UCLASS()
class TBW_API ATBWPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ATBWPlayerController();

	virtual void SetupInputComponent() override;
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintPure, Category = "TBW|Input")
	UTBWInputConfig* GetInputConfig() const { return InputConfig; }

	void TogglePauseMenu();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TBW|Input")
	TObjectPtr<UInputMappingContext> OverrideMappingContext;

private:
	void EnsureInputConfig();
	void PushMapping();

	UPROPERTY()
	TObjectPtr<UTBWInputConfig> InputConfig;

	bool bGameplayPaused = false;
};
