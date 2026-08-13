// The Betrayed Will / وصية الغدر
// Copyright (c) 2026. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TBWInteractorComponent.generated.h"

class UCameraComponent;

/**
 * Camera-forward trace. Phase 1 foundation only — no examine turntable.
 */
UCLASS(ClassGroup = (TBW), meta = (BlueprintSpawnableComponent))
class TBW_API UTBWInteractorComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UTBWInteractorComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "TBW|Interact")
	void TryInteract();

	UFUNCTION(BlueprintPure, Category = "TBW|Interact")
	FText GetCurrentPrompt() const { return CurrentPrompt; }

	UFUNCTION(BlueprintPure, Category = "TBW|Interact")
	AActor* GetFocusedActor() const { return FocusedActor.Get(); }

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TBW|Interact")
	float TraceDistance = 230.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TBW|Interact")
	float TraceRadius = 22.f;

protected:
	virtual void BeginPlay() override;

private:
	void RefreshFocus();

	UPROPERTY()
	TWeakObjectPtr<AActor> FocusedActor;

	FText CurrentPrompt;
};
