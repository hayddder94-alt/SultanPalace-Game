// The Betrayed Will / وصية الغدر
// Copyright (c) 2026. All rights reserved.

#pragma once

#include "CoreMinimal.h"

class UAnimSequence;
class USkeletalMeshComponent;
class USkeleton;

/**
 * Locomotion clips found on this machine, for one skeleton.
 *
 * WHY THIS EXISTS
 *
 * tools/ADD_CHARACTERS.cmd copies the mannequin out of the MoverExamples
 * plugin, and the pawn wore it: the self test said "body: skeletal mesh". It
 * then walked with its arms at its sides and its feet welded together.
 *
 * Two separate reasons, both real:
 *
 *  1. The anim blueprints that ship beside that mesh (ABP_Manny,
 *     ABP_MannyExtended) are authored for the Mover plugin's own movement
 *     component. Bound to an ACharacter with a UCharacterMovementComponent
 *     they get no state to read, so they output the reference pose.
 *
 *  2. Copying the assets to /Game/Characters left their skeleton reference
 *     pointing back into the plugin, so even the binding is not guaranteed.
 *
 * The honest fix at this stage is not to fake an anim blueprint from C++ - a
 * state machine belongs in an AnimGraph, and one authored blind would be worse
 * than none. It is to drive the skeletal mesh in SINGLE NODE mode from the
 * movement component we already own: pick a clip per locomotion state, loop
 * it, and scale the play rate so the feet roughly keep up with the ground.
 *
 * That is a blockout-grade solution and it is labelled as one. It gives moving
 * limbs today and is deleted in one commit the day a real AnimBP exists.
 */
struct FTBWLocomotionClips
{
	TObjectPtr<UAnimSequence> Idle = nullptr;
	TObjectPtr<UAnimSequence> Walk = nullptr;
	TObjectPtr<UAnimSequence> Run = nullptr;
	TObjectPtr<UAnimSequence> CrouchIdle = nullptr;
	TObjectPtr<UAnimSequence> CrouchWalk = nullptr;
	TObjectPtr<UAnimSequence> Sit = nullptr;
	TObjectPtr<UAnimSequence> Lie = nullptr;

	bool HasAnything() const { return Idle || Walk || Run; }
};

class TBW_API FTBWAnimLibrary
{
public:
	/** Scan once per skeleton, then cache. Safe to call every frame. */
	static const FTBWLocomotionClips& For(const USkeleton* Skeleton);

	/** What the last scan found, one line per slot. For the self test and log. */
	static FString Describe(const USkeleton* Skeleton);

	/**
	 * Put the component into single-node mode and loop Clip, but only if it is
	 * not already playing exactly that. Calling PlayAnimation every frame
	 * restarts the clip and the character twitches in place.
	 */
	static void PlayLooping(USkeletalMeshComponent* Mesh, UAnimSequence* Clip, float PlayRate);

	static void Forget();
};
