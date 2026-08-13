// The Betrayed Will / وصية الغدر
// Copyright (c) 2026. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

DECLARE_LOG_CATEGORY_EXTERN(LogTBW, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogTBWSave, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogTBWQuest, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogTBWAI, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogTBWIdentity, Log, All);

class FTBWModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
