// The Betrayed Will / وصية الغدر
// Copyright (c) 2026. All rights reserved.

#include "Core/TBWGameInstance.h"
#include "TBW.h"

void UTBWGameInstance::Init()
{
	Super::Init();
	UE_LOG(LogTBW, Log, TEXT("TBWGameInstance::Init language=%s"), *UILanguage);
}
