// The Betrayed Will / وصية الغدر
// Copyright (c) 2026. All rights reserved.

#include "TBW.h"
#include "Core/TBWVersion.h"

DEFINE_LOG_CATEGORY(LogTBW);
DEFINE_LOG_CATEGORY(LogTBWSave);
DEFINE_LOG_CATEGORY(LogTBWQuest);
DEFINE_LOG_CATEGORY(LogTBWAI);
DEFINE_LOG_CATEGORY(LogTBWIdentity);

#define LOCTEXT_NAMESPACE "FTBWModule"

void FTBWModule::StartupModule()
{
	UE_LOG(LogTBW, Log, TEXT("TBW module started. Version %s. Engine lock: UE %s."), TBW_VERSION_STRING, TBW_ENGINE_LOCK);
}

void FTBWModule::ShutdownModule()
{
	UE_LOG(LogTBW, Log, TEXT("TBW module shutdown."));
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_PRIMARY_GAME_MODULE(FTBWModule, TBW, "TBW");
