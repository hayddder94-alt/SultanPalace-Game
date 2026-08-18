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


def build_grid(data):
    """
    Two passes, because what blocks you depends on where the floor is.

    pass 1  floor height per cell   = tallest surface at or below 2 m
    pass 2  for that floor, a box is
              BLOCKING     if its top is more than a step above the floor and its
                           underside is below crouch height  -> a wall
              OVERHEAD     if its underside clears crouch but not standing
                           -> duck under it
              IGNORED      if it clears standing height       -> a lintel, a ceiling
    """
    fx, fy = [v * 100 for v in data["footprint_m"]]
    margin = 200.0
    nx = int((fx + 2 * margin) / CELL)
    ny = int((fy + 2 * margin) / CELL)
    ox = oy = -margin

    solids = [b for b in data["boxes"] if b["kind"] != "water"]

    def cells_of(b, inflate):
        """
        Blockers are inflated by the capsule radius - you cannot press your centre
        into a wall. Walkable SURFACES must not be inflated: a capsule may overhang
        the edge of a step. Inflating them merged a three-tread stair into one
        60 cm block and made the dais unclimbable, which is how this was found.
        """
        pad = RADIUS if inflate else 0.0
        cx, cy, _ = b["center"]
        sx, sy, _ = b["size"]
        x0 = max(0, int((cx - sx / 2 - pad - ox) / CELL))
        x1 = min(nx - 1, int((cx + sx / 2 + pad - ox) / CELL))
        y0 = max(0, int((cy - sy / 2 - pad - oy) / CELL))
        y1 = min(ny - 1, int((cy + sy / 2 + pad - oy) / CELL))
        return x0, x1, y0, y1

    floor = [[None] * nx for _ in range(ny)]
    for b in solids:
        cz, sz = b["center"][2], b["size"][2]
        top = cz + sz / 2
        if top > 200.0:
            continue                                  # not something you stand on
        x0, x1, y0, y1 = cells_of(b, inflate=False)
        for gy in range(y0, y1 + 1):
            for gx in range(x0, x1 + 1):
                cur = floor[gy][gx]
                if cur is None or top > cur:
                    floor[gy][gx] = top

    blocked = [[False] * nx for _ in range(ny)]
    ceiling = [[9999.0] * nx for _ in range(ny)]
    for b in solids:
        cz, sz = b["center"][2], b["size"][2]
        top = cz + sz / 2
        bottom = cz - sz / 2
        x0, x1, y0, y1 = cells_of(b, inflate=True)
        for gy in range(y0, y1 + 1):
            for gx in range(x0, x1 + 1):
                f = floor[gy][gx]
                if f is None:
                    continue
                if top <= f + STEP:
                    continue                          # a step, a kerb, the floor itself
                if bottom < f + CROUCH:
                    blocked[gy][gx] = True            # a wall: no posture gets through
                elif bottom < f + STAND:
                    if bottom < ceiling[gy][gx]:
                        ceiling[gy][gx] = bottom      # duck under it

    return floor, ceiling, blocked, nx, ny, ox, oy


def flood(data, floor, ceiling, blocked, nx, ny, ox, oy):
    ps = data["player_start"]["location"]
    sx = int((ps[0] - ox) / CELL)
    sy = int((ps[1] - oy) / CELL)

    if floor[sy][sx] is None:
        return None, "the PlayerStart has no floor under it"
    if blocked[sy][sx]:
        return None, "the PlayerStart is inside a wall"

    reach = [[False] * nx for _ in range(ny)]
    crouched = [[False] * nx for _ in range(ny)]
    stack = [(sx, sy)]
    reach[sy][sx] = True

    while stack:
        x, y = stack.pop()
        h = floor[y][x]
        for dx, dy in ((1, 0), (-1, 0), (0, 1), (0, -1)):
            nx2, ny2 = x + dx, y + dy
            if not (0 <= nx2 < nx and 0 <= ny2 < ny) or reach[ny2][nx2]:
                continue
            if blocked[ny2][nx2]:
                continue                       # wall
            nh = floor[ny2][nx2]
            if nh is None:
                continue                       # no ground: a hole, or outside
            if abs(nh - h) > STEP:
                continue                       # too tall to step, too deep to drop safely
            head = ceiling[ny2][nx2] - nh
            if head < CROUCH:
                continue                       # cannot fit even crouched
            reach[ny2][nx2] = True
            crouched[ny2][nx2] = head < STAND
            stack.append((nx2, ny2))

    return (reach, crouched), None


def path_seconds(reach, floor, blocked, nx, ny, ox, oy, start, goal, speed_ms):
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
    if not (0 <= sx < nx and 0 <= sy < ny) or not reach[sy][sx]:
        return None

    DIAG = CELL * 1.41421356
    best = {(sx, sy): 0.0}
    heap = [(0.0, sx, sy)]
    while heap:
        d, x, y = heapq.heappop(heap)
        if d > best.get((x, y), 1e18):
            continue
        if abs(x - gx) <= 1 and abs(y - gy) <= 1:
            return (d / 100.0) / speed_ms
        for dx, dy in ((1, 0), (-1, 0), (0, 1), (0, -1),
                       (1, 1), (1, -1), (-1, 1), (-1, -1)):
            nx2, ny2 = x + dx, y + dy
            if not (0 <= nx2 < nx and 0 <= ny2 < ny) or not reach[ny2][nx2]:
                continue
            if dx and dy:
                # no squeezing through a diagonal gap between two blockers
                if not reach[y][x + dx] or not reach[y + dy][x]:
                    continue
                step = DIAG
            else:
                step = CELL
            nd = d + step
            if nd < best.get((nx2, ny2), 1e18):
                best[(nx2, ny2)] = nd
                heapq.heappush(heap, (nd, nx2, ny2))
    return None


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("--map", action="store_true", help="print an ASCII floor plan")
    ap.add_argument("--times", action="store_true", help="measure walk times between rooms")
    args = ap.parse_args()

    if not LAYOUT.is_file():
        print(f"layout missing: {LAYOUT}")
        return 1
    data = json.loads(LAYOUT.read_text(encoding="utf-8"))

    floor, ceiling, blocked, nx, ny, ox, oy = build_grid(data)
    result, err = flood(data, floor, ceiling, blocked, nx, ny, ox, oy)
    if err:
        print(f"X  {err}")
        return 1
    reach, crouched = result

    walkable = sum(r.count(True) for r in reach)
    crouch_cells = sum(r.count(True) for r in crouched)
    area = walkable * (CELL / 100.0) ** 2

    print("=" * 66)
    print("WALKABILITY  (UE rules: 32 cm step, 38 cm radius, 176/116 cm heights)")
    print("=" * 66)
    print(f"  reachable area   {area:.0f} m^2  ({walkable} cells)")
    print(f"  crouch-only      {crouch_cells} cells")
    print()

    failures = 0
    print("  story beats")
    for it in data.get("interactables", []):
        bx, by, bz = it["location"]
        gx = int((bx - ox) / CELL)
        gy = int((by - oy) / CELL)
        span = int(REACH / CELL)

        best = None
        for y in range(max(0, gy - span), min(ny, gy + span + 1)):
            for x in range(max(0, gx - span), min(nx, gx + span + 1)):
                if not reach[y][x]:
                    continue
                px = ox + x * CELL
                py = oy + y * CELL
                d = ((px - bx) ** 2 + (py - by) ** 2) ** 0.5
                if d > REACH:
                    continue
                # the interactor traces from eye height, so the prop must not be
                # far above the floor you are standing on
                dz = bz - (floor[y][x] or 0.0)
                if -50.0 <= dz <= 200.0 and (best is None or d < best):
                    best = d

        ok = best is not None
        if not ok:
            failures += 1
        print(f"    {'OK ' if ok else 'X  '} {it['id']:<14} "
              f"{'reachable from ' + format(best, '.0f') + ' cm' if ok else 'NO WALKABLE FLOOR WITHIN REACH'}")

    if args.times:
        # The wing spec commits to hall -> canal in 25 s and hall -> annex in 40 s.
        # Pawn speeds: 420 uu/s walk, 620 uu/s sprint.
        rois = data.get("rooms_of_interest", {})
        ps = data["player_start"]["location"]
        print()
        print("  walk times from the PlayerStart (4.2 m/s walk, 6.2 m/s sprint)")
        for name, loc in rois.items():
            t_walk = path_seconds(reach, floor, blocked, nx, ny, ox, oy, ps, loc, 4.2)
            t_run = path_seconds(reach, floor, blocked, nx, ny, ox, oy, ps, loc, 6.2)
            if t_walk is None:
                print(f"    X   {name:<20} unreachable")
                failures += 1
            else:
                print(f"    OK  {name:<20} {t_walk:5.1f} s walk   {t_run:5.1f} s sprint")

        hall = rois.get("R2_AudienceHall")
        dock = rois.get("R1_Dock")
        if hall and dock:
            t = path_seconds(reach, floor, blocked, nx, ny, ox, oy, hall, dock, 4.2)
            if t is not None:
                verdict = "within the 25 s budget" if t <= 25.0 else "OVER the 25 s budget"
                print(f"\n    hall -> canal gate: {t:.1f} s walking - {verdict}")
                if t > 25.0:
                    failures += 1

    if args.map:
        print()
        print("  floor plan   . walk   c crouch   # blocked   S start   * beat")
        beats = {}
        for it in data.get("interactables", []):
            beats[(int((it['location'][0] - ox) / CELL) // 2,
                   int((it['location'][1] - oy) / CELL) // 2)] = "*"
        ps = data["player_start"]["location"]
        start = (int((ps[0] - ox) / CELL) // 2, int((ps[1] - oy) / CELL) // 2)
        for gy in range(ny - 1, -1, -4):
            row = []
            for gx in range(0, nx, 2):
                key = (gx // 2, gy // 2)
                if key == start:
                    row.append("S")
                elif key in beats:
                    row.append("*")
                elif reach[gy][gx]:
                    row.append("c" if crouched[gy][gx] else ".")
                elif blocked[gy][gx]:
                    row.append("#")
                else:
                    row.append("-" if floor[gy][gx] is not None else " ")
            print("   " + "".join(row))

    print()
    print("RESULT: FAIL" if failures else "RESULT: PASS")
    return 1 if failures else 0


if __name__ == "__main__":
    sys.exit(main())
