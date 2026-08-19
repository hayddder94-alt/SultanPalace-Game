// The Betrayed Will / وصية الغدر
// Copyright (c) 2026. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interaction/TBWInteractable.h"
#include "TBWStoryCharacter.generated.h"

class USkeletalMeshComponent;
class UCapsuleComponent;
class UStaticMeshComponent;
class UTextRenderComponent;

UENUM(BlueprintType)
enum class ETBWCharacterPose : uint8
{
	Standing,
	Seated,
	Lying
};

/**
 * A staged story character.
 *
 * Deliberately NOT a pawn and NOT AI: no controller, no behaviour tree, no
 * perception, no navigation. It stands, sits or lies where the script puts it,
 * faces a direction, and can carry a line. Enemy AI remains out of scope.
 *
 * The mesh resolves the same way the player's does - a real skeletal character if
 * the project has one, otherwise a proxy built from engine shapes so the cast is
 * visible and blockable from day one.
 */
UCLASS()
class TBW_API ATBWStoryCharacter : public AActor, public ITBWInteractable
{
	GENERATED_BODY()

public:
	ATBWStoryCharacter();

	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;

	virtual bool CanInteract_Implementation(APawn* InstigatorPawn) const override;
	virtual FText GetPrompt_Implementation(APawn* InstigatorPawn) const override;
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

	/** Display name. Shown on the floating label and in the interact prompt. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TBW|Character")
	FName CharacterName = TEXT("Servant");

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TBW|Character")
	ETBWCharacterPose Pose = ETBWCharacterPose::Standing;

	/** Scene played when the player interacts. Empty means silent. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TBW|Character")
	FName PlaysDialogueScene;

	/** Segment this staging belongs to, e.g. VS-02. Documentation at runtime. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TBW|Character")
	FName Segment;

	/** Debug label above the head. Stripped in Shipping. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TBW|Character")
	bool bShowNameLabel = true;

	UPROPERTY(VisibleAnywhere, Category = "TBW|Character")
	TObjectPtr<UCapsuleComponent> Capsule;

	UPROPERTY(VisibleAnywhere, Category = "TBW|Character")
	TObjectPtr<USkeletalMeshComponent> SkeletalBody;

	/** Proxy shown only when no skeletal mesh could be resolved. */
	UPROPERTY(VisibleAnywhere, Category = "TBW|Character")
	TObjectPtr<UStaticMeshComponent> ProxyBody;

	UPROPERTY(VisibleAnywhere, Category = "TBW|Character")
	TObjectPtr<UStaticMeshComponent> ProxyHead;

	UPROPERTY(VisibleAnywhere, Category = "TBW|Character")
	TObjectPtr<UTextRenderComponent> NameLabel;

	UFUNCTION(BlueprintPure, Category = "TBW|Character")
	bool IsUsingSkeletalMesh() const { return bUsingSkeletal; }

private:
	void ResolveBody();
	void ApplyPose();

	bool bUsingSkeletal = false;
};
