// The Betrayed Will / وصية الغدر
// Copyright (c) 2026. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "TBWInteractable.generated.h"

class APawn;

UINTERFACE(BlueprintType)
class TBW_API UTBWInteractable : public UInterface
{
	GENERATED_BODY()
};

class TBW_API ITBWInteractable
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "TBW|Interact")
	bool CanInteract(APawn* InstigatorPawn) const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "TBW|Interact")
	FText GetPrompt(APawn* InstigatorPawn) const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "TBW|Interact")
	void Interact(APawn* InstigatorPawn);
};
