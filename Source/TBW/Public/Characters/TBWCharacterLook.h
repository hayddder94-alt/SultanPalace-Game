// The Betrayed Will / وصية الغدر
// Copyright (c) 2026. All rights reserved.

#pragma once

#include "CoreMinimal.h"

class UMeshComponent;

/**
 * Blockout costume.
 *
 * Seventeen identical grey mannequins standing in a palace is not a cast, it is
 * a warehouse. Faces and cloth are a MetaHuman-and-Megascans job that this
 * hardware cannot carry yet, but telling one brother from another does not
 * actually require either - it requires that they not be the same colour.
 *
 * So every staged character is flat-tinted from the blockout master material,
 * with a colour chosen per role in tools/layout/east_wing.py. Darius reads dark
 * and still, Leila reads pale and apart, the guards read leather. That is a
 * silhouette-and-value pass, which is what a real art director would ask for
 * before anyone models a face anyway.
 *
 * It is deliberately temporary. The day a character has its own material this
 * is skipped: Apply() does nothing when the master material is absent.
 */
class TBW_API FTBWCharacterLook
{
public:
	/** Tint every material slot on Mesh. No-op if the master is not built. */
	static void Apply(UMeshComponent* Mesh, const FLinearColor& Tint, float Roughness = 0.75f);

	/** Stable 0..1 from a name, so idles do not breathe in unison. */
	static float Jitter(FName Seed);
};
