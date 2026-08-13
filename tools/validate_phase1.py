#!/usr/bin/env python3
"""Phase 1 structural validation. Does not require Unreal."""

from __future__ import annotations

import re
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
ERRORS: list[str] = []


def ok(msg: str) -> None:
    print(f"  PASS  {msg}")


def fail(msg: str) -> None:
    ERRORS.append(msg)
    print(f"  FAIL  {msg}")


def main() -> int:
    phase0 = subprocess.run([sys.executable, str(ROOT / "tools" / "validate_phase0.py")], check=False)
    if phase0.returncode != 0:
        fail("phase 0 validation failed")
    else:
        ok("phase 0 validation")

    required = [
        "Source/TBW/Public/Input/TBWInputConfig.h",
        "Source/TBW/Private/Input/TBWInputConfig.cpp",
        "Source/TBW/Public/Interaction/TBWInteractable.h",
        "Source/TBW/Public/Interaction/TBWInteractorComponent.h",
        "Source/TBW/Public/Interaction/TBWInteractableActor.h",
        "Source/TBW/Public/World/TBWEastWingSandbox.h",
        "Source/TBW/Private/World/TBWEastWingSandbox.cpp",
        "Source/TBW/Public/UI/TBWHUD.h",
        "Source/TBW/Private/Core/TBWConsoleCommands.cpp",
        "Source/TBW/Public/Core/TBWVersion.h",
        "tools/package_win_dev.ps1",
        "VERSION",
    ]
    for rel in required:
        (ok if (ROOT / rel).is_file() else fail)(rel)

    version = (ROOT / "VERSION").read_text(encoding="utf-8").strip()
    if version != "0.1.0-phase1":
        fail(f"VERSION is {version!r}")
    else:
        ok("VERSION 0.1.0-phase1")

    uproject = (ROOT / "TheBetrayedWill.uproject").read_text(encoding="utf-8")
    if '"EngineAssociation": "5.7"' not in uproject:
        fail("engine association drifted")
    else:
        ok("still locked to 5.7")

    build_cs = (ROOT / "Source/TBW/TBW.Build.cs").read_text(encoding="utf-8")
    for banned in ('"GameplayAbilities"', '"OnlineSubsystemSteam"', '"Steamworks"'):
        if banned in build_cs:
            fail(f"banned dep {banned}")
        else:
            ok(f"no {banned}")

    cpp_files = list((ROOT / "Source").rglob("*.cpp"))
    forbidden_tokens = [
        "UGameplayAbility",
        "OnlineSubsystemSteam",
        "ATBWAIController",
        "UTBWQuestSubsystem",
        "UTBWSaveGame",
        "UTBWDialogueSubsystem",
        "UTBWCombatComponent",
    ]
    joined = "\n".join(p.read_text(encoding="utf-8") for p in cpp_files)
    for token in forbidden_tokens:
        if token in joined:
            fail(f"campaign/system token present: {token}")
        else:
            ok(f"no {token}")

    sandbox = (ROOT / "Source/TBW/Private/World/TBWEastWingSandbox.cpp").read_text(encoding="utf-8")
    for room in ("AUDIENCE HALL", "FAMILY CORRIDOR", "RAYNOR", "EVAN", "STUDY", "CANAL GATE", "BARRACKS ANNEX", "STORAGE COURT"):
        if room not in sandbox:
            fail(f"sandbox missing room {room}")
        else:
            ok(f"sandbox has {room}")

    console = (ROOT / "Source/TBW/Private/Core/TBWConsoleCommands.cpp").read_text(encoding="utf-8")
    for cmd in ("tbw.Flags.Set", "tbw.Flags.List", "tbw.Flags.Get", "tbw.Identity.Set", "tbw.Version"):
        if cmd not in console:
            fail(f"missing console {cmd}")
        else:
            ok(cmd)

    print()
    print(f"{len(ERRORS)} failed")
    return 1 if ERRORS else 0


if __name__ == "__main__":
    sys.exit(main())
