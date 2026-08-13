// The Betrayed Will / وصية الغدر
// Copyright (c) 2026. All rights reserved.

#include "Player/TBWIdentityFactory.h"
#include "Player/TBWIdentityData.h"

static UTBWIdentityData* MakeBase(UObject* Outer, const FName Id, const FText& Name)
{
	UTBWIdentityData* Data = NewObject<UTBWIdentityData>(Outer, Id);
	Data->IdentityId = Id;
	Data->DisplayName = Name;
	return Data;
}

UTBWIdentityData* FTBWIdentityFactory::MakeEvan(UObject* Outer)
{
	UTBWIdentityData* Data = MakeBase(Outer, TEXT("Evan"), NSLOCTEXT("TBW", "IdEvan", "Evan"));
	Data->MaxWalkSpeed = 420.f;
	Data->MaxCrouchSpeed = 160.f;
	Data->MaxSprintSpeed = 620.f;
	Data->NoiseMultiplier = 0.8f;
	Data->VisibilityMultiplier = 0.85f;
	Data->LightDamageMultiplier = 0.8f;
	Data->HeavyDamageMultiplier = 0.85f;
	Data->Poise = 0.8f;
	Data->MeleeReach = 130.f;
	Data->bCanChargeHeavy = false;
	return Data;
}

UTBWIdentityData* FTBWIdentityFactory::MakeRaynor(UObject* Outer)
{
	UTBWIdentityData* Data = MakeBase(Outer, TEXT("Raynor"), NSLOCTEXT("TBW", "IdRaynor", "Raynor"));
	Data->MaxWalkSpeed = 380.f;
	Data->MaxCrouchSpeed = 140.f;
	Data->MaxSprintSpeed = 560.f;
	Data->NoiseMultiplier = 1.15f;
	Data->VisibilityMultiplier = 1.2f;
	Data->LightDamageMultiplier = 1.0f;
	Data->HeavyDamageMultiplier = 1.25f;
	Data->Poise = 1.3f;
	Data->MeleeReach = 165.f;
	Data->bCanChargeHeavy = true;
	return Data;
}
