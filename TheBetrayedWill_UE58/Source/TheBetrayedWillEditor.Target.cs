// The Betrayed Will / وصية الغدر
// Copyright (c) 2026. All rights reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class TheBetrayedWillEditorTarget : TargetRules
{
	public TheBetrayedWillEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		// Evidence: 5.7→5.8 C++ migration notes (Jun 2026) use V7 + Unreal5_8.
		DefaultBuildSettings = BuildSettingsVersion.V7;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_8;
		ExtraModuleNames.AddRange(new string[] { "TBW" });
	}
}
