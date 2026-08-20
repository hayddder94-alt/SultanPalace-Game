// The Betrayed Will / وصية الغدر
// Copyright (c) 2026. All rights reserved.

#include "Characters/TBWAnimLibrary.h"
#include "Animation/AnimSequence.h"
#include "Animation/Skeleton.h"
#include "Components/SkeletalMeshComponent.h"
// GetSingleNodeInstance() is forward declared on the component; calling
// GetAnimationAsset() on the result needs the definition. Without this:
//   error C2027: use of undefined type 'UAnimSingleNodeInstance'
#include "Animation/AnimSingleNodeInstance.h"
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

		// The registry does not necessarily know about these paths yet. In a
		// commandlet or a fresh -game process the background scan may not have
		// reached /Game/Characters, and GetAssets then honestly returns nothing.
		// That is what "clips: idle=- walk=- run=-" was: not an empty disk, an
		// unasked question.
		TArray<FString> Roots;
		for (const TCHAR* Root : SearchRoots)
		{
			Roots.Add(FString(Root));
		}
		Registry.ScanPathsSynchronous(Roots, /*bForceRescan*/ false);
		Registry.WaitForCompletion();

		TArray<FAssetData> Assets;
		for (const TCHAR* Root : SearchRoots)
		{
			FARFilter Filter;
			Filter.PackagePaths.Add(FName(Root));
			Filter.bRecursivePaths = true;
			Filter.ClassPaths.Add(UAnimSequence::StaticClass()->GetClassPathName());
			Registry.GetAssets(Filter, Assets);
		}

		// Prefer clips authored for this exact skeleton - a clip from another rig
		// produces a folded, broken pose, which reads as a bug in our own code.
		// But prefer, not require. tools/ADD_CHARACTERS.cmd copies the mannequin
		// out of a plugin, and the copies can end up tagged with the plugin's
		// skeleton path while the mesh reports the copied one. Demanding an exact
		// match then discards every clip on disk and leaves a statue, which is a
		// strictly worse outcome than a possible retarget artefact.
		TArray<FAssetData> Matching;
		if (Skeleton)
		{
			const FString SkeletonPath = Skeleton->GetPathName();
			const FString SkeletonName = Skeleton->GetName();
			for (const FAssetData& Data : Assets)
			{
				const FString Tag = Data.GetTagValueRef<FString>(TEXT("Skeleton"));
				if (Tag.IsEmpty() || Tag.Contains(SkeletonPath) || Tag.Contains(SkeletonName))
				{
					Matching.Add(Data);
				}
			}
		}

		const int32 TotalFound = Assets.Num();
		const int32 OnSkeleton = Matching.Num();
		if (OnSkeleton == 0 && TotalFound > 0)
		{
			UE_LOG(LogTBW, Warning,
				TEXT("%d animation sequence(s) on disk but none tagged for skeleton '%s'. "
					 "Using them anyway - a retarget artefact is easier to see and fix "
					 "than a statue."),
				TotalFound, Skeleton ? *Skeleton->GetName() : TEXT("<none>"));
			Matching = Assets;
		}

		int32 Considered = 0;
		for (const FAssetData& Data : Matching)
		{
			const FString Name = Data.AssetName.ToString();
			++Considered;

			// Load it and check it can actually be played.
			//
			// A copied .uasset keeps ABSOLUTE references, so an animation lifted
			// out of a plugin still points at /MoverExamples/.../SK_Mannequin_Skeleton
			// and comes back with GetSkeleton() == nullptr:
			//     "Unable to retrieve target Skeleton for Animation Asset"
			// Name matching happily accepted those, the report said idle=MM_Idle,
			// and the character stood in its bind pose while every line read PASS.
			// A clip without a skeleton is not a clip.
			auto Take = [&Data]() -> UAnimSequence*
			{
				UAnimSequence* Seq = Cast<UAnimSequence>(Data.GetAsset());
				if (Seq && !Seq->GetSkeleton())
				{
					UE_LOG(LogTBW, Warning,
						TEXT("Rejecting %s: no target skeleton. It was almost certainly "
							 "copied out of a plugin, which breaks its references. Run "
							 "tools\\ADD_CHARACTERS.cmd -Remove and let the plugin mount."),
						*Data.GetObjectPathString());
					return nullptr;
				}
				return Seq;
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
			TEXT("Anim scan: %d found, %d used. idle=%s walk=%s run=%s"),
			TotalFound, Considered,
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
	// Name plus where it came from. "idle=MM_Idle" was true and useless: the
	// same name exists in a working plugin mount and in a broken copy, and the
	// difference between them is the whole bug.
	auto N = [](UAnimSequence* S)
	{
		if (!S) { return FString(TEXT("-")); }
		FString Root = S->GetPathName();
		int32 Slash = INDEX_NONE;
		Root.FindChar(TEXT('/'), Slash);
		Root = Root.Mid(1);
		Root.FindChar(TEXT('/'), Slash);
		Root = (Slash > 0) ? Root.Left(Slash) : Root;
		return FString::Printf(TEXT("%s@%s"), *S->GetName(), *Root);
	};
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
