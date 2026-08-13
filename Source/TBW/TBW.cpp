// The Betrayed Will / وصية الغدر
// Copyright (c) 2026. All rights reserved.

#include "TBW.h"

DEFINE_LOG_CATEGORY(LogTBW);
DEFINE_LOG_CATEGORY(LogTBWSave);
DEFINE_LOG_CATEGORY(LogTBWQuest);
DEFINE_LOG_CATEGORY(LogTBWAI);
DEFINE_LOG_CATEGORY(LogTBWIdentity);

#define LOCTEXT_NAMESPACE "FTBWModule"

void FTBWModule::StartupModule()
{
	UE_LOG(LogTBW, Log, TEXT("TBW module started (The Betrayed Will). Engine lock: UE 5.7."));
}

void FTBWModule::ShutdownModule()
{
	UE_LOG(LogTBW, Log, TEXT("TBW module shutdown."));
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_PRIMARY_GAME_MODULE(FTBWModule, TBW, "TBW");
