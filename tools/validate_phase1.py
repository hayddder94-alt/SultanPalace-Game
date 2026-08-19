#!/usr/bin/env python3
"""Phase 1 validation.

STRUCTURAL: can run without Unreal.
RUNTIME: Editor / UBT / packaged exe — reported as skipped when the engine is absent.
"""

from __future__ import annotations

import shutil
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


def skip(msg: str) -> None:
    print(f"  SKIP  {msg}")


def structural() -> None:
    print("=== STRUCTURAL VALIDATION ===")
    phase0 = subprocess.run([sys.executable, str(ROOT / "tools" / "validate_phase0.py")], check=False)
    if phase0.returncode != 0:
        fail("phase 0 validation failed")
    else:
        ok("phase 0 validation")

    required = [
        "Source/TBW/Public/Input/TBWInputConfig.h",
        "Source/TBW/Public/Interaction/TBWInteractable.h",
        "Source/TBW/Public/World/TBWDevSandbox.h",
        "Source/TBW/Private/World/TBWDevSandbox.cpp",
        "Source/TBW/Public/Core/TBWWorldFlags.h",
        "Source/TBW/Public/UI/TBWHUD.h",
        "Source/TBW/Private/Core/TBWConsoleCommands.cpp",
        "docs/PHASE1_EXECUTION.md",
        "docs/INPUT_MAP.md",
        "tools/package_win_dev.ps1",
        "VERSION",
    ]
    for rel in required:
        (ok if (ROOT / rel).is_file() else fail)(rel)

    # Same stale pin as validate_ue58 had: the literal phase-1 string. The
    # invariant is the -ue58 suffix, not the phase number.
    version = (ROOT / "VERSION").read_text(encoding="utf-8").strip()
    (ok if version.endswith("-ue58") else fail)(f"VERSION {version}")

    uproject = (ROOT / "TheBetrayedWill.uproject").read_text(encoding="utf-8")
    (ok if '"EngineAssociation": "5.8"' in uproject else fail)("engine lock 5.8")

    build_cs = (ROOT / "Source/TBW/TBW.Build.cs").read_text(encoding="utf-8")
    for banned in ('"GameplayAbilities"', '"OnlineSubsystemSteam"', '"Steamworks"'):
        (ok if banned not in build_cs else fail)(f"no {banned}")

    # Scope gate. Save and dialogue were on this list until 2026-08-18, when the
    # director authorised building the full game; both now exist deliberately and
    # have their own validators. What remains here is what is still NOT authorised:
    # combat, GAS, enemy AI, a quest graph, and any Steam integration. Removing a
    # name from this list must be a decision, never a side effect.
    joined = "\n".join(p.read_text(encoding="utf-8") for p in (ROOT / "Source").rglob("*.cpp"))
    for token in (
        "UGameplayAbility",
        "OnlineSubsystemSteam",
        "ATBWAIController",
        "UTBWQuestSubsystem",
        "UTBWCombatComponent",
    ):
        (ok if token not in joined else fail)(f"still gated: no {token}")

    flags = (ROOT / "Source/TBW/Public/Core/TBWWorldFlags.h").read_text(encoding="utf-8")
    for name in ("WillWasRead", "RaynorDisappeared", "EvanInvestigating", "ClueFound_01", "GuardAlerted"):
        (ok if name in flags else fail)(f"flag {name}")

    input_h = (ROOT / "Source/TBW/Public/Input/TBWInputConfig.h").read_text(encoding="utf-8")
    for action in ("MoveAction", "LookAction", "SprintAction", "CrouchAction", "InteractAction", "PrimaryAction", "SecondaryAction", "PauseAction"):
        (ok if action in input_h else fail)(action)

    gamemode = (ROOT / "Source/TBW/Private/Core/TBWGameMode.cpp").read_text(encoding="utf-8")
    (ok if "ATBWDevSandbox" in gamemode else fail)("GameMode uses DevSandbox")
    if "SpawnActor<ATBWEastWingSandbox>" in gamemode:
        fail("GameMode still auto-spawns East Wing (Phase 1 default must be L_Dev_Sandbox)")
    else:
        ok("GameMode does not auto-spawn East Wing")

    dev = (ROOT / "Source/TBW/Private/World/TBWDevSandbox.cpp").read_text(encoding="utf-8")
    (ok if "ClueFound_01" in dev and "WillWasRead" in dev else fail)("DevSandbox world-state tests")

    console = (ROOT / "Source/TBW/Private/Core/TBWConsoleCommands.cpp").read_text(encoding="utf-8")
    for cmd in (
        "tbw.Flags.Set",
        "tbw.Flags.Get",
        "tbw.Flags.Check",
        "tbw.Flags.Clear",
        "tbw.Flags.List",
        "tbw.Debug.Reset",
        "tbw.Debug.Hud",
        "tbw.Identity.Set",
        "tbw.Version",
    ):
        (ok if cmd in console else fail)(cmd)


def runtime() -> None:
    print()
    print("=== RUNTIME VALIDATION ===")
    if shutil.which("UnrealEditor") is None:
        skip("UnrealEditor not on PATH — no Editor launch")
        skip("C++ UBT compile")
        skip("PIE L_Dev_Sandbox")
        skip("player spawn / move / camera / sprint / interact")
        skip("Windows Development Build")
        skip("Windows Shipping Build")
        skip("performance baseline capture")
        return
    skip("UnrealEditor found but automated runtime suite is not implemented yet")


def main() -> int:
    print(f"Phase 1 validation @ {ROOT}")
    structural()
    runtime()
    print()
    print(f"{len(ERRORS)} structural failures")
    return 1 if ERRORS else 0


if __name__ == "__main__":
    sys.exit(main())
