// The Betrayed Will / وصية الغدر
// Copyright (c) 2026. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TBWPlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UTBWPlayerIdentityComponent;
class UInputAction;
struct FInputActionValue;

/**
 * Single pawn for Evan and Raynor.
 * Phase 0: movement + look only. Combat, stealth, interact arrive in Phases 2–5.
 */
UCLASS()
class TBW_API ATBWPlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ATBWPlayerCharacter();

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintPure, Category = "TBW|Identity")
	UTBWPlayerIdentityComponent* GetIdentityComponent() const { return IdentityComponent; }

protected:
	virtual void BeginPlay() override;

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TBW|Camera")
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TBW|Camera")
	TObjectPtr<UCameraComponent> FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TBW|Identity")
	TObjectPtr<UTBWPlayerIdentityComponent> IdentityComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TBW|Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TBW|Input")
	TObjectPtr<UInputAction> LookAction;
};
