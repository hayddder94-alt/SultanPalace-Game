// The Betrayed Will / وصية الغدر
// Copyright (c) 2026. All rights reserved.

#include "Narrative/TBWObjectiveSubsystem.h"
#include "Core/TBWWorldStateSubsystem.h"
#include "Engine/World.h"
#include "Misc/Paths.h"
#include "Misc/FileHelper.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"
#include "Dom/JsonObject.h"
#include "TBW.h"

void UTBWObjectiveSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	const int32 Count = ReloadRules();
	UE_LOG(LogTBW, Log, TEXT("Objective subsystem ready. %d rule(s)."), Count);
}

int32 UTBWObjectiveSubsystem::ReloadRules()
{
	Rules.Reset();

	const FString Path = FPaths::ProjectContentDir() / TEXT("TBW/Data/Objectives.json");
	FString Raw;
	if (!FFileHelper::LoadFileToString(Raw, *Path))
	{
		UE_LOG(LogTBW, Warning, TEXT("Objectives: cannot read %s"), *Path);
		return 0;
	}

	TSharedPtr<FJsonObject> Root;
	const TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Raw);
	if (!FJsonSerializer::Deserialize(Reader, Root) || !Root.IsValid())
	{
		UE_LOG(LogTBW, Error, TEXT("Objectives: malformed JSON in %s"), *Path);
		return 0;
	}

	const TArray<TSharedPtr<FJsonValue>>* Array = nullptr;
	if (!Root->TryGetArrayField(TEXT("objectives"), Array) || !Array)
	{
		UE_LOG(LogTBW, Error, TEXT("Objectives: no objectives array."));
		return 0;
	}

	for (const TSharedPtr<FJsonValue>& Value : *Array)
	{
		const TSharedPtr<FJsonObject>* Obj = nullptr;
		if (!Value->TryGetObject(Obj) || !Obj)
		{
			continue;
		}

		FTBWObjectiveRule Rule;
		FString Segment;
		if ((*Obj)->TryGetStringField(TEXT("segment"), Segment))
		{
			Rule.Segment = FName(*Segment);
		}
		(*Obj)->TryGetStringField(TEXT("en"), Rule.TextEN);
		(*Obj)->TryGetStringField(TEXT("ar"), Rule.TextAR);

		const TArray<TSharedPtr<FJsonValue>>* Requires = nullptr;
		if ((*Obj)->TryGetArrayField(TEXT("requires"), Requires) && Requires)
		{
			for (const TSharedPtr<FJsonValue>& F : *Requires)
			{
				Rule.RequiresFlags.Add(FName(*F->AsString()));
			}
		}

		const TArray<TSharedPtr<FJsonValue>>* Blocked = nullptr;
		if ((*Obj)->TryGetArrayField(TEXT("blocked_by"), Blocked) && Blocked)
		{
			for (const TSharedPtr<FJsonValue>& F : *Blocked)
			{
				Rule.BlockedByFlags.Add(FName(*F->AsString()));
			}
		}

		Rules.Add(Rule);
	}

	return Rules.Num();
}

const FTBWObjectiveRule* UTBWObjectiveSubsystem::FindActiveRule() const
{
	const UWorld* World = GetWorld();
	if (!World)
	{
		return nullptr;
	}

	UTBWWorldStateSubsystem* State =
		const_cast<UWorld*>(World)->GetSubsystem<UTBWWorldStateSubsystem>();
	if (!State)
	{
		return nullptr;
	}

	for (const FTBWObjectiveRule& Rule : Rules)
	{
		bool bOk = true;
		for (const FName& Flag : Rule.RequiresFlags)
		{
			if (!State->HasFlag(Flag))
			{
				bOk = false;
				break;
			}
		}
		if (!bOk)
		{
			continue;
		}
		for (const FName& Flag : Rule.BlockedByFlags)
		{
			if (State->HasFlag(Flag))
			{
				bOk = false;
				break;
			}
		}
		if (bOk)
		{
			return &Rule;
		}
	}
	return nullptr;
}

FString UTBWObjectiveSubsystem::GetCurrentObjective(bool bArabic) const
{
	const FTBWObjectiveRule* Rule = FindActiveRule();
	if (!Rule)
	{
		return FString();
	}
	return (bArabic && !Rule->TextAR.IsEmpty()) ? Rule->TextAR : Rule->TextEN;
}

FName UTBWObjectiveSubsystem::GetCurrentSegment() const
{
	const FTBWObjectiveRule* Rule = FindActiveRule();
	return Rule ? Rule->Segment : NAME_None;
}

float UTBWObjectiveSubsystem::GetProgress() const
{
	if (Rules.Num() == 0)
	{
		return 0.f;
	}
	const FTBWObjectiveRule* Rule = FindActiveRule();
	if (!Rule)
	{
		return 1.f;   // nothing left to do in the authored rules
	}
	const int32 Index = static_cast<int32>(Rule - Rules.GetData());
	return static_cast<float>(Index) / static_cast<float>(Rules.Num());
}
