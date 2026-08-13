// The Betrayed Will / وصية الغدر
// Copyright (c) 2026. All rights reserved.

#pragma once

#include "CoreMinimal.h"

class UTBWIdentityData;

/** Runtime identity data so Phase 1 does not depend on content assets. */
struct TBW_API FTBWIdentityFactory
{
	static UTBWIdentityData* MakeEvan(UObject* Outer);
	static UTBWIdentityData* MakeRaynor(UObject* Outer);
};
