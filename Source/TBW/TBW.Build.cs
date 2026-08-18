// The Betrayed Will / وصية الغدر
// Copyright (c) 2026. All rights reserved.

using UnrealBuildTool;

public class TBW : ModuleRules
{
	public TBW(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicIncludePaths.AddRange(new string[]
		{
			"TBW/Public"
		});

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"UMG",
			"Slate",
			"SlateCore",
			"GameplayTags",
			"Json",
			"JsonUtilities",
			"NavigationSystem",
			"LevelSequence",
			"MovieScene",
			"CinematicCamera"
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
		});

		// Loose TTF used by the debug HUD Arabic title (Slate/HarfBuzz).
		RuntimeDependencies.Add("$(ProjectDir)/Content/TBW/UI/Fonts/DejaVuSans.ttf");

		// Intentionally NOT added in Phase 0 / Vertical Slice:
		//   GameplayAbilities, GameplayTasks (GAS)
		//   CommonUI
		//   OnlineSubsystem, OnlineSubsystemSteam, Steamworks
		//   StateTreeModule, GameplayStateTreeModule  — add in Phase 4
		//   Water, ChaosVehicles, Niagara (add only when an asset requires them)
		//   MetaHuman / HairStrands / Groom — add when the first hero is imported
	}
}
