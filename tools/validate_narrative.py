#!/usr/bin/env python3
"""
Validate the narrative data the runtime will load.

Dialogue and objectives live in JSON so Arabic never passes through the C++
compiler. The cost of that choice is that a typo becomes a runtime surprise
instead of a build error - unless something checks the data. This is that thing.

    python3 tools/validate_narrative.py
"""

from __future__ import annotations

import json
import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
DIALOGUE_DIR = ROOT / "Content" / "TBW" / "Data" / "Dialogue"
OBJECTIVES = ROOT / "Content" / "TBW" / "Data" / "Objectives.json"
FLAGS_H = ROOT / "Source" / "TBW" / "Public" / "Core" / "TBWWorldFlags.h"
FLAGS_CPP = ROOT / "Source" / "TBW" / "Private" / "Core" / "TBWWorldFlags.cpp"
BIBLE = ROOT / "docs" / "CHARACTER_BIBLE.md"

errors: list[str] = []
warnings: list[str] = []


def known_flags() -> set[str]:
    text = FLAGS_CPP.read_text(encoding="utf-8")
    return {m[1] for m in re.findall(r'case ETBWWorldFlag::(\w+):\s*return TEXT\("(\w+)"\)', text)}


def known_speakers() -> set[str]:
    """Names the story bible actually establishes, plus the six sons."""
    names = {"Orin", "Nofan", "Raynor", "Darius", "Evan", "Malik", "Soren",
             "Leila", "Yasmin", "Nurse", "Guard", "Boatman", "Steward"}
    if BIBLE.is_file():
        text = BIBLE.read_text(encoding="utf-8")
        for m in re.findall(r"^#{2,3}\s+([A-Z][a-zA-Z]+)", text, re.M):
            names.add(m)
    return names


def main() -> int:
    flags = known_flags()
    speakers = known_speakers()

    scenes = sorted(DIALOGUE_DIR.glob("*.json")) if DIALOGUE_DIR.is_dir() else []
    if not scenes:
        errors.append(f"no dialogue scenes under {DIALOGUE_DIR}")

    total_lines = 0
    total_seconds = 0.0
    scene_ids: set[str] = set()
    used_flags: set[str] = set()

    for path in scenes:
        rel = path.relative_to(ROOT)
        raw = path.read_bytes()
        try:
            raw.decode("utf-8")
        except UnicodeDecodeError as exc:
            errors.append(f"{rel}: not valid UTF-8 ({exc})")
            continue

        try:
            data = json.loads(raw.decode("utf-8"))
        except json.JSONDecodeError as exc:
            errors.append(f"{rel}: malformed JSON at line {exc.lineno} ({exc.msg})")
            continue

        sid = data.get("scene_id")
        if not sid:
            errors.append(f"{rel}: missing scene_id")
            continue
        if sid in scene_ids:
            errors.append(f"{rel}: duplicate scene_id '{sid}'")
        scene_ids.add(sid)

        flag = data.get("completion_flag")
        if flag:
            used_flags.add(flag)
            if flag not in flags:
                errors.append(f"{rel}: completion_flag '{flag}' has no case in FTBWWorldFlags::ToName")

        lines = data.get("lines") or []
        if not lines:
            errors.append(f"{rel}: no lines")
        for i, line in enumerate(lines):
            total_lines += 1
            en = line.get("en", "")
            ar = line.get("ar", "")
            if not en:
                errors.append(f"{rel} line {i}: missing English text (production language)")
            if not ar:
                warnings.append(f"{rel} line {i}: no Arabic yet - will fall back to English")
            if ar and not re.search(r"[\u0600-\u06FF]", ar):
                errors.append(f"{rel} line {i}: 'ar' field contains no Arabic characters")

            speaker = line.get("speaker")
            if speaker and speaker not in speakers:
                warnings.append(f"{rel} line {i}: speaker '{speaker}' is not in the character bible")
            if not speaker and not line.get("action"):
                warnings.append(f"{rel} line {i}: no speaker and not marked as an action line")

            seconds = line.get("seconds")
            if seconds is not None:
                if not isinstance(seconds, (int, float)) or seconds <= 0:
                    errors.append(f"{rel} line {i}: seconds must be a positive number")
                else:
                    total_seconds += float(seconds)
                    # ~14 characters a second is already fast for a subtitle
                    longest = max(len(en), len(ar))
                    if longest / max(seconds, 0.1) > 22:
                        warnings.append(
                            f"{rel} line {i}: {longest} chars in {seconds}s is too fast to read")

    # ---------------------------------------------------------------- objectives
    rules = []
    if OBJECTIVES.is_file():
        try:
            rules = json.loads(OBJECTIVES.read_text(encoding="utf-8")).get("objectives", [])
        except json.JSONDecodeError as exc:
            errors.append(f"Objectives.json: malformed JSON at line {exc.lineno} ({exc.msg})")
    else:
        errors.append("Content/TBW/Data/Objectives.json is missing")

    for i, rule in enumerate(rules):
        for key in ("requires", "blocked_by"):
            for flag in rule.get(key, []):
                used_flags.add(flag)
                if flag not in flags:
                    errors.append(f"Objectives.json rule {i}: {key} flag '{flag}' is not a real world flag")
        if not rule.get("en"):
            errors.append(f"Objectives.json rule {i}: missing English text")
        if not rule.get("ar"):
            warnings.append(f"Objectives.json rule {i}: no Arabic text")

    # An objective nothing can ever satisfy is dead content.
    for i, rule in enumerate(rules):
        overlap = set(rule.get("requires", [])) & set(rule.get("blocked_by", []))
        if overlap:
            errors.append(f"Objectives.json rule {i}: flag(s) {sorted(overlap)} both required and blocking")

    # Every rule after the first should be reachable: its requirements must be
    # satisfiable by some earlier rule's blocking flag, or by an interactable.
    layout_flags: set[str] = set()
    layout = ROOT / "web" / "preview" / "data" / "east_wing.json"
    if layout.is_file():
        layout_flags = {i["flag"] for i in json.loads(layout.read_text(encoding="utf-8")).get("interactables", [])}

    producible = layout_flags | {f for f in used_flags if f in {r.get("completion_flag") for r in []}}
    producible |= {d for d in (json.loads(p.read_text(encoding="utf-8")).get("completion_flag")
                               for p in scenes) if d}
    for i, rule in enumerate(rules):
        for flag in rule.get("requires", []):
            if flag not in producible:
                warnings.append(
                    f"Objectives.json rule {i}: nothing in the level or the dialogue sets '{flag}' yet")

    # The data only helps if it actually ships. Raw .json under Content/ is not
    # a uasset, so the cooker ignores it; only UFS staging copies it into the
    # pak. Without that line the packaged game launches with no dialogue and no
    # objectives, and nothing before launch would have said so.
    game_ini = ROOT / "Config" / "DefaultGame.ini"
    if game_ini.is_file():
        ini = game_ini.read_text(encoding="utf-8")
        if 'DirectoriesToAlwaysStageAsUFS=(Path="TBW/Data")' not in ini:
            errors.append(
                "DefaultGame.ini: Content/TBW/Data is not staged as UFS - the "
                "packaged game would ship with no dialogue and no objectives")

    # The editor must not try to turn these files into DataTable assets.
    editor_ini = ROOT / "Config" / "DefaultEditor.ini"
    if editor_ini.is_file():
        ed = editor_ini.read_text(encoding="utf-8")
        if "bMonitorContentDirectories=False" not in ed:
            warnings.append(
                "DefaultEditor.ini: content monitoring is on - the editor will "
                "offer to import Content/TBW/Data/*.json as DataTables")

    print("=" * 66)
    print("NARRATIVE DATA CHECK")
    print("=" * 66)
    print(f"  scenes         {len(scene_ids)}")
    print(f"  spoken lines   {total_lines}")
    print(f"  authored time  {total_seconds/60:.1f} min")
    print(f"  objectives     {len(rules)}")
    print(f"  flags in use   {len(used_flags)} of {len(flags)} declared")
    print()

    for w in warnings:
        print(f"  !  {w}")
    for e in errors:
        print(f"  X  {e}")

    print()
    print("RESULT: FAIL" if errors else "RESULT: PASS")
    return 1 if errors else 0


if __name__ == "__main__":
    sys.exit(main())
