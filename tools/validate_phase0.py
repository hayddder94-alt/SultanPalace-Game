#!/usr/bin/env python3
"""Structural validation for The Betrayed Will Phase 0. Does not require Unreal."""

from __future__ import annotations

import json
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
ERRORS: list[str] = []
WARNS: list[str] = []


def ok(msg: str) -> None:
    print(f"  PASS  {msg}")


def fail(msg: str) -> None:
    ERRORS.append(msg)
    print(f"  FAIL  {msg}")


def warn(msg: str) -> None:
    WARNS.append(msg)
    print(f"  WARN  {msg}")


def check_uproject() -> None:
    path = ROOT / "TheBetrayedWill.uproject"
    if not path.is_file():
        fail("missing TheBetrayedWill.uproject")
        return
    data = json.loads(path.read_text(encoding="utf-8"))
    if data.get("EngineAssociation") != "5.7":
        fail(f"EngineAssociation is {data.get('EngineAssociation')!r}, expected '5.7'")
    else:
        ok("EngineAssociation == 5.7")
    modules = [m.get("Name") for m in data.get("Modules", [])]
    if "TBW" not in modules:
        fail("TBW module not listed")
    else:
        ok("TBW module listed")
    plugins = [p.get("Name") for p in data.get("Plugins", []) if p.get("Enabled")]
    forbidden = {
        "GameplayAbilities",
        "OnlineSubsystemSteam",
        "Steamworks",
        "CommonUI",
        "StateTree",
        "GameplayStateTree",
        "Water",
        "ChaosVehicles",
        "PCG",
    }
    bad = forbidden.intersection(plugins)
    if bad:
        fail(f"forbidden plugins enabled: {sorted(bad)}")
    else:
        ok(f"plugin allowlist clean (enabled={plugins})")
    if "EnhancedInput" not in plugins:
        warn("EnhancedInput not explicitly enabled (may still be default)")
    else:
        ok("EnhancedInput enabled")


def check_required_files() -> None:
    required = [
        "Source/TheBetrayedWill.Target.cs",
        "Source/TheBetrayedWillEditor.Target.cs",
        "Source/TBW/TBW.Build.cs",
        "Source/TBW/TBW.h",
        "Source/TBW/TBW.cpp",
        "Source/TBW/Public/Core/TBWGameInstance.h",
        "Source/TBW/Public/Core/TBWGameMode.h",
        "Source/TBW/Public/Core/TBWWorldStateSubsystem.h",
        "Source/TBW/Public/Player/TBWPlayerCharacter.h",
        "Source/TBW/Public/Player/TBWPlayerController.h",
        "Source/TBW/Public/Player/TBWPlayerIdentityComponent.h",
        "Source/TBW/Public/Player/TBWIdentityData.h",
        "Config/DefaultEngine.ini",
        "Config/DefaultGame.ini",
        "Config/DefaultInput.ini",
        "docs/ENGINE_VERSION_LOCK.md",
        "docs/VERTICAL_SLICE_SCRIPT.md",
        "docs/VERTICAL_SLICE_ASSET_BUDGET.md",
        "docs/SCOPE_GATE.md",
        "docs/PALACE_WING_SPEC.md",
        "docs/CHARACTER_PIPELINE.md",
        "docs/LEILA_SPEC.md",
        "docs/PHASE1_PLAN.md",
    ]
    for rel in required:
        if (ROOT / rel).is_file():
            ok(rel)
        else:
            fail(f"missing {rel}")


def check_build_cs() -> None:
    text = (ROOT / "Source/TBW/TBW.Build.cs").read_text(encoding="utf-8")
    for banned in ("GameplayAbilities", "OnlineSubsystemSteam", "Steamworks"):
        # comments may mention them; fail only if added as a dependency string pair
        if f'"{banned}"' in text:
            fail(f"TBW.Build.cs depends on {banned}")
        else:
            ok(f"Build.cs does not depend on {banned}")


def check_no_campaign_maps() -> None:
    forbidden_names = {
        "L_Market_P",
        "L_Temple_P",
        "L_Kurnah_P",
        "L_Noble_P",
        "L_Residential_P",
    }
    content = ROOT / "Content"
    if not content.is_dir():
        fail("Content/ missing")
        return
    found = [p.stem for p in content.rglob("*") if p.stem in forbidden_names]
    if found:
        fail(f"campaign maps present: {found}")
    else:
        ok("no campaign district maps present")


def check_content_tree() -> None:
    required_dirs = [
        "Content/TBW/Characters/Evan",
        "Content/TBW/Combat",
        "Content/TBW/AI",
        "Content/TBW/Dialogue",
        "Content/TBW/Quests",
        "Content/TBW/Interaction",
        "Content/TBW/Save",
        "Content/TBW/UI",
        "Content/TBW/Audio",
        "Content/TBW/Cinematics",
        "Content/TBW/Environment/Palace/EastWing",
        "Content/TBW/Data",
        "Content/TBW/Maps/Dev",
        "Content/TBW/Maps/VS",
        "Content/TBW/Materials",
        "Content/TBW/Development",
    ]
    for rel in required_dirs:
        if (ROOT / rel).is_dir():
            ok(rel + "/")
        else:
            fail(f"missing dir {rel}")


def main() -> int:
    print(f"Phase 0 validation @ {ROOT}")
    check_uproject()
    check_required_files()
    check_build_cs()
    check_content_tree()
    check_no_campaign_maps()
    print()
    print(f"{len(ERRORS)} failed, {len(WARNS)} warnings")
    return 1 if ERRORS else 0


if __name__ == "__main__":
    sys.exit(main())
