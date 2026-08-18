#!/usr/bin/env python3
"""
Cross-check the shared level layout against the C++ that has to consume it.

A layout file is data, so nothing stops it from naming a world flag that does not
exist, or dropping a story prop inside a wall. Both fail silently at runtime:
the flag write goes to an ad-hoc name nobody reads, and the prop is unreachable.
This catches them before the editor is even opened.

    python3 tools/validate_layout.py
"""

from __future__ import annotations

import json
import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
LAYOUT = ROOT / "web" / "preview" / "data" / "east_wing.json"
FLAGS_H = ROOT / "Source" / "TBW" / "Public" / "Core" / "TBWWorldFlags.h"
FLAGS_CPP = ROOT / "Source" / "TBW" / "Private" / "Core" / "TBWWorldFlags.cpp"

errors: list[str] = []
warnings: list[str] = []


def known_flags() -> set[str]:
    text = FLAGS_H.read_text(encoding="utf-8")
    body = text[text.index("enum class ETBWWorldFlag"):]
    body = body[: body.index("};")]
    names = set()
    for line in body.splitlines():
        line = line.strip().rstrip(",")
        if not line or line.startswith(("//", "enum", "{")):
            continue
        name = line.split("=")[0].split()[0].strip()
        if name and name not in {"None", "COUNT"} and "UMETA" not in name:
            names.add(name)
    return names


def mapped_flags() -> set[str]:
    """Flags that FTBWWorldFlags::ToName actually returns a string for."""
    text = FLAGS_CPP.read_text(encoding="utf-8")
    return set(re.findall(r'case ETBWWorldFlag::(\w+):\s*return TEXT\("(\w+)"\)', text) and
               [m[1] for m in re.findall(r'case ETBWWorldFlag::(\w+):\s*return TEXT\("(\w+)"\)', text)])


def aabb(entry) -> tuple[list[float], list[float]]:
    c, s = entry["center"], entry["size"]
    return ([c[i] - s[i] / 2 for i in range(3)], [c[i] + s[i] / 2 for i in range(3)])


def main() -> int:
    if not LAYOUT.is_file():
        print(f"layout missing: {LAYOUT}\nrun: python3 tools/layout/east_wing.py")
        return 1

    data = json.loads(LAYOUT.read_text(encoding="utf-8"))
    boxes = data["boxes"]
    items = data.get("interactables", [])
    flags_enum = known_flags()
    flags_named = mapped_flags()

    # 1 — every story flag must exist in C++ and be reachable by name
    for it in items:
        flag = it["flag"]
        if flag not in flags_enum:
            errors.append(
                f"{it['id']}: flag '{flag}' is not in ETBWWorldFlag. "
                f"Add it to TBWWorldFlags.h or fix the layout.")
        elif flag not in flags_named:
            errors.append(
                f"{it['id']}: flag '{flag}' exists in the enum but FTBWWorldFlags::ToName "
                "has no case for it, so tbw.Flags.List will never show it.")

    # 2 — no story prop buried inside architecture
    solids = [aabb(b) for b in boxes if b["kind"] != "water"]
    for it in items:
        loc = it["location"]
        half = [s / 2 for s in it["size"]]
        lo = [loc[i] - half[i] for i in range(3)]
        hi = [loc[i] + half[i] for i in range(3)]
        for smin, smax in solids:
            overlap = all(hi[i] > smin[i] + 1.0 and lo[i] < smax[i] - 1.0 for i in range(3))
            if overlap:
                errors.append(
                    f"{it['id']} at {loc} intersects level geometry - it would be unreachable.")
                break

    # 3 — props must sit inside the playable footprint
    fx, fy = [v * 100 for v in data["footprint_m"]]
    for it in items:
        x, y, _ = it["location"]
        if not (0 <= x <= fx and 0 <= y <= fy):
            warnings.append(f"{it['id']} at x={x} y={y} is outside the {fx/100:.0f}x{fy/100:.0f} m footprint.")

    # 4 — ids and flags unique
    for key in ("id", "flag"):
        seen = {}
        for it in items:
            seen.setdefault(it[key], []).append(it["id"])
        for value, owners in seen.items():
            if len(owners) > 1:
                errors.append(f"duplicate {key} '{value}' used by {owners}")

    # 5 — the layout must actually describe a level
    if len(boxes) < 50:
        errors.append(f"only {len(boxes)} boxes - the wing did not build")
    if not data.get("player_start"):
        errors.append("no player_start in the layout")

    print("=" * 66)
    print("LAYOUT / C++ CROSS-CHECK")
    print("=" * 66)
    print(f"  boxes          {len(boxes)}")
    print(f"  interactables  {len(items)}")
    print(f"  flags in C++   {len(flags_enum)}")
    print(f"  story flags    {', '.join(sorted(i['flag'] for i in items))}")
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
