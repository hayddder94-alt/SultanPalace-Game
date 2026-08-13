// The Betrayed Will / وصية الغدر
// Copyright (c) 2026. All rights reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class TheBetrayedWillEditorTarget : TargetRules
{
	public TheBetrayedWillEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V7;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_8;
		ExtraModuleNames.AddRange(new string[] { "TBW" });
	}
}
