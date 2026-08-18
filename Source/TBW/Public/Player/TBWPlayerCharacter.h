// The Betrayed Will / وصية الغدر
// Copyright (c) 2026. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TBWPlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UStaticMeshComponent;
class UTBWPlayerIdentityComponent;
class UTBWInteractorComponent;
class UInputAction;
struct FInputActionValue;

UENUM(BlueprintType)
enum class ETBWMoveState : uint8
{
	Idle,
	Walk,
	Sprint,
	Crouch,
	Airborne
};

/**
 * Single pawn for Evan and Raynor.
 * Phase 2: player feel. No combat.
 */
UCLASS()
class TBW_API ATBWPlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ATBWPlayerCharacter();

	virtual void Tick(float DeltaSeconds) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PossessedBy(AController* NewController) override;

	UFUNCTION(BlueprintPure, Category = "TBW|Identity")
	UTBWPlayerIdentityComponent* GetIdentityComponent() const { return IdentityComponent; }

	UFUNCTION(BlueprintPure, Category = "TBW|Interact")
	UTBWInteractorComponent* GetInteractor() const { return Interactor; }

	UFUNCTION(BlueprintPure, Category = "TBW|Move")
	ETBWMoveState GetMoveState() const { return MoveState; }

	UFUNCTION(BlueprintPure, Category = "TBW|Move")
	FString GetMoveStateName() const;

	void ApplyMovementFromIdentity();
	void ApplyPresentationFromIdentity();

	/**
	 * Swaps the placeholder cube for a real skeletal character when one is
	 * available in the project, and hides the cube. Free sources, in order:
	 *   1. /Game/Characters/Mannequins  (Content Browser > Add > Third Person feature pack)
	 *   2. /Game/TBW/Characters         (our own imported or MetaHuman-derived meshes)
	 * If nothing is found the cube stays, so the project always boots.
	 */
	void ResolveCharacterVisual();

	UFUNCTION(BlueprintPure, Category = "TBW|Presentation")
	bool IsUsingRealCharacterMesh() const { return bUsingRealMesh; }

protected:
	virtual void BeginPlay() override;

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void SprintStarted(const FInputActionValue& Value);
	void SprintEnded(const FInputActionValue& Value);
	void CrouchStarted(const FInputActionValue& Value);
	void CrouchEnded(const FInputActionValue& Value);
	void InteractPressed(const FInputActionValue& Value);
	void PrimaryPressed(const FInputActionValue& Value);
	void SecondaryPressed(const FInputActionValue& Value);
	void PausePressed(const FInputActionValue& Value);

	void UpdateCamera(float DeltaSeconds);
	void RefreshMoveState();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TBW|Camera")
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TBW|Camera")
	TObjectPtr<UCameraComponent> FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TBW|Preview")
	TObjectPtr<UStaticMeshComponent> PreviewBody;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TBW|Identity")
	TObjectPtr<UTBWPlayerIdentityComponent> IdentityComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TBW|Interact")
	TObjectPtr<UTBWInteractorComponent> Interactor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TBW|Camera")
	float DefaultBoomLength = 280.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TBW|Camera")
	float SprintBoomLength = 340.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TBW|Camera")
	float CrouchBoomLength = 230.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TBW|Camera")
	float LookYawScale = 0.85f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TBW|Camera")
	float LookPitchScale = 0.70f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TBW|Input")
	bool bCrouchToggle = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TBW|Move")
	float MoveDeadzone = 0.12f;

private:
	bool bWantsSprint = false;
	float WalkSpeed = 420.f;
	float SprintSpeed = 620.f;
	float CrouchSpeed = 160.f;
	ETBWMoveState MoveState = ETBWMoveState::Idle;
	bool bUsingRealMesh = false;
};
