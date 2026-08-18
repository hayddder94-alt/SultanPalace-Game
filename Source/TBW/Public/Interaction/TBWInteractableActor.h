// The Betrayed Will / وصية الغدر
// Copyright (c) 2026. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interaction/TBWInteractable.h"
#include "TBWInteractableActor.generated.h"

class UStaticMeshComponent;

/**
 * Greybox interactable. Sets a world flag and prints an examine line.
 * Replace with authored clue actors in Phase 5.
 */
UCLASS()
class TBW_API ATBWInteractableActor : public AActor, public ITBWInteractable
{
	GENERATED_BODY()

public:
	ATBWInteractableActor();

	virtual bool CanInteract_Implementation(APawn* InstigatorPawn) const override;
	virtual FText GetPrompt_Implementation(APawn* InstigatorPawn) const override;
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

	void Configure(const FText& InPrompt, const FText& InExamine, FName InFlag, int32 InFlagValue = 1);

	UPROPERTY(VisibleAnywhere, Category = "TBW|Interact")
	TObjectPtr<UStaticMeshComponent> MeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TBW|Interact")
	FText PromptText;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TBW|Interact")
	FText ExamineText;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TBW|Interact")
	FName SetsFlag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TBW|Interact")
	int32 FlagValue = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TBW|Interact")
	bool bOnce = true;

	/** Optional authored scene to play on interact, e.g. VS01_OrinLastWords. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TBW|Interact")
	FName PlaysDialogueScene;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TBW|Interact")
	bool bConsumed = false;
};
