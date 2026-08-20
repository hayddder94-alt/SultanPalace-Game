// The Betrayed Will / وصية الغدر
// Copyright (c) 2026. All rights reserved.

#include "Core/TBWGameInstance.h"
#include "Characters/TBWAnimLibrary.h"
#include "TBW.h"

void UTBWGameInstance::Init()
{
	Super::Init();
	UE_LOG(LogTBW, Log, TEXT("TBWGameInstance::Init language=%s"), *UILanguage);
}

void UTBWGameInstance::Shutdown()
{
	// The clip lookup caches by skeleton for the lifetime of the process. In a
	// packaged game that is correct and free. In the editor it is a trap: you
	// re-import an animation, press Play, and get the old scan back with no
	// hint that anything is stale.
	FTBWAnimLibrary::Forget();
	Super::Shutdown();
}
