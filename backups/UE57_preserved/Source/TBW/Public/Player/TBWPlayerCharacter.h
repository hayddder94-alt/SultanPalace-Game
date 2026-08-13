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

/**
 * Single pawn for Evan and Raynor.
 * Phase 1: movement, camera, interact. No combat.
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

	void ApplyMovementFromIdentity();

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
	float LookYawScale = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TBW|Camera")
	float LookPitchScale = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TBW|Input")
	bool bCrouchToggle = true;

private:
	bool bWantsSprint = false;
	float WalkSpeed = 420.f;
	float SprintSpeed = 620.f;
	float CrouchSpeed = 160.f;
};
