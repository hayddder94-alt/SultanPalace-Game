"""
The Betrayed Will / وصية الغدر — East Ceremonial Wing layout, single source of truth.

Pure Python. No Unreal import. Both consumers read this:

    tools/ue_python/build_east_wing.py   -> spawns it inside UE 5.8
    web/preview/                          -> walks it in a browser

Keeping one authority is the whole point: a layout that exists twice drifts within
a week, and then the browser preview is a lie.

Units: centimetres (Unreal units). 1 m = 100 uu. Z up, X east, Y north.
Dimensions follow docs/PALACE_WING_SPEC.md.
"""

M = 100.0

WALL_T = 0.35 * M
WALL_H = 5.5 * M
HALL_H = 9.0 * M
DOOR_W = 1.6 * M
DOOR_H = 2.6 * M

# surface kinds -> the material family each consumer resolves in its own way
WALL, FLOOR, HERO, GROUND, WATER = "wall", "floor", "hero", "ground", "water"


class Layout:
    def __init__(self):
        self.boxes = []

    # -- primitives ---------------------------------------------------------

    def box(self, name, center, size, kind=WALL):
        self.boxes.append({
            "name": name,
            "center": [round(c, 2) for c in center],
            "size": [round(s, 2) for s in size],
            "kind": kind,
        })

    def wall(self, name, p0, p1, height, thickness, kind=WALL,
             door_at=None, door_w=DOOR_W, door_h=DOOR_H):
        """Straight axis-aligned wall, optionally pierced by one doorway with a lintel."""
        x0, y0 = p0
        x1, y1 = p1
        dx, dy = x1 - x0, y1 - y0
        length = (dx * dx + dy * dy) ** 0.5
        if length <= 1.0:
            return
        horizontal = abs(dx) >= abs(dy)

        def segment(tag, start, end):
            if end - start < 1.0:
                return
            mid = (start + end) * 0.5
            if horizontal:
                cx, cy = x0 + (dx / length) * mid, y0
                size = (end - start, thickness, height)
            else:
                cx, cy = x0, y0 + (dy / length) * mid
                size = (thickness, end - start, height)
            self.box("{0}_{1}".format(name, tag), (cx, cy, height * 0.5), size, kind)

        if door_at is None:
            segment("solid", 0.0, length)
            return

        opening = length * door_at
        a = max(0.0, opening - door_w * 0.5)
        b = min(length, opening + door_w * 0.5)
        segment("a", 0.0, a)
        segment("b", b, length)

        if height > door_h:
            mid = (a + b) * 0.5
            if horizontal:
                cx, cy = x0 + (dx / length) * mid, y0
                size = (b - a, thickness, height - door_h)
            else:
                cx, cy = x0, y0 + (dy / length) * mid
                size = (thickness, b - a, height - door_h)
            self.box("{0}_lintel".format(name), (cx, cy, door_h + (height - door_h) * 0.5), size, kind)

    def window_wall(self, name, p0, p1, height, thickness, kind=WALL,
                    at=0.5, width=1.8 * M, sill=3.2 * M, header=4.6 * M):
        """
        Wall pierced by a high window. Clerestory light is the whole reason the
        audience hall reads as a hall and not a box: sun enters above head height,
        hits the floor in bars, and Lumen bounces it into the room.
        """
        x0, y0 = p0
        x1, y1 = p1
        dx, dy = x1 - x0, y1 - y0
        length = (dx * dx + dy * dy) ** 0.5
        if length <= 1.0:
            return
        horizontal = abs(dx) >= abs(dy)
        a = max(0.0, length * at - width * 0.5)
        b = min(length, length * at + width * 0.5)

        def piece(tag, start, end, z0, z1):
            if end - start < 1.0 or z1 - z0 < 1.0:
                return
            mid = (start + end) * 0.5
            if horizontal:
                cx, cy = x0 + (dx / length) * mid, y0
                size = (end - start, thickness, z1 - z0)
            else:
                cx, cy = x0, y0 + (dy / length) * mid
                size = (thickness, end - start, z1 - z0)
            self.box("{0}_{1}".format(name, tag), (cx, cy, (z0 + z1) * 0.5), size, kind)

        piece("left", 0.0, a, 0.0, height)
        piece("right", b, length, 0.0, height)
        piece("sill", a, b, 0.0, sill)
        piece("header", a, b, header, height)

    def room(self, name, x, y, w, d, height, doors=(), ceiling=False,
             wall_kind=WALL, floor_kind=FLOOR):
        door_map = dict(doors)
        self.box("{0}_Floor".format(name), (x + w * 0.5, y + d * 0.5, -0.1 * M),
                 (w, d, 0.2 * M), floor_kind)
        self.wall("{0}_S".format(name), (x, y), (x + w, y), height, WALL_T, wall_kind, door_map.get("S"))
        self.wall("{0}_N".format(name), (x, y + d), (x + w, y + d), height, WALL_T, wall_kind, door_map.get("N"))
        self.wall("{0}_W".format(name), (x, y), (x, y + d), height, WALL_T, wall_kind, door_map.get("W"))
        self.wall("{0}_E".format(name), (x + w, y), (x + w, y + d), height, WALL_T, wall_kind, door_map.get("E"))
        if ceiling:
            self.box("{0}_Ceiling".format(name), (x + w * 0.5, y + d * 0.5, height + 0.1 * M),
                     (w, d, 0.2 * M), wall_kind)


# ---------------------------------------------------------------------------

def build():
    L = Layout()

    # R2 audience hall - the hero volume
    hx, hy, hw, hd = 11 * M, 26 * M, 18 * M, 24 * M
    L.room("R2_AudienceHall", hx, hy, hw, hd, HALL_H,
           doors=(("S", 0.5), ("W", 0.5), ("E", 0.35)), wall_kind=HERO)
    for i in range(6):
        cy = hy + 3.0 * M + i * 3.6 * M
        L.box("R2_ColW_{0}".format(i), (hx + 4.0 * M, cy, HALL_H * 0.5), (1.1 * M, 1.1 * M, HALL_H), HERO)
        L.box("R2_ColE_{0}".format(i), (hx + hw - 4.0 * M, cy, HALL_H * 0.5), (1.1 * M, 1.1 * M, HALL_H), HERO)
    L.box("R2_Dais", (hx + hw * 0.5, hy + hd - 4.0 * M, 0.3 * M), (7.0 * M, 4.0 * M, 0.6 * M), HERO)
    L.box("R2_ChairPlinth", (hx + hw * 0.5, hy + hd - 3.2 * M, 0.9 * M), (1.2 * M, 1.2 * M, 0.6 * M), HERO)

    # Clerestory band: six high windows down each long wall of the hall, above
    # the colonnade. Replaces the solid east/west walls built by room().
    L.boxes = [b for b in L.boxes if not (b["name"].startswith("R2_AudienceHall_W")
                                          or b["name"].startswith("R2_AudienceHall_E"))]
    for i in range(4):
        y_a = hy + 2.0 * M + i * 6.0 * M
        y_b = y_a + 6.0 * M
        L.window_wall("R2_ClerestoryW_{0}".format(i), (hx, y_a), (hx, y_b), HALL_H, WALL_T,
                      HERO, at=0.5, width=2.4 * M, sill=5.4 * M, header=7.6 * M)
        L.window_wall("R2_ClerestoryE_{0}".format(i), (hx + hw, y_a), (hx + hw, y_b), HALL_H, WALL_T,
                      HERO, at=0.5, width=2.4 * M, sill=5.4 * M, header=7.6 * M)
    # the two doorways the room() call used to carve
    L.wall("R2_DoorW", (hx, hy), (hx, hy + 2.0 * M), HALL_H, WALL_T, HERO, door_at=0.5, door_h=3.2 * M)
    L.wall("R2_DoorE", (hx + hw, hy), (hx + hw, hy + 2.0 * M), HALL_H, WALL_T, HERO, door_at=0.5, door_h=3.2 * M)

    # R4 family corridor
    cx, cy0, cw, cd = 18 * M, 12 * M, 3.5 * M, 14 * M
    L.room("R4_FamilyCorridor", cx, cy0, cw, cd, WALL_H,
           doors=(("N", 0.5), ("W", 0.35), ("E", 0.35), ("S", 0.5)), ceiling=True)

    # R5 / R6 chambers
    L.room("R5_EvanChamber", cx - 8.0 * M, cy0 + 2.0 * M, 8.0 * M, 7.0 * M, WALL_H,
           doors=(("E", 0.35),), ceiling=True)
    L.room("R6_RaynorChamber", cx + cw, cy0 + 2.0 * M, 8.0 * M, 7.0 * M, WALL_H,
           doors=(("W", 0.35),), ceiling=True)

    # R7 steward study
    L.room("R7_Study", cx - 3.0 * M, cy0 - 8.0 * M, 9.0 * M, 8.0 * M, WALL_H,
           doors=(("N", 0.6),), ceiling=True)

    # R3 terrace
    L.box("R3_Terrace_Floor", (6.0 * M, 34 * M, -0.1 * M), (10 * M, 16 * M, 0.2 * M), FLOOR)
    L.wall("R3_Parapet_W", (1.0 * M, 26 * M), (1.0 * M, 42 * M), 1.1 * M, 0.4 * M)
    L.wall("R3_Parapet_N", (1.0 * M, 42 * M), (11 * M, 42 * M), 1.1 * M, 0.4 * M)

    # R1 canal gate and dock
    L.box("R1_Dock", (6.0 * M, 6.0 * M, -0.1 * M), (10 * M, 10 * M, 0.2 * M), FLOOR)
    L.box("R1_Water", (6.0 * M, -3.0 * M, -0.6 * M), (18 * M, 8 * M, 0.2 * M), WATER)
    L.wall("R1_GateWall", (1.0 * M, 11 * M), (11 * M, 11 * M), 4.5 * M, WALL_T,
           door_at=0.5, door_w=2.4 * M, door_h=3.4 * M)

    # R8 yard / R9 storage court
    L.box("R8_YardFloor", (30 * M, 12 * M, -0.1 * M), (14 * M, 12 * M, 0.2 * M), GROUND)
    L.wall("R8_YardWall_E", (37 * M, 6 * M), (37 * M, 18 * M), 4.0 * M, WALL_T)
    L.room("R9_StorageCourt", 24 * M, 19 * M, 12 * M, 10 * M, WALL_H,
           doors=(("S", 0.4), ("N", 0.6)))

    # R10 barracks annex
    L.room("R10_BarracksAnnex", 26 * M, 40 * M, 12 * M, 12 * M, WALL_H,
           doors=(("S", 0.5),), ceiling=True)

    # soft-walled growth points
    L.box("SoftWall_WestArch", (0.5 * M, 34 * M, 2.0 * M), (1.0 * M, 4.0 * M, 4.0 * M), HERO)
    L.box("SoftWall_NorthProcessional", (20 * M, 54.5 * M, 2.0 * M), (5.0 * M, 1.0 * M, 4.0 * M), HERO)

    # perimeter
    L.wall("Perim_S", (0.0, 0.0), (40 * M, 0.0), 8.0 * M, 0.6 * M)
    L.wall("Perim_N", (0.0, 55 * M), (40 * M, 55 * M), 8.0 * M, 0.6 * M)
    L.wall("Perim_W", (0.0, 0.0), (0.0, 55 * M), 8.0 * M, 0.6 * M)
    L.wall("Perim_E", (40 * M, 0.0), (40 * M, 55 * M), 8.0 * M, 0.6 * M)

    # -----------------------------------------------------------------
    # Story beats. Every flag here already exists in ETBWWorldFlag (C++),
    # so the interaction system and the debug HUD light up with no new code.
    # Prompts stay English for now: the Arabic HUD path is Slate-only, and the
    # Canvas prompt line is the one place Arabic still mojibakes.
    # -----------------------------------------------------------------
    interactables = [
        {"id": "WillTablet", "room": "R2_AudienceHall",
         "location": [hx + hw * 0.5, hy + hd - 3.2 * M, 1.45 * M], "size": [0.5 * M, 0.35 * M, 0.5 * M],
         "prompt": "Read the will", "examine": "The tablet names Raynor. The clay is newer than the seal.",
         "flag": "WillWasRead", "vs": "VS-02"},

        {"id": "EmptyChair", "room": "R2_AudienceHall",
         "location": [hx + hw * 0.5 + 2.4 * M, hy + hd - 4.0 * M, 1.25 * M], "size": [0.8 * M, 0.8 * M, 1.2 * M],
         "prompt": "Examine the empty chair", "examine": "Cloth laid for an heir who has not sat down.",
         "flag": "EmptyChairExamined", "vs": "VS-05"},

        {"id": "UnusedBed", "room": "R6_RaynorChamber",
         "location": [cx + cw + 5.5 * M, cy0 + 5.5 * M, 0.35 * M], "size": [2.0 * M, 1.1 * M, 0.7 * M],
         "prompt": "Examine the bed", "examine": "Slept in once, then straightened by someone in a hurry.",
         "flag": "UnusedBed", "vs": "VS-06"},

        {"id": "ScarOil", "room": "R6_RaynorChamber",
         "location": [cx + cw + 1.4 * M, cy0 + 7.6 * M, 0.95 * M], "size": [0.3 * M, 0.3 * M, 0.35 * M],
         "prompt": "Examine the oil jar", "examine": "Scar oil. Raynor stopped needing it years ago.",
         "flag": "ScarOil", "vs": "VS-06"},

        {"id": "Roster", "room": "R7_Study",
         "location": [cx + 1.2 * M, cy0 - 5.5 * M, 1.05 * M], "size": [0.6 * M, 0.45 * M, 0.1 * M],
         "prompt": "Read the guard roster", "examine": "Two names on the canal gate were scraped and rewritten.",
         "flag": "RosterAltered", "vs": "VS-09"},

        {"id": "FalseLetter", "room": "R7_Study",
         "location": [cx + 3.0 * M, cy0 - 4.2 * M, 1.05 * M], "size": [0.4 * M, 0.3 * M, 0.06 * M],
         "prompt": "Read the letter", "examine": "Orin's hand, but the pressure is wrong. Someone practised it.",
         "flag": "LetterIsFalse", "vs": "VS-09"},

        {"id": "CanalClasp", "room": "R1_Dock",
         "location": [4.0 * M, 8.4 * M, 0.25 * M], "size": [0.22 * M, 0.22 * M, 0.12 * M],
         "prompt": "Pick up the clasp", "examine": "A cloak clasp from the house guard, in the water at the gate.",
         "flag": "ClaspFound", "vs": "VS-09"},

        {"id": "WestArch", "room": "R3_Terrace",
         "location": [1.6 * M, 34 * M, 1.3 * M], "size": [0.4 * M, 1.6 * M, 2.4 * M],
         "prompt": "Look through the west arch", "examine": "The rest of the palace. Not tonight.",
         "flag": "WestArchTouched", "vs": "VS-05"},
    ]

    return {
        "name": "L_VS_Palace_EastWing",
        "spec": "docs/PALACE_WING_SPEC.md",
        "units": "cm",
        "footprint_m": [40, 55],
        "player_start": {"location": [19.5 * M, 15.0 * M, 1.2 * M], "yaw": 90.0},
        "sun": {"pitch": -14.0, "yaw": 125.0, "intensity": 6.0, "color": [1.0, 0.84, 0.67]},
        "fog": {"density": 0.035, "height_falloff": 0.15},
        "rooms_of_interest": {
            "R2_AudienceHall": [hx + hw * 0.5, hy + hd * 0.5, 1.7 * M],
            "R4_FamilyCorridor": [cx + cw * 0.5, cy0 + cd * 0.5, 1.7 * M],
            "R5_EvanChamber": [cx - 4.0 * M, cy0 + 5.5 * M, 1.7 * M],
            "R7_Study": [cx + 1.5 * M, cy0 - 4.0 * M, 1.7 * M],
            "R1_Dock": [6.0 * M, 6.0 * M, 1.7 * M],
        },
        "boxes": L.boxes,
        "interactables": interactables,
    }


if __name__ == "__main__":
    import json
    import os

    data = build()
    root = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
    out = os.path.join(root, "web", "preview", "data", "east_wing.json")
    os.makedirs(os.path.dirname(out), exist_ok=True)
    with open(out, "w", encoding="utf-8") as fh:
        json.dump(data, fh, indent=1)
    print("wrote {0}  ({1} boxes)".format(out, len(data["boxes"])))
