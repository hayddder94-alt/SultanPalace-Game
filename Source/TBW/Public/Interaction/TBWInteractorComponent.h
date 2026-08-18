// The Betrayed Will / وصية الغدر
// Copyright (c) 2026. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TBWInteractorComponent.generated.h"

/**
 * Camera-forward trace. Phase 2: more reliable focus, no combat.
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

	UFUNCTION(BlueprintPure, Category = "TBW|Interact")
	FString GetStatusLine() const;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TBW|Interact")
	float TraceDistance = 250.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TBW|Interact")
	float TraceRadius = 24.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TBW|Interact")
	float InteractCooldown = 0.18f;

	/** Seconds between focus sweeps. 0.05 = 20 Hz, cheap and visually instant. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TBW|Interact")
	float FocusRefreshInterval = 0.05f;

protected:
	virtual void BeginPlay() override;

private:
	void RefreshFocus();
	static AActor* ResolveInteractable(AActor* HitActor);

	UPROPERTY()
	TWeakObjectPtr<AActor> FocusedActor;

	FText CurrentPrompt;
	float LastInteractTime = -100.f;
	float TimeSinceFocusRefresh = 0.f;
};
