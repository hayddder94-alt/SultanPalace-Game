// The Betrayed Will / وصية الغدر
// Copyright (c) 2026. All rights reserved.

#include "Characters/TBWAnimLibrary.h"
#include "Animation/AnimSequence.h"
#include "Animation/Skeleton.h"
#include "Components/SkeletalMeshComponent.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "AssetRegistry/IAssetRegistry.h"
#include "TBW.h"

namespace
{
	/**
	 * Where a mannequin can come from on this project. Three routes, because
	 * three different things put one here:
	 *   /Game/TBW/...        our own imports
	 *   /Game/Characters/... copied per machine by tools/ADD_CHARACTERS.cmd
	 *   /MoverExamples etc.  mounted only while that engine plugin is enabled
	 */
	static const TCHAR* SearchRoots[] =
	{
		TEXT("/Game/TBW/Characters"),
		TEXT("/Game/Characters"),
		TEXT("/MoverExamples"),
		TEXT("/MoverTests"),
		TEXT("/AnimToTexture")
	};

	bool Has(const FString& Name, const TCHAR* Needle)
	{
		return Name.Contains(Needle, ESearchCase::IgnoreCase);
	}

	/** Forward-facing clip, not a strafe or a turn-in-place. */
	bool IsForward(const FString& Name)
	{
		if (Has(Name, TEXT("_bwd")) || Has(Name, TEXT("_left")) || Has(Name, TEXT("_right"))
			|| Has(Name, TEXT("back")) || Has(Name, TEXT("strafe")) || Has(Name, TEXT("turn"))
			|| Has(Name, TEXT("pivot")) || Has(Name, TEXT("start")) || Has(Name, TEXT("stop")))
		{
			return false;
		}
		return true;
	}

	TMap<TWeakObjectPtr<const USkeleton>, FTBWLocomotionClips> Cache;

	void ScanInto(FTBWLocomotionClips& Out, const USkeleton* Skeleton)
	{
		const FAssetRegistryModule& Module =
			FModuleManager::LoadModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry"));
		IAssetRegistry& Registry = Module.Get();

		TArray<FAssetData> Assets;
		for (const TCHAR* Root : SearchRoots)
		{
			FARFilter Filter;
			Filter.PackagePaths.Add(FName(Root));
			Filter.bRecursivePaths = true;
			Filter.ClassPaths.Add(UAnimSequence::StaticClass()->GetClassPathName());
			Registry.GetAssets(Filter, Assets);
		}

		int32 Considered = 0;
		for (const FAssetData& Data : Assets)
		{
			// Only clips that belong to this skeleton. Playing a clip authored
			// for another rig produces a folded, broken pose - worse than a
			// T-pose because it looks like a bug in our own code.
			if (Skeleton)
			{
				const FString Tag = Data.GetTagValueRef<FString>(TEXT("Skeleton"));
				if (!Tag.IsEmpty() && !Tag.Contains(Skeleton->GetPathName()))
				{
					continue;
				}
			}

			const FString Name = Data.AssetName.ToString();
			++Considered;

			auto Take = [&Data]() -> UAnimSequence*
			{
				return Cast<UAnimSequence>(Data.GetAsset());
			};

			const bool bCrouch = Has(Name, TEXT("crouch"));
			const bool bIdle = Has(Name, TEXT("idle"));
			const bool bWalk = Has(Name, TEXT("walk"));
			const bool bRun = Has(Name, TEXT("run")) || Has(Name, TEXT("jog")) || Has(Name, TEXT("sprint"));

			if (bCrouch && bIdle && !Out.CrouchIdle) { Out.CrouchIdle = Take(); continue; }
			if (bCrouch && bWalk && IsForward(Name) && !Out.CrouchWalk) { Out.CrouchWalk = Take(); continue; }
			if (bCrouch) { continue; }

			if (bIdle && !Out.Idle) { Out.Idle = Take(); continue; }
			if (bWalk && IsForward(Name) && !Out.Walk) { Out.Walk = Take(); continue; }
			if (bRun && IsForward(Name) && !Out.Run) { Out.Run = Take(); continue; }
			if ((Has(Name, TEXT("sit")) || Has(Name, TEXT("seated"))) && !Out.Sit) { Out.Sit = Take(); continue; }
			if ((Has(Name, TEXT("lie")) || Has(Name, TEXT("lying")) || Has(Name, TEXT("death")))
				&& !Out.Lie) { Out.Lie = Take(); continue; }
		}

		// Better a walk cycle at the wrong speed than a statue.
		if (!Out.Walk && Out.Run) { Out.Walk = Out.Run; }
		if (!Out.Run && Out.Walk) { Out.Run = Out.Walk; }
		if (!Out.CrouchWalk) { Out.CrouchWalk = Out.Walk; }
		if (!Out.CrouchIdle) { Out.CrouchIdle = Out.Idle; }
		if (!Out.Sit) { Out.Sit = Out.Idle; }
		if (!Out.Lie) { Out.Lie = Out.Idle; }

		UE_LOG(LogTBW, Display,
			TEXT("Anim scan: %d sequence(s) on this skeleton. idle=%s walk=%s run=%s"),
			Considered,
			Out.Idle ? *Out.Idle->GetName() : TEXT("none"),
			Out.Walk ? *Out.Walk->GetName() : TEXT("none"),
			Out.Run ? *Out.Run->GetName() : TEXT("none"));

		if (!Out.HasAnything())
		{
			UE_LOG(LogTBW, Warning,
				TEXT("No animation sequences found for this skeleton. The character will "
					 "stand in its reference pose. Run tools\\FIND_CHARACTERS.cmd and send "
					 "the output."));
		}
	}
}

const FTBWLocomotionClips& FTBWAnimLibrary::For(const USkeleton* Skeleton)
{
	if (const FTBWLocomotionClips* Found = Cache.Find(Skeleton))
	{
		return *Found;
	}

	FTBWLocomotionClips Clips;
	ScanInto(Clips, Skeleton);
	return Cache.Add(Skeleton, MoveTemp(Clips));
}

FString FTBWAnimLibrary::Describe(const USkeleton* Skeleton)
{
	const FTBWLocomotionClips& C = For(Skeleton);
	auto N = [](UAnimSequence* S) { return S ? S->GetName() : FString(TEXT("-")); };
	return FString::Printf(TEXT("idle=%s walk=%s run=%s crouch=%s"),
		*N(C.Idle), *N(C.Walk), *N(C.Run), *N(C.CrouchWalk));
}

void FTBWAnimLibrary::PlayLooping(USkeletalMeshComponent* Mesh, UAnimSequence* Clip, float PlayRate)
{
	if (!Mesh || !Clip)
	{
		return;
	}

	// Restarting the clip every frame is the classic version of this mistake:
	// the character vibrates on frame zero and never reaches the second pose.
	const bool bSameClip = (Mesh->GetSingleNodeInstance() != nullptr)
		&& (Mesh->GetSingleNodeInstance()->GetAnimationAsset() == Clip);

	if (!bSameClip)
	{
		Mesh->SetAnimationMode(EAnimationMode::AnimationSingleNode);
		Mesh->PlayAnimation(Clip, true);
	}
	Mesh->SetPlayRate(PlayRate);
}

void FTBWAnimLibrary::Forget()
{
	Cache.Empty();
}
