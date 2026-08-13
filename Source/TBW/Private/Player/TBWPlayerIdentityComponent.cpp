// The Betrayed Will / وصية الغدر
// Copyright (c) 2026. All rights reserved.

#include "Player/TBWPlayerIdentityComponent.h"
#include "Player/TBWIdentityData.h"
#include "Player/TBWPlayerCharacter.h"
#include "TBW.h"

UTBWPlayerIdentityComponent::UTBWPlayerIdentityComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UTBWPlayerIdentityComponent::BeginPlay()
{
	Super::BeginPlay();
	if (DefaultIdentity && !CurrentIdentity)
	{
		ApplyIdentity(DefaultIdentity);
	}
}

void UTBWPlayerIdentityComponent::ApplyIdentity(UTBWIdentityData* NewIdentity)
{
	if (!NewIdentity)
	{
		UE_LOG(LogTBWIdentity, Warning, TEXT("ApplyIdentity called with null data."));
		return;
	}

	CurrentIdentity = NewIdentity;

	if (ATBWPlayerCharacter* Character = Cast<ATBWPlayerCharacter>(GetOwner()))
	{
		Character->ApplyMovementFromIdentity();
	}

	UE_LOG(LogTBWIdentity, Log, TEXT("Identity applied: %s"), *NewIdentity->IdentityId.ToString());
}
