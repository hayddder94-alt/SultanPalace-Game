// The Betrayed Will / وصية الغدر
// Copyright (c) 2026. All rights reserved.

#include "Input/TBWInputConfig.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "InputModifiers.h"
#include "InputCoreTypes.h"
#include "TBW.h"

UInputAction* UTBWInputConfig::CreateAction(const FName Name, EInputActionValueType ValueType)
{
	UInputAction* Action = NewObject<UInputAction>(this, Name);
	Action->ValueType = ValueType;
	return Action;
}

void UTBWInputConfig::Build()
{
	if (MappingContext)
	{
		return;
	}

	MoveAction = CreateAction(TEXT("IA_TBW_Move"), EInputActionValueType::Axis2D);
	LookAction = CreateAction(TEXT("IA_TBW_Look"), EInputActionValueType::Axis2D);
	SprintAction = CreateAction(TEXT("IA_TBW_Sprint"), EInputActionValueType::Boolean);
	CrouchAction = CreateAction(TEXT("IA_TBW_Crouch"), EInputActionValueType::Boolean);
	InteractAction = CreateAction(TEXT("IA_TBW_Interact"), EInputActionValueType::Boolean);
	PauseAction = CreateAction(TEXT("IA_TBW_Pause"), EInputActionValueType::Boolean);

	// Reserved for Phase 3 combat. They are CREATED here even though nothing uses
	// them yet: mapping a key to a null action makes Enhanced Input log
	// "A Key Mapping with a blank action has been added!" and silently drop the
	// binding, which the first headless self test surfaced four times.
	PrimaryAction = CreateAction(TEXT("IA_TBW_Primary"), EInputActionValueType::Boolean);
	SecondaryAction = CreateAction(TEXT("IA_TBW_Secondary"), EInputActionValueType::Boolean);

	MappingContext = NewObject<UInputMappingContext>(this, TEXT("IMC_TBW_Default"));

	auto AddSwizzleY = [this](FEnhancedActionKeyMapping& Mapping)
	{
		UInputModifierSwizzleAxis* Swizzle = NewObject<UInputModifierSwizzleAxis>(MappingContext);
		Swizzle->Order = EInputAxisSwizzle::YXZ;
		Mapping.Modifiers.Add(Swizzle);
	};

	auto AddNegate = [this](FEnhancedActionKeyMapping& Mapping)
	{
		UInputModifierNegate* Negate = NewObject<UInputModifierNegate>(MappingContext);
		Mapping.Modifiers.Add(Negate);
	};

	// WASD → Move Axis2D (X = right, Y = forward)
	{
		FEnhancedActionKeyMapping& W = MappingContext->MapKey(MoveAction, EKeys::W);
		AddSwizzleY(W);
	}
	{
		FEnhancedActionKeyMapping& S = MappingContext->MapKey(MoveAction, EKeys::S);
		AddSwizzleY(S);
		AddNegate(S);
	}
	{
		MappingContext->MapKey(MoveAction, EKeys::D);
	}
	{
		FEnhancedActionKeyMapping& A = MappingContext->MapKey(MoveAction, EKeys::A);
		AddNegate(A);
	}

	MappingContext->MapKey(MoveAction, EKeys::Gamepad_Left2D);
	MappingContext->MapKey(LookAction, EKeys::Mouse2D);
	MappingContext->MapKey(LookAction, EKeys::Gamepad_Right2D);

	MappingContext->MapKey(SprintAction, EKeys::LeftShift);
	MappingContext->MapKey(SprintAction, EKeys::Gamepad_LeftThumbstick);

	MappingContext->MapKey(CrouchAction, EKeys::LeftControl);
	MappingContext->MapKey(CrouchAction, EKeys::C);
	MappingContext->MapKey(CrouchAction, EKeys::Gamepad_FaceButton_Right);

	MappingContext->MapKey(InteractAction, EKeys::E);
	MappingContext->MapKey(InteractAction, EKeys::Gamepad_FaceButton_Bottom);

	MappingContext->MapKey(PrimaryAction, EKeys::LeftMouseButton);
	MappingContext->MapKey(PrimaryAction, EKeys::Gamepad_RightTrigger);

	MappingContext->MapKey(SecondaryAction, EKeys::RightMouseButton);
	MappingContext->MapKey(SecondaryAction, EKeys::Gamepad_LeftTrigger);

	MappingContext->MapKey(PauseAction, EKeys::Escape);
	MappingContext->MapKey(PauseAction, EKeys::P);
	MappingContext->MapKey(PauseAction, EKeys::Gamepad_Special_Right);

	UE_LOG(LogTBW, Log, TEXT("Runtime Enhanced Input built (WASD, mouse, gamepad)."));
}
