// The Betrayed Will / وصية الغدر
// Copyright (c) 2026. All rights reserved.

#include "Core/TBWWorldFlags.h"

FName FTBWWorldFlags::ToName(ETBWWorldFlag Flag)
{
	switch (Flag)
	{
	case ETBWWorldFlag::WillWasRead: return TEXT("WillWasRead");
	case ETBWWorldFlag::RaynorDisappeared: return TEXT("RaynorDisappeared");
	case ETBWWorldFlag::EvanInvestigating: return TEXT("EvanInvestigating");
	case ETBWWorldFlag::ClueFound_01: return TEXT("ClueFound_01");
	case ETBWWorldFlag::GuardAlerted: return TEXT("GuardAlerted");
	case ETBWWorldFlag::EmptyChairExamined: return TEXT("EmptyChairExamined");
	case ETBWWorldFlag::UnusedBed: return TEXT("UnusedBed");
	case ETBWWorldFlag::ScarOil: return TEXT("ScarOil");
	case ETBWWorldFlag::ClaspFound: return TEXT("ClaspFound");
	case ETBWWorldFlag::RosterAltered: return TEXT("RosterAltered");
	case ETBWWorldFlag::LetterIsFalse: return TEXT("LetterIsFalse");
	case ETBWWorldFlag::WestArchTouched: return TEXT("WestArchTouched");
	case ETBWWorldFlag::TestDoorOpen: return TEXT("TestDoorOpen");
	default: return NAME_None;
	}
}

bool FTBWWorldFlags::TryParse(const FString& Text, ETBWWorldFlag& OutFlag)
{
	for (uint8 Index = 1; Index < static_cast<uint8>(ETBWWorldFlag::COUNT); ++Index)
	{
		const ETBWWorldFlag Candidate = static_cast<ETBWWorldFlag>(Index);
		if (ToName(Candidate).ToString().Equals(Text, ESearchCase::IgnoreCase))
		{
			OutFlag = Candidate;
			return true;
		}
	}
	return false;
}

void FTBWWorldFlags::GetKnownNames(TArray<FName>& OutNames)
{
	OutNames.Reset();
	for (uint8 Index = 1; Index < static_cast<uint8>(ETBWWorldFlag::COUNT); ++Index)
	{
		OutNames.Add(ToName(static_cast<ETBWWorldFlag>(Index)));
	}
}
