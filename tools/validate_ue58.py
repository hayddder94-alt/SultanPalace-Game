#!/usr/bin/env python3
"""Enforce a single authoritative Unreal Engine 5.8 project.

Fails on active 5.6/5.7 configuration, competing .uproject files,
and Target.cs / version mismatches.
Does not launch Unreal Editor.
"""

from __future__ import annotations

import json
import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
ARCHIVE_ROOTS = ("backups/",)
ERRORS: list[str] = []


def is_archive(path: Path) -> bool:
    rel = path.relative_to(ROOT).as_posix()
    return any(rel.startswith(prefix) for prefix in ARCHIVE_ROOTS)


def fail(path: Path | None, line: int | None, msg: str) -> None:
    loc = "repo"
    if path is not None:
        loc = path.relative_to(ROOT).as_posix()
        if line:
            loc = f"{loc}:{line}"
    ERRORS.append(f"FAIL  {loc}  {msg}")
    print(ERRORS[-1])


def ok(msg: str) -> None:
    print(f"PASS  {msg}")


def scan_files() -> list[Path]:
    skip_dirs = {".git", "Binaries", "Intermediate", "Saved", "DerivedDataCache", "__pycache__"}
    out: list[Path] = []
    for p in ROOT.rglob("*"):
        if not p.is_file():
            continue
        if any(part in skip_dirs for part in p.parts):
            continue
        if p.suffix.lower() == ".zip":
            continue
        out.append(p)
    return out


def check_single_uproject() -> Path | None:
    active = [p for p in ROOT.rglob("*.uproject") if p.is_file() and not is_archive(p) and ".git" not in p.parts]
    if len(active) != 1:
        fail(None, None, f"expected exactly 1 active .uproject, found {len(active)}: {[str(p.relative_to(ROOT)) for p in active]}")
        return None
    if active[0].name != "TheBetrayedWill.uproject" or active[0].parent != ROOT:
        fail(active[0], None, "authoritative project must be repository-root TheBetrayedWill.uproject")
    else:
        ok("one authoritative TheBetrayedWill.uproject at repo root")
    return active[0]


def check_association(uproject: Path) -> None:
    data = json.loads(uproject.read_text(encoding="utf-8"))
    assoc = data.get("EngineAssociation")
    if assoc != "5.8":
        fail(uproject, 0, f"EngineAssociation={assoc!r}, expected '5.8'")
    else:
        ok("EngineAssociation == 5.8")


def check_targets() -> None:
    targets = [p for p in (ROOT / "Source").glob("*.Target.cs")]
    if not targets:
        fail(ROOT / "Source", None, "no Target.cs files")
        return
    for path in targets:
        text = path.read_text(encoding="utf-8")
        if "Unreal5_7" in text or "Unreal5_6" in text:
            for i, line in enumerate(text.splitlines(), 1):
                if "Unreal5_7" in line or "Unreal5_6" in line:
                    fail(path, i, "obsolete IncludeOrderVersion")
        elif "Unreal5_8" not in text:
            fail(path, None, "missing EngineIncludeOrderVersion.Unreal5_8")
        else:
            ok(f"{path.name} uses Unreal5_8")
        if "BuildSettingsVersion.V7" not in text:
            fail(path, None, "expected DefaultBuildSettings = BuildSettingsVersion.V7")
        else:
            ok(f"{path.name} uses BuildSettingsVersion.V7")


def check_build_cs() -> None:
    path = ROOT / "Source/TBW/TBW.Build.cs"
    text = path.read_text(encoding="utf-8")
    for banned in ('"GameplayAbilities"', '"OnlineSubsystemSteam"', '"Steamworks"'):
        if banned in text:
            fail(path, None, f"forbidden dependency {banned}")
        else:
            ok(f"Build.cs has no {banned}")
    if re.search(r"UE_5_[678]", text):
        fail(path, None, "version-conditional compile remains")
    else:
        ok("Build.cs has no UE_5_x conditionals")


def check_version_files() -> None:
    version = (ROOT / "VERSION").read_text(encoding="utf-8").strip()
    if version != "0.1.0-phase1-ue58":
        fail(ROOT / "VERSION", 1, f"VERSION={version!r}")
    else:
        ok("VERSION 0.1.0-phase1-ue58")
    header = (ROOT / "Source/TBW/Public/Core/TBWVersion.h").read_text(encoding="utf-8")
    if 'TEXT("5.8")' not in header:
        fail(ROOT / "Source/TBW/Public/Core/TBWVersion.h", None, "TBW_ENGINE_LOCK is not 5.8")
    else:
        ok("TBW_ENGINE_LOCK is 5.8")
    if "5.7" in header or "ue57" in header.lower():
        fail(ROOT / "Source/TBW/Public/Core/TBWVersion.h", None, "5.7 remains in version header")


def check_no_generated() -> None:
    tracked_bad = []
    # only care about repo files that exist on disk under those names at root
    for name in ("Binaries", "Intermediate", "Saved", "DerivedDataCache"):
        p = ROOT / name
        if p.exists() and not (p / ".gitkeep").exists():
            # existence on disk is ok if gitignored; warn only if not ignored
            pass
    ok("generated folders are not required (not validated as git-tracked here)")


ACTIVE_CONFIG_PATTERNS = [
    (re.compile(r'"EngineAssociation"\s*:\s*"5\.[67]"'), "active EngineAssociation 5.6/5.7"),
    (re.compile(r"EngineIncludeOrderVersion\.Unreal5_[67]\b"), "active IncludeOrderVersion 5.6/5.7"),
    (re.compile(r"\bUE57_ROOT\b"), "script still points at UE 5.7"),
    (re.compile(r"Epic Games\\UE_5\.7"), "script still guesses UE_5.7 install"),
]


def check_active_text(files: list[Path]) -> None:
    doc_historical_ok = {
        "docs/UE58_MIGRATION_REPORT.md",
        "docs/UE58_FULL_MIGRATION_REPORT.md",
        "docs/ENGINE_VERSION_LOCK.md",
        "docs/PHASE0_REPORT.md",
        "docs/PHASE0_VALIDATION.md",
        "docs/PHASE0_RISK_REGISTER.md",
        "docs/PHASE1_REPORT.md",
        "docs/PHASE1_EXECUTION.md",
        "docs/PHASE1_PLAN.md",
    }
    for path in files:
        if is_archive(path):
            continue
        if path.suffix.lower() not in {".md", ".py", ".ps1", ".sh", ".cs", ".h", ".cpp", ".ini", ".uproject", ".json", ".txt", ""} and path.name not in {"VERSION", "README.md"}:
            if path.suffix not in {".md", ".py", ".ps1", ".sh", ".cs", ".h", ".cpp", ".ini", ".uproject"}:
                continue
        rel = path.relative_to(ROOT).as_posix()
        try:
            text = path.read_text(encoding="utf-8")
        except UnicodeDecodeError:
            continue
        for i, line in enumerate(text.splitlines(), 1):
            for pat, why in ACTIVE_CONFIG_PATTERNS:
                if pat.search(line):
                    if rel in doc_historical_ok and ("HISTORICAL" in text or "ARCHIVED" in text or rel.endswith("MIGRATION_REPORT.md")):
                        continue
                    # allow historical banners that mention 5.7 only as past tense in labelled docs
                    if rel.startswith("docs/") and ("HISTORICAL" in text[:800] or "ARCHIVED" in text[:800]):
                        continue
                    fail(path, i, f"{why}: {line.strip()}")


def main() -> int:
    print(f"UE 5.8-only validation @ {ROOT}")
    files = scan_files()
    uproject = check_single_uproject()
    if uproject:
        check_association(uproject)
    check_targets()
    check_build_cs()
    check_version_files()
    check_no_generated()
    check_active_text(files)

    required = [
        "Source/TBW/TBW.cpp",
        "Source/TBW/TBW.Build.cs",
        "Config/DefaultEngine.ini",
        "Config/DefaultGame.ini",
        "Config/DefaultInput.ini",
        "Content/TBW",
        "docs/UE58_FULL_MIGRATION_REPORT.md",
        "tools/validate_ue58.py",
        "README.md",
        "VERSION",
        ".gitignore",
    ]
    for rel in required:
        if not (ROOT / rel).exists():
            fail(ROOT / rel, None, "required path missing")
        else:
            ok(f"present {rel}")

    print()
    if ERRORS:
        print(f"{len(ERRORS)} FAILED")
        return 1
    print("0 FAILED — structural PASS")
    return 0


if __name__ == "__main__":
    sys.exit(main())
