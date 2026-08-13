// The Betrayed Will / وصية الغدر
// Copyright (c) 2026. All rights reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class TheBetrayedWillTarget : TargetRules
{
	public TheBetrayedWillTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V5;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_7;
		ExtraModuleNames.AddRange(new string[] { "TBW" });
	}
}
