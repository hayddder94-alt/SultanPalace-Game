// The Betrayed Will / وصية الغدر
// Copyright (c) 2026. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "InputAction.h"
#include "TBWInputConfig.generated.h"

class UInputAction;
class UInputMappingContext;

/**
 * Runtime Enhanced Input. No .uasset required for Phase 1.
 * Editor assets (IA_TBW_*, IMC_TBW_Default) may replace this later
 * by assigning them on the player controller.
 */
UCLASS()
class TBW_API UTBWInputConfig : public UObject
{
	GENERATED_BODY()

public:
	void Build();

	UPROPERTY(BlueprintReadOnly, Category = "TBW|Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(BlueprintReadOnly, Category = "TBW|Input")
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(BlueprintReadOnly, Category = "TBW|Input")
	TObjectPtr<UInputAction> SprintAction;

	UPROPERTY(BlueprintReadOnly, Category = "TBW|Input")
	TObjectPtr<UInputAction> CrouchAction;

	UPROPERTY(BlueprintReadOnly, Category = "TBW|Input")
	TObjectPtr<UInputAction> InteractAction;

	UPROPERTY(BlueprintReadOnly, Category = "TBW|Input")
	TObjectPtr<UInputAction> PrimaryAction;

	UPROPERTY(BlueprintReadOnly, Category = "TBW|Input")
	TObjectPtr<UInputAction> SecondaryAction;

	UPROPERTY(BlueprintReadOnly, Category = "TBW|Input")
	TObjectPtr<UInputAction> PauseAction;

	UPROPERTY(BlueprintReadOnly, Category = "TBW|Input")
	TObjectPtr<UInputMappingContext> MappingContext;

private:
	UInputAction* CreateAction(const FName Name, EInputActionValueType ValueType);
};
