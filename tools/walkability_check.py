#!/usr/bin/env python3
"""
Can the player actually walk there?

Geometry that looks right in a viewport is routinely unplayable: a dais too tall
to step onto, a room whose only door was replaced by a window, a lintel at head
height. None of that shows up in a screenshot, and all of it costs an editor
session to diagnose.

This walks the shared layout the way UCharacterMovementComponent would:

    step height      32 cm   (MaxStepHeight in TBWPlayerCharacter)
    capsule radius   38 cm
    standing height 176 cm   (2 x 88 cm half height)
    crouch height   116 cm   (2 x 58 cm crouched half height)

It builds a height field, floods it from the PlayerStart, then asks whether every
story beat has walkable floor next to it.

    python3 tools/walkability_check.py
    python3 tools/walkability_check.py --map      # ASCII floor plan
"""

from __future__ import annotations

import argparse
import json
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
LAYOUT = ROOT / "web" / "preview" / "data" / "east_wing.json"

CELL = 25.0          # cm per grid cell
STEP = 32.0          # UE MaxStepHeight
RADIUS = 38.0        # capsule radius
STAND = 176.0
CROUCH = 116.0
REACH = 250.0        # interactor TraceDistance


def build_surfaces(data):
    """
    Multi-level navigation, built the way Recast does it.

    An earlier version inflated every box by the capsule radius and then asked
    whether a surface was clear. That is correct for walls and catastrophically
    wrong for stairs: standing on one tread, the tread two above is 40 cm higher,
    and its inflated footprint spills back over you, so every step of every flight
    was declared a wall and the upper floor was unreachable.

    The right order is: decide what is standable with no inflation at all, then
    ERODE the result by the agent radius. Adjacent treads all remain standable
    because none of them is solid; only real walls erode their neighbours.
    """
    fx, fy = [v * 100 for v in data["footprint_m"]]
    margin = 200.0
    nx = int((fx + 2 * margin) / CELL)
    ny = int((fy + 2 * margin) / CELL)
    ox = oy = -margin

    solids = [b for b in data["boxes"] if b["kind"] != "water"]

    def span(b):
        cx, cy, _ = b["center"]
        sx, sy, _ = b["size"]
        return (max(0, int((cx - sx / 2 - ox) / CELL)),
                min(nx - 1, int((cx + sx / 2 - ox) / CELL)),
                max(0, int((cy - sy / 2 - oy) / CELL)),
                min(ny - 1, int((cy + sy / 2 - oy) / CELL)))

    tops = [[set() for _ in range(nx)] for _ in range(ny)]
    vols = [[[] for _ in range(nx)] for _ in range(ny)]
    for b in solids:
        top = b["center"][2] + b["size"][2] / 2
        bottom = b["center"][2] - b["size"][2] / 2
        x0, x1, y0, y1 = span(b)
        for gy in range(y0, y1 + 1):
            for gx in range(x0, x1 + 1):
                tops[gy][gx].add(round(top, 1))
                vols[gy][gx].append((bottom, top))

    stand = [[{} for _ in range(nx)] for _ in range(ny)]
    for gy in range(ny):
        for gx in range(nx):
            volumes = vols[gy][gx]
            for h in tops[gy][gx]:
                head = 9999.0
                ok = True
                for bottom, top in volumes:
                    if top <= h + STEP:
                        continue                     # the surface itself, or a step
                    if bottom < h + CROUCH:
                        ok = False                   # solid through the body
                        break
                    if bottom < head:
                        head = bottom
                if ok:
                    stand[gy][gx][h] = head - h

    # Erode by the capsule radius: a cell touching solid ground within one radius
    # cannot hold the player's centre. Two cells at 25 cm covers the 38 cm radius.
    pad = int(round(RADIUS / CELL))
    solid_cells = [[not stand[y][x] for x in range(nx)] for y in range(ny)]
    eroded = [[dict(stand[y][x]) for x in range(nx)] for y in range(ny)]
    for gy in range(ny):
        for gx in range(nx):
            if not eroded[gy][gx]:
                continue
            hit = False
            for dy in range(-pad, pad + 1):
                for dx in range(-pad, pad + 1):
                    y2, x2 = gy + dy, gx + dx
                    if not (0 <= x2 < nx and 0 <= y2 < ny) or solid_cells[y2][x2]:
                        hit = True
                        break
                if hit:
                    break
            if hit:
                eroded[gy][gx] = {}
    return eroded, nx, ny, ox, oy


def flood(data, stand, nx, ny, ox, oy):
    ps = data["player_start"]["location"]
    sx = int((ps[0] - ox) / CELL)
    sy = int((ps[1] - oy) / CELL)
    if not stand[sy][sx]:
        return None, "the PlayerStart has no standable surface"

    # start on the surface nearest the spawn Z
    start_h = min(stand[sy][sx], key=lambda h: abs(h - (ps[2] - 120.0)))
    reach = {(sx, sy, start_h)}
    stack = [(sx, sy, start_h)]
    while stack:
        x, y, h = stack.pop()
        for dx, dy in ((1, 0), (-1, 0), (0, 1), (0, -1)):
            nx2, ny2 = x + dx, y + dy
            if not (0 <= nx2 < nx and 0 <= ny2 < ny):
                continue
            for h2, head in stand[ny2][nx2].items():
                if abs(h2 - h) > STEP or head < CROUCH:
                    continue
                node = (nx2, ny2, h2)
                if node not in reach:
                    reach.add(node)
                    stack.append(node)
    return reach, None


def path_seconds(reach, stand, nx, ny, ox, oy, start, goal, speed_ms):
    """
    Shortest walk, in seconds. Eight-connected with true diagonal cost and no
    corner cutting - four-connected search inflates diagonal routes by up to 41%
    and would have reported walk budgets that no player experiences.
    Returns None if the goal cannot be walked to.
    """
    import heapq

    sx = int((start[0] - ox) / CELL)
    sy = int((start[1] - oy) / CELL)
    gx = int((goal[0] - ox) / CELL)
    gy = int((goal[1] - oy) / CELL)
    here = [n for n in reach if n[0] == sx and n[1] == sy]
    if not here:
        return None
    start_h = min(here, key=lambda n: abs(n[2] - (start[2] - 120.0)))[2]
    walkable = {(n[0], n[1]) for n in reach}
    levels = {}
    for n in reach:
        levels.setdefault((n[0], n[1]), []).append(n[2])

    DIAG = CELL * 1.41421356
    best = {(sx, sy, start_h): 0.0}
    heap = [(0.0, sx, sy, start_h)]
    while heap:
        d, x, y, h = heapq.heappop(heap)
        if d > best.get((x, y, h), 1e18):
            continue
        if abs(x - gx) <= 1 and abs(y - gy) <= 1:
            return (d / 100.0) / speed_ms
        for dx, dy in ((1, 0), (-1, 0), (0, 1), (0, -1),
                       (1, 1), (1, -1), (-1, 1), (-1, -1)):
            nx2, ny2 = x + dx, y + dy
            if (nx2, ny2) not in walkable:
                continue
            if dx and dy:
                # no squeezing through a diagonal gap between two blockers
                if (x + dx, y) not in walkable or (x, y + dy) not in walkable:
                    continue
                step = DIAG
            else:
                step = CELL
            for h2 in levels[(nx2, ny2)]:
                if abs(h2 - h) > STEP:
                    continue
                nd = d + step
                if nd < best.get((nx2, ny2, h2), 1e18):
                    best[(nx2, ny2, h2)] = nd
                    heapq.heappush(heap, (nd, nx2, ny2, h2))
    return None


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("--map", action="store_true", help="print an ASCII floor plan")
    ap.add_argument("--times", action="store_true", help="measure walk times between rooms")
    ap.add_argument("--level", type=float, default=None,
                    help="only report surfaces near this height, in cm (e.g. 940 for the upper chamber)")
    args = ap.parse_args()

    if not LAYOUT.is_file():
        print(f"layout missing: {LAYOUT}")
        return 1
    data = json.loads(LAYOUT.read_text(encoding="utf-8"))

    stand, nx, ny, ox, oy = build_surfaces(data)
    reach, err = flood(data, stand, nx, ny, ox, oy)
    if err:
        print(f"X  {err}")
        return 1

    by_level = {}
    for x, y, h in reach:
        by_level.setdefault(round(h / 100.0), 0)
        by_level[round(h / 100.0)] += 1
    ground = sum(c for m, c in by_level.items() if m <= 2)
    upper = sum(c for m, c in by_level.items() if m > 2)

    print("=" * 66)
    print("WALKABILITY  (UE rules: 32 cm step, 38 cm radius, 176/116 cm heights)")
    print("=" * 66)
    print(f"  reachable nodes  {len(reach)}")
    print(f"  ground level     {ground * (CELL / 100.0) ** 2:.0f} m^2")
    print(f"  above 2 m        {upper * (CELL / 100.0) ** 2:.0f} m^2   (stair treads and the upper chamber)")
    print()

    failures = 0
    print("  story beats")
    for it in data.get("interactables", []):
        bx, by, bz = it["location"]
        gx = int((bx - ox) / CELL)
        gy = int((by - oy) / CELL)
        span = int(REACH / CELL)

        best = None
        for x, y, h in reach:
            if abs(x - gx) > span or abs(y - gy) > span:
                continue
            px = ox + x * CELL
            py = oy + y * CELL
            d = ((px - bx) ** 2 + (py - by) ** 2) ** 0.5
            if d > REACH:
                continue
            dz = bz - h
            if -50.0 <= dz <= 200.0 and (best is None or d < best):
                best = d

        ok = best is not None
        if not ok:
            failures += 1
        print(f"    {'OK ' if ok else 'X  '} {it['id']:<14} "
              f"{'reachable from ' + format(best, '.0f') + ' cm' if ok else 'NO WALKABLE SURFACE WITHIN REACH'}")

    if args.times:
        rois = data.get("rooms_of_interest", {})
        ps = data["player_start"]["location"]
        print()
        print("  walk times from the PlayerStart (4.2 m/s walk, 6.2 m/s sprint)")
        for name, loc in rois.items():
            t_walk = path_seconds(reach, stand, nx, ny, ox, oy, ps, loc, 4.2)
            t_run = path_seconds(reach, stand, nx, ny, ox, oy, ps, loc, 6.2)
            if t_walk is None:
                print(f"    X   {name:<20} unreachable on foot")
                failures += 1
            else:
                print(f"    OK  {name:<20} {t_walk:5.1f} s walk   {t_run:5.1f} s sprint")

        hall = rois.get("R2_AudienceHall")
        dock = rois.get("R1_Dock")
        if hall and dock:
            t = path_seconds(reach, stand, nx, ny, ox, oy, hall, dock, 4.2)
            if t is not None:
                verdict = "within the 25 s budget" if t <= 25.0 else "OVER the 25 s budget"
                print(f"\n    hall -> canal gate: {t:.1f} s walking - {verdict}")
                if t > 25.0:
                    failures += 1

    if args.map:
        level = args.level if args.level is not None else 0.0
        cells = {(x, y) for x, y, h in reach if abs(h - level) <= 120.0}
        print()
        print(f"  floor plan at z = {level / 100:.1f} m   . walk   # solid   S start   * beat")
        beats = {(int((it['location'][0] - ox) / CELL) // 2,
                  int((it['location'][1] - oy) / CELL) // 2)
                 for it in data.get("interactables", [])
                 if abs(it['location'][2] - level) <= 250.0}
        ps = data["player_start"]["location"]
        start_c = (int((ps[0] - ox) / CELL) // 2, int((ps[1] - oy) / CELL) // 2)
        for gy in range(ny - 1, -1, -4):
            row = []
            for gx in range(0, nx, 2):
                key = (gx // 2, gy // 2)
                if key == start_c and level < 200:
                    row.append("S")
                elif key in beats:
                    row.append("*")
                elif (gx, gy) in cells:
                    row.append(".")
                elif stand[gy][gx]:
                    row.append("-")
                else:
                    row.append("#")
            print("   " + "".join(row))

    print()
    print("RESULT: FAIL" if failures else "RESULT: PASS")
    return 1 if failures else 0


if __name__ == "__main__":
    sys.exit(main())
