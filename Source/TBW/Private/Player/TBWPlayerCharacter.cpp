// The Betrayed Will / وصية الغدر
// Copyright (c) 2026. All rights reserved.

#include "Player/TBWPlayerCharacter.h"
#include "Player/TBWPlayerIdentityComponent.h"
#include "Player/TBWIdentityFactory.h"
#include "Player/TBWIdentityData.h"
#include "Player/TBWPlayerController.h"
#include "Input/TBWInputConfig.h"
#include "Interaction/TBWInteractorComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/SkeletalMesh.h"
#include "Animation/AnimInstance.h"
#include "Engine/StaticMesh.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Materials/Material.h"
#include "EnhancedInputComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "TBW.h"

ATBWPlayerCharacter::ATBWPlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCapsuleComponent()->InitCapsuleSize(38.f, 88.f);

	UCharacterMovementComponent* MoveComp = GetCharacterMovement();
	MoveComp->bOrientRotationToMovement = true;
	MoveComp->RotationRate = FRotator(0.f, 520.f, 0.f);
	MoveComp->MaxWalkSpeed = 420.f;
	MoveComp->MaxWalkSpeedCrouched = 160.f;
	MoveComp->NavAgentProps.bCanCrouch = true;
	MoveComp->SetCrouchedHalfHeight(58.f);
	MoveComp->MaxAcceleration = 1800.f;
	MoveComp->BrakingDecelerationWalking = 2200.f;
	MoveComp->GroundFriction = 10.f;
	MoveComp->BrakingFrictionFactor = 1.15f;
	MoveComp->MaxStepHeight = 32.f;
	MoveComp->JumpZVelocity = 0.f;
	MoveComp->AirControl = 0.05f;
	MoveComp->bUseSeparateBrakingFriction = false;
	MoveComp->MinAnalogWalkSpeed = 20.f;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = DefaultBoomLength;
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->SocketOffset = FVector(0.f, 55.f, 62.f);
	CameraBoom->bEnableCameraLag = true;
	CameraBoom->CameraLagSpeed = 14.f;
	CameraBoom->bEnableCameraRotationLag = true;
	CameraBoom->CameraRotationLagSpeed = 22.f;
	CameraBoom->ProbeSize = 20.f;
	CameraBoom->bDoCollisionTest = true;
	CameraBoom->ProbeChannel = ECC_Camera;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
	FollowCamera->FieldOfView = 78.f;

	PreviewBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PreviewBody"));
	PreviewBody->SetupAttachment(GetCapsuleComponent());
	PreviewBody->SetRelativeLocation(FVector(0.f, 0.f, -88.f));
	PreviewBody->SetRelativeScale3D(FVector(0.38f, 0.38f, 1.72f));
	PreviewBody->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeFinder(TEXT("/Engine/BasicShapes/Cube.Cube"));
	if (CubeFinder.Succeeded())
	{
		PreviewBody->SetStaticMesh(CubeFinder.Object);
	}

	IdentityComponent = CreateDefaultSubobject<UTBWPlayerIdentityComponent>(TEXT("Identity"));
	Interactor = CreateDefaultSubobject<UTBWInteractorComponent>(TEXT("Interactor"));
}

void ATBWPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (IdentityComponent && !IdentityComponent->GetIdentity())
	{
		IdentityComponent->ApplyIdentity(FTBWIdentityFactory::MakeEvan(this));
	}
	ResolveCharacterVisual();
	ApplyMovementFromIdentity();
	ApplyPresentationFromIdentity();

	UE_LOG(LogTBW, Log, TEXT("Player ready. Phase 2 feel. Body: %s. No combat."),
		bUsingRealMesh ? TEXT("skeletal") : TEXT("placeholder cube"));
}

void ATBWPlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
}

void ATBWPlayerCharacter::ApplyMovementFromIdentity()
{
	const UTBWIdentityData* Data = IdentityComponent ? IdentityComponent->GetIdentity() : nullptr;
	WalkSpeed = Data ? Data->MaxWalkSpeed : 420.f;
	SprintSpeed = Data ? Data->MaxSprintSpeed : 620.f;
	CrouchSpeed = Data ? Data->MaxCrouchSpeed : 160.f;
	if (UCharacterMovementComponent* MoveComp = GetCharacterMovement())
	{
		MoveComp->MaxWalkSpeed = WalkSpeed;
		MoveComp->MaxWalkSpeedCrouched = CrouchSpeed;
	}
	ApplyPresentationFromIdentity();
}

void ATBWPlayerCharacter::ResolveCharacterVisual()
{
	USkeletalMeshComponent* MeshComp = GetMesh();
	if (!MeshComp)
	{
		return;
	}

	// Candidate skeletal meshes, best first. All are free: the Third Person
	// feature pack ships with the editor, our own imports land under TBW.
	static const TCHAR* MeshCandidates[] =
	{
		TEXT("/Game/TBW/Characters/Evan/SKM_Evan.SKM_Evan"),
		TEXT("/Game/Characters/Mannequins/Meshes/SKM_Manny.SKM_Manny"),
		TEXT("/Game/Characters/Mannequins/Meshes/SKM_Quinn.SKM_Quinn"),
		TEXT("/Game/Characters/Mannequin_UE4/Meshes/SK_Mannequin.SK_Mannequin"),
		TEXT("/Game/Characters/Mannequins/Meshes/SKM_Manny_Simple.SKM_Manny_Simple"),
		TEXT("/Game/Characters/Mannequins/Meshes/SKM_Quinn_Simple.SKM_Quinn_Simple")
	};

	USkeletalMesh* Chosen = nullptr;
	for (const TCHAR* Path : MeshCandidates)
	{
		Chosen = LoadObject<USkeletalMesh>(nullptr, Path, nullptr, LOAD_NoWarn | LOAD_Quiet);
		if (Chosen)
		{
			UE_LOG(LogTBW, Display, TEXT("Character mesh: %s"), Path);
			break;
		}
	}

	if (!Chosen)
	{
		UE_LOG(LogTBW, Warning,
			TEXT("No skeletal character found. Still using the placeholder cube. "
				 "Add the free Third Person feature pack: Content Browser > Add > "
				 "Add Feature or Content Pack > Blueprint Feature > Third Person."));
		return;
	}

	MeshComp->SetSkeletalMesh(Chosen);
	// Unreal characters author facing +X while the capsule faces +X too, but the
	// mesh pivot sits at the feet: drop by half height and yaw -90.
	MeshComp->SetRelativeLocationAndRotation(
		FVector(0.f, 0.f, -88.f), FRotator(0.f, -90.f, 0.f));
	MeshComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	MeshComp->SetCollisionProfileName(TEXT("CharacterMesh"));

	static const TCHAR* AnimCandidates[] =
	{
		TEXT("/Game/TBW/Characters/Evan/ABP_Evan.ABP_Evan_C"),
		TEXT("/Game/Characters/Mannequins/Animations/ABP_Manny.ABP_Manny_C"),
		TEXT("/Game/Characters/Mannequins/Animations/ABP_Quinn.ABP_Quinn_C")
	};

	for (const TCHAR* Path : AnimCandidates)
	{
		if (UClass* AnimClass = LoadObject<UClass>(nullptr, Path, nullptr, LOAD_NoWarn | LOAD_Quiet))
		{
			MeshComp->SetAnimationMode(EAnimationMode::AnimationBlueprint);
			MeshComp->SetAnimInstanceClass(AnimClass);
			UE_LOG(LogTBW, Display, TEXT("Character anim blueprint: %s"), Path);
			break;
		}
	}

	bUsingRealMesh = true;

	// The cube was only ever a stand-in for a body. Retire it, do not delete it:
	// tbw.Identity.Set still uses it when no skeletal mesh exists.
	if (PreviewBody)
	{
		PreviewBody->SetVisibility(false, true);
		PreviewBody->SetHiddenInGame(true, true);
	}
}

void ATBWPlayerCharacter::ApplyPresentationFromIdentity()
{
	if (!PreviewBody || bUsingRealMesh)
	{
		return;
	}

	const UTBWIdentityData* Data = IdentityComponent ? IdentityComponent->GetIdentity() : nullptr;
	const bool bRaynor = Data && Data->IdentityId == TEXT("Raynor");
	PreviewBody->SetRelativeScale3D(bRaynor ? FVector(0.46f, 0.42f, 1.82f) : FVector(0.38f, 0.38f, 1.72f));

	if (UMaterial* Base = UMaterial::GetDefaultMaterial(MD_Surface))
	{
		if (UMaterialInstanceDynamic* Mid = UMaterialInstanceDynamic::Create(Base, this))
		{
			const FLinearColor Color = bRaynor
				? FLinearColor(0.22f, 0.18f, 0.14f)
				: FLinearColor(0.35f, 0.38f, 0.42f);
			Mid->SetVectorParameterValue(TEXT("BaseColor"), Color);
			Mid->SetVectorParameterValue(TEXT("Color"), Color);
			PreviewBody->SetMaterial(0, Mid);
		}
	}
}

FString ATBWPlayerCharacter::GetMoveStateName() const
{
	switch (MoveState)
	{
	case ETBWMoveState::Walk: return TEXT("Walk");
	case ETBWMoveState::Sprint: return TEXT("Sprint");
	case ETBWMoveState::Crouch: return TEXT("Crouch");
	case ETBWMoveState::Airborne: return TEXT("Airborne");
	default: return TEXT("Idle");
	}
}

void ATBWPlayerCharacter::RefreshMoveState()
{
	const UCharacterMovementComponent* MoveComp = GetCharacterMovement();
	if (!MoveComp)
	{
		MoveState = ETBWMoveState::Idle;
		return;
	}

	if (!MoveComp->IsMovingOnGround())
	{
		MoveState = ETBWMoveState::Airborne;
		return;
	}
	if (bIsCrouched)
	{
		MoveState = ETBWMoveState::Crouch;
		return;
	}
	const bool bMoving = MoveComp->Velocity.Size2D() > 20.f;
	if (bWantsSprint && bMoving)
	{
		MoveState = ETBWMoveState::Sprint;
		return;
	}
	MoveState = bMoving ? ETBWMoveState::Walk : ETBWMoveState::Idle;
}

void ATBWPlayerCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	UCharacterMovementComponent* MoveComp = GetCharacterMovement();
	if (MoveComp && !bIsCrouched && MoveComp->IsMovingOnGround())
	{
		const bool bMoving = !MoveComp->Velocity.IsNearlyZero(10.f);
		MoveComp->MaxWalkSpeed = (bWantsSprint && bMoving) ? SprintSpeed : WalkSpeed;
	}

	RefreshMoveState();
	UpdateCamera(DeltaSeconds);
}

void ATBWPlayerCharacter::UpdateCamera(float DeltaSeconds)
{
	if (!CameraBoom)
	{
		return;
	}

	const bool bSprinting = (MoveState == ETBWMoveState::Sprint);
	const float TargetLength = bIsCrouched ? CrouchBoomLength : (bSprinting ? SprintBoomLength : DefaultBoomLength);
	const FVector TargetOffset = bIsCrouched ? FVector(0.f, 48.f, 38.f) : FVector(0.f, 55.f, 62.f);

	CameraBoom->TargetArmLength = FMath::FInterpTo(CameraBoom->TargetArmLength, TargetLength, DeltaSeconds, 7.f);
	CameraBoom->SocketOffset = FMath::VInterpTo(CameraBoom->SocketOffset, TargetOffset, DeltaSeconds, 7.f);
}

void ATBWPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* Enhanced = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (!Enhanced)
	{
		UE_LOG(LogTBW, Error, TEXT("Enhanced Input component missing."));
		return;
	}

	ATBWPlayerController* PC = Cast<ATBWPlayerController>(Controller);
	UTBWInputConfig* Input = PC ? PC->GetInputConfig() : nullptr;
	if (!Input)
	{
		UE_LOG(LogTBW, Warning, TEXT("InputConfig missing on controller. Movement will be dead."));
		return;
	}

	Enhanced->BindAction(Input->MoveAction, ETriggerEvent::Triggered, this, &ATBWPlayerCharacter::Move);
	Enhanced->BindAction(Input->LookAction, ETriggerEvent::Triggered, this, &ATBWPlayerCharacter::Look);
	Enhanced->BindAction(Input->SprintAction, ETriggerEvent::Started, this, &ATBWPlayerCharacter::SprintStarted);
	Enhanced->BindAction(Input->SprintAction, ETriggerEvent::Completed, this, &ATBWPlayerCharacter::SprintEnded);
	Enhanced->BindAction(Input->CrouchAction, ETriggerEvent::Started, this, &ATBWPlayerCharacter::CrouchStarted);
	Enhanced->BindAction(Input->CrouchAction, ETriggerEvent::Completed, this, &ATBWPlayerCharacter::CrouchEnded);
	Enhanced->BindAction(Input->InteractAction, ETriggerEvent::Started, this, &ATBWPlayerCharacter::InteractPressed);
	Enhanced->BindAction(Input->PrimaryAction, ETriggerEvent::Started, this, &ATBWPlayerCharacter::PrimaryPressed);
	Enhanced->BindAction(Input->SecondaryAction, ETriggerEvent::Started, this, &ATBWPlayerCharacter::SecondaryPressed);
	Enhanced->BindAction(Input->PauseAction, ETriggerEvent::Started, this, &ATBWPlayerCharacter::PausePressed);
}

void ATBWPlayerCharacter::Move(const FInputActionValue& Value)
{
	const FVector2D Axis = Value.Get<FVector2D>();
	if (Controller == nullptr)
	{
		return;
	}
	if (Axis.SizeSquared() < FMath::Square(MoveDeadzone))
	{
		return;
	}

	const FRotator Yaw(0.f, Controller->GetControlRotation().Yaw, 0.f);
	const FRotationMatrix Matrix(Yaw);
	AddMovementInput(Matrix.GetUnitAxis(EAxis::X), Axis.Y);
	AddMovementInput(Matrix.GetUnitAxis(EAxis::Y), Axis.X);
}

void ATBWPlayerCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D Axis = Value.Get<FVector2D>();
	AddControllerYawInput(Axis.X * LookYawScale);
	AddControllerPitchInput(Axis.Y * LookPitchScale);
}

void ATBWPlayerCharacter::SprintStarted(const FInputActionValue& Value)
{
	bWantsSprint = true;
}

void ATBWPlayerCharacter::SprintEnded(const FInputActionValue& Value)
{
	bWantsSprint = false;
}

void ATBWPlayerCharacter::CrouchStarted(const FInputActionValue& Value)
{
	if (bCrouchToggle)
	{
		if (bIsCrouched)
		{
			UnCrouch();
		}
		else
		{
			Crouch();
		}
	}
	else
	{
		Crouch();
	}
}

void ATBWPlayerCharacter::CrouchEnded(const FInputActionValue& Value)
{
	if (!bCrouchToggle)
	{
		UnCrouch();
	}
}

void ATBWPlayerCharacter::InteractPressed(const FInputActionValue& Value)
{
	if (Interactor)
	{
		Interactor->TryInteract();
	}
}

void ATBWPlayerCharacter::PrimaryPressed(const FInputActionValue& Value)
{
	UE_LOG(LogTBW, Verbose, TEXT("Primary action reserved for Phase 3 combat."));
}

void ATBWPlayerCharacter::SecondaryPressed(const FInputActionValue& Value)
{
	UE_LOG(LogTBW, Verbose, TEXT("Secondary action reserved for Phase 3 combat."));
}

void ATBWPlayerCharacter::PausePressed(const FInputActionValue& Value)
{
	if (ATBWPlayerController* PC = Cast<ATBWPlayerController>(Controller))
	{
		PC->TogglePauseMenu();
	}
}
