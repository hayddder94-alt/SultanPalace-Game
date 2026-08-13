// The Betrayed Will / وصية الغدر
// Copyright (c) 2026. All rights reserved.

#include "Player/TBWPlayerIdentityComponent.h"
#include "Player/TBWIdentityData.h"
#include "Player/TBWPlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "TBW.h"

UTBWPlayerIdentityComponent::UTBWPlayerIdentityComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UTBWPlayerIdentityComponent::BeginPlay()
{
	Super::BeginPlay();
	if (DefaultIdentity)
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

	ATBWPlayerCharacter* Character = Cast<ATBWPlayerCharacter>(GetOwner());
	if (!Character)
	{
		return;
	}

	if (UCharacterMovementComponent* Move = Character->GetCharacterMovement())
	{
		Move->MaxWalkSpeed = NewIdentity->MaxWalkSpeed;
		Move->MaxWalkSpeedCrouched = NewIdentity->MaxCrouchSpeed;
	}

	UE_LOG(LogTBWIdentity, Log, TEXT("Identity applied: %s"), *NewIdentity->IdentityId.ToString());
}
