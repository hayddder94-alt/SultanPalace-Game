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
        # Every room built with a ceiling. A roofed box with no light source in
        # it is black in any renderer - that is not a lighting style, it is a
        # missing actor. Recording them here means lights are generated from
        # the same authority as the geometry and cannot be forgotten.
        self.ceilinged = []

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

    def stair(self, name, x, y_start, width, rise_total, run_total,
              riser=0.2 * M, direction=1, base_z=0.0, kind=WALL):
        """
        A real flight, not a ramp. Riser is 20 cm against UE's 32 cm step limit,
        so the character walks it without the movement component fighting.
        Returns the landing height and the y it ends at.
        """
        steps = max(1, int(round(rise_total / riser)))
        tread = run_total / steps
        for i in range(steps):
            h = base_z + riser * (i + 1)
            cy = y_start + direction * (tread * (i + 0.5))
            self.box("{0}_{1:02d}".format(name, i),
                     (x, cy, base_z + h * 0.5 - base_z * 0.5),
                     (width, tread, h - base_z), kind)
        return base_z + riser * steps, y_start + direction * run_total

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
            self.ceilinged.append({"room": name, "x": x, "y": y, "w": w, "d": d,
                                   "height": height, "z_base": 0.0})


# ---------------------------------------------------------------------------

def build():
    L = Layout()

    # Paved site slab under the whole footprint. Without this the canal dock is
    # an unreachable island: the walkability check proved it.
    #
    # Its top used to sit at EXACTLY z = 0, flush with every room floor. Eleven
    # overlapping surfaces then shared one plane and the GPU had no way to
    # choose between them, so large patches of the palace changed colour as the
    # camera moved - classic z-fighting, and exactly what "layer over layer" in
    # the bug report describes.
    #
    # 2 cm lower. Far below MaxStepHeight so nothing about walking changes, and
    # far above the depth buffer's ability to confuse the two.
    L.box("Site_GroundSlab", (20 * M, 27.5 * M, -0.17 * M), (40 * M, 55 * M, 0.3 * M), GROUND)

    # R2 audience hall - the hero volume
    hx, hy, hw, hd = 11 * M, 26 * M, 18 * M, 24 * M
    L.room("R2_AudienceHall", hx, hy, hw, hd, HALL_H,
           doors=(("S", (19.75 * M - hx) / hw), ("W", 0.5), ("E", 0.35)), wall_kind=HERO)
    for i in range(6):
        cy = hy + 3.0 * M + i * 3.6 * M
        L.box("R2_ColW_{0}".format(i), (hx + 4.0 * M, cy, HALL_H * 0.5), (1.1 * M, 1.1 * M, HALL_H), HERO)
        L.box("R2_ColE_{0}".format(i), (hx + hw - 4.0 * M, cy, HALL_H * 0.5), (1.1 * M, 1.1 * M, HALL_H), HERO)
    L.box("R2_Dais", (hx + hw * 0.5, hy + hd - 4.0 * M, 0.3 * M), (7.0 * M, 4.0 * M, 0.6 * M), HERO)
    # Three 20 cm treads. UE MaxStepHeight is 32 cm, so a bare 60 cm dais is a
    # wall: the player could look at the will tablet but never stand at it.
    for step_i in range(3):
        L.box("R2_DaisStep_{0}".format(step_i),
              (hx + hw * 0.5, hy + hd - 6.0 * M - step_i * 0.5 * M, (0.6 - 0.2 * step_i) * 0.5 * M),
              (5.0 * M, 0.5 * M, (0.6 - 0.2 * step_i) * M), HERO)
    L.box("R2_ChairPlinth", (hx + hw * 0.5, hy + hd - 3.2 * M, 0.9 * M), (1.2 * M, 1.2 * M, 0.6 * M), HERO)

    # Clerestory band: six high windows down each long wall of the hall, above
    # the colonnade. Replaces the solid east/west walls built by room().
    L.boxes = [b for b in L.boxes if not (b["name"].startswith("R2_AudienceHall_W")
                                          or b["name"].startswith("R2_AudienceHall_E"))]
    bay = (hd - 2.0 * M) / 4.0            # exactly fills the wall, no overshoot
    for i in range(4):
        y_a = hy + 2.0 * M + i * bay
        y_b = y_a + bay
        L.window_wall("R2_ClerestoryW_{0}".format(i), (hx, y_a), (hx, y_b), HALL_H, WALL_T,
                      HERO, at=0.5, width=2.4 * M, sill=5.4 * M, header=7.6 * M)
        L.window_wall("R2_ClerestoryE_{0}".format(i), (hx + hw, y_a), (hx + hw, y_b), HALL_H, WALL_T,
                      HERO, at=0.5, width=2.4 * M, sill=5.4 * M, header=7.6 * M)
    # the two doorways the room() call used to carve
    L.wall("R2_DoorW", (hx, hy), (hx, hy + 2.0 * M), HALL_H, WALL_T, HERO, door_at=0.5, door_h=3.2 * M)
    L.wall("R2_DoorE", (hx + hw, hy), (hx + hw, hy + 2.0 * M), HALL_H, WALL_T, HERO, door_at=0.5, door_h=3.2 * M)

    # R4 family corridor and everything that opens off it.
    #
    # Doors are placed from SHARED WORLD COORDINATES, never from per-room
    # fractions. Two rooms sharing a wall with door_at 0.35 and 0.5 look aligned
    # on paper and leave a 19 cm gap once the 38 cm capsule radius is applied -
    # the walkability check caught exactly that and sealed the study off.
    cx, cy0, cw, cd = 18 * M, 12 * M, 3.5 * M, 14 * M
    corridor_axis_x = cx + cw * 0.5          # 19.75 m, the spine
    chamber_door_y = cy0 + 5.5 * M           # 17.5 m, shared by both chambers

    def at(origin, length, world):
        """door_at fraction that puts a door centre on an absolute coordinate"""
        return max(0.08, min(0.92, (world - origin) / length))

    L.room("R4_FamilyCorridor", cx, cy0, cw, cd, WALL_H,
           doors=(("N", 0.5),
                  ("W", at(cy0, cd, chamber_door_y)),
                  ("E", at(cy0, cd, chamber_door_y)),
                  ("S", at(cx, cw, corridor_axis_x))),
           ceiling=True)

    # R5 / R6 chambers - doors on the same world line as the corridor openings
    L.room("R5_EvanChamber", cx - 8.0 * M, cy0 + 2.0 * M, 8.0 * M, 7.0 * M, WALL_H,
           doors=(("E", at(cy0 + 2.0 * M, 7.0 * M, chamber_door_y)),), ceiling=True)
    L.room("R6_RaynorChamber", cx + cw, cy0 + 2.0 * M, 8.0 * M, 7.0 * M, WALL_H,
           doors=(("W", at(cy0 + 2.0 * M, 7.0 * M, chamber_door_y)),), ceiling=True)

    # R7 steward study - north door on the corridor spine
    L.room("R7_Study", cx - 3.0 * M, cy0 - 8.0 * M, 9.0 * M, 8.0 * M, WALL_H,
           doors=(("N", at(cx - 3.0 * M, 9.0 * M, corridor_axis_x)),), ceiling=True)

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
    # The yard used to be 14 x 12 m at x 23..37, which drove it straight through
    # Raynor's chamber (6.5 x 4.0 m), the steward study and the storage court.
    # Rule 5 missed it for a spelling reason: it tested names ending "_Floor",
    # and this one is "R8_YardFloor". Now it starts east of the chamber block
    # and stops south of the court.
    L.box("R8_YardFloor", (34.5 * M, 9.9 * M, -0.1 * M), (10 * M, 11.8 * M, 0.2 * M), GROUND)
    L.wall("R8_YardWall_E", (39.5 * M, 4 * M), (39.5 * M, 15.8 * M), 4.0 * M, WALL_T)
    # Ends exactly where the hall begins: hall y0 = 26 m. It used to run to y = 29
    # and sat inside the hall's south-east corner.
    # East of Raynor's chamber (which ends at x = 29.5 m) and south of the hall.
    L.room("R9_StorageCourt", 29.8 * M, 16 * M, 10 * M, 10 * M, WALL_H,
           doors=(("S", 0.4), ("N", 0.6)))

    # R10 barracks annex
    # Starts clear of the hall's east wall at x = 29 m; it used to start at 26 m
    # and overlapped the hall - which is what buried the stair's upper treads.
    L.room("R10_BarracksAnnex", 29.5 * M, 40 * M, 10 * M, 12 * M, WALL_H,
           doors=(("S", 0.5),), ceiling=True)

    # -----------------------------------------------------------------
    # R11 upper chamber - Orin dies here (VS-01). One floor above the hall,
    # reached by a switchback stair against the hall's east wall.
    #
    # 9.4 m of rise at a 20 cm riser is 47 treads; at 50 cm each that is 23.5 m
    # of run, which only fits the 24 m hall as two opposed flights. The first
    # attempt landed the top flight at y = 30.8 while the slab started at y = 38,
    # so the stair climbed into thin air - the walkability check caught it. Every
    # dimension below is now derived, not guessed.
    # -----------------------------------------------------------------
    up_z = 9.4 * M
    flight_run = 11.75 * M
    stair_a_x = hx + hw - 2.2 * M                      # 26.8 m, hard against the east wall
    stair_b_x = stair_a_x - 3.2 * M                    # 23.6 m, the return flight

    # Start clear of R9's north wall, which sits exactly on y = hy + 3 m and had
    # swallowed the first tread - the stair was unclimbable from the ground.
    stair_a_y = hy + 4.5 * M
    mid_z, land_y = L.stair("R2_StairA", stair_a_x, stair_a_y, 2.0 * M,
                            up_z * 0.5, flight_run, direction=1, kind=HERO)
    L.box("R2_StairLanding_Mid", (stair_a_x - 1.6 * M, land_y + 0.9 * M, mid_z * 0.5),
          (5.2 * M, 1.8 * M, mid_z), HERO)
    top_z, top_y = L.stair("R2_StairB", stair_b_x, land_y + 1.8 * M, 2.0 * M,
                           up_z - mid_z, flight_run, direction=-1, base_z=mid_z, kind=HERO)

    # Upper slab: south half of the hall, stopping short of the stair well so the
    # flights stay open. Its east edge is placed to meet the top of flight B.
    slab_w = 9.5 * M
    slab_x0 = hx + 2.0 * M
    slab_d = 12.0 * M
    slab_y0 = hy
    L.box("R11_Floor", (slab_x0 + slab_w * 0.5, slab_y0 + slab_d * 0.5, up_z - 0.2 * M),
          (slab_w, slab_d, 0.4 * M), HERO)

    # Bridge from the head of the stair onto the slab - the piece that was missing.
    bridge_x0 = slab_x0 + slab_w
    bridge_w = (stair_b_x + 1.0 * M) - bridge_x0
    # Beyond the last tread, not over it. Placed at top_y + 0.75 m it sat 20 cm
    # above the final four treads and capped them: the flight became unclimbable
    # in its last metre. Flight B descends in -Y, so the deck belongs at -Y.
    L.box("R11_StairHead", (bridge_x0 + bridge_w * 0.5, top_y - 1.25 * M, up_z - 0.2 * M),
          (bridge_w, 2.5 * M, 0.4 * M), HERO)

    # Chamber walls sit on the slab. Door faces the stair head.
    door_world_y = top_y - 1.25 * M
    L.room("R11_UpperChamber", slab_x0, slab_y0, slab_w, slab_d, 3.6 * M,
           doors=(("E", max(0.1, min(0.9, (door_world_y - slab_y0) / slab_d))),),
           ceiling=True, wall_kind=HERO, floor_kind=HERO)
    for b in L.boxes:
        if b["name"].startswith("R11_UpperChamber"):
            b["center"][2] += up_z
    for c in L.ceilinged:
        if c["room"] == "R11_UpperChamber":
            c["z_base"] = up_z
    L.boxes = [b for b in L.boxes if b["name"] != "R11_UpperChamber_Floor"]

    # Orin's bed, against the west wall of the chamber
    orin_bed = [slab_x0 + 3.0 * M, slab_y0 + 6.0 * M, up_z + 0.55 * M]

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
         "flag": "WillWasRead", "vs": "VS-02", "scene": "VS02_TheWillReading"},

        {"id": "EmptyChair", "room": "R2_AudienceHall",
         "location": [hx + hw * 0.5 + 2.4 * M, hy + hd - 4.0 * M, 1.25 * M], "size": [0.8 * M, 0.8 * M, 1.2 * M],
         "prompt": "Examine the empty chair", "examine": "Cloth laid for an heir who has not sat down.",
         "flag": "EmptyChairExamined", "vs": "VS-05", "scene": "VS05_LearnTheHouse"},

        {"id": "UnusedBed", "room": "R6_RaynorChamber",
         "location": [cx + cw + 5.5 * M, cy0 + 5.5 * M, 0.35 * M], "size": [2.0 * M, 1.1 * M, 0.7 * M],
         "prompt": "Examine the bed", "examine": "Slept in once, then straightened by someone in a hurry.",
         "flag": "UnusedBed", "vs": "VS-06"},

        {"id": "ScarOil", "room": "R6_RaynorChamber",
         "location": [cx + cw + 1.4 * M, cy0 + 7.6 * M, 0.95 * M], "size": [0.3 * M, 0.3 * M, 0.35 * M],
         "prompt": "Examine the oil jar", "examine": "Scar oil. Raynor stopped needing it years ago.",
         "flag": "ScarOil", "vs": "VS-06", "scene": "VS06_TheUnusedRoom"},

        {"id": "Roster", "room": "R7_Study",
         "location": [cx + 1.2 * M, cy0 - 5.5 * M, 1.05 * M], "size": [0.6 * M, 0.45 * M, 0.1 * M],
         "prompt": "Read the guard roster", "examine": "Two names on the canal gate were scraped and rewritten.",
         "flag": "RosterAltered", "vs": "VS-10", "scene": "VS10_TheNightRoster"},

        {"id": "FalseLetter", "room": "R7_Study",
         "location": [cx + 3.0 * M, cy0 - 4.2 * M, 1.05 * M], "size": [0.4 * M, 0.3 * M, 0.06 * M],
         "prompt": "Read the letter", "examine": "Orin's hand, but the pressure is wrong. Someone practised it.",
         "flag": "LetterIsFalse", "vs": "VS-12", "scene": "VS12_WrongWax"},

        {"id": "CanalClasp", "room": "R1_Dock",
         "location": [4.0 * M, 8.4 * M, 0.25 * M], "size": [0.22 * M, 0.22 * M, 0.12 * M],
         "prompt": "Pick up the clasp", "examine": "A cloak clasp from the house guard, in the water at the gate.",
         "flag": "ClaspFound", "vs": "VS-09"},

        {"id": "OrinBedside", "room": "R11_UpperChamber",
         "location": orin_bed, "size": [2.1 * M, 1.2 * M, 0.7 * M],
         "prompt": "Orin's bedside", "examine": "Where the father asked for water, and warned about Nofan.",
         "flag": "OrinLastWords", "vs": "VS-01", "scene": "VS01_OrinLastWords"},

        {"id": "WestArch", "room": "R3_Terrace",
         "location": [1.6 * M, 34 * M, 1.3 * M], "size": [0.4 * M, 1.6 * M, 2.4 * M],
         "prompt": "Look through the west arch", "examine": "The rest of the palace. Not tonight.",
         "flag": "WestArchTouched", "vs": "VS-05"},
    ]

    # -----------------------------------------------------------------
    # The cast, placed where the script puts them.
    #
    # These are staged story characters, not AI: no behaviour trees, no
    # perception, no navigation. They stand, sit and lie where a scene needs
    # them, face a direction, and can carry a line. Enemy AI stays gated.
    # -----------------------------------------------------------------
    hall_cx = hx + hw * 0.5
    dais_y = hy + hd - 4.0 * M

    # The switchback stair is a solid mass occupying the hall's east side from
    # x = 22.6 m outward. Blocking the will-reading on the hall's geometric
    # centre put Raynor inside the mid landing and Soren inside tread 18 - they
    # rendered as limbs coming out of a staircase.
    #
    # The ceremony is centred on the CLEAR half instead. That is not a
    # compromise: a crowd standing where a staircase is, is not blocking, and
    # the camera reads a group better against an unbroken wall anyway.
    group_cx = hx + 5.75 * M

    characters = [
        # VS-01, upper chamber: the death scene
        {"id": "Orin", "name": "Orin", "pose": "lying", "room": "R11_UpperChamber",
         "location": [orin_bed[0], orin_bed[1], up_z + 0.95 * M], "yaw": 0.0,
         "vs": "VS-01", "tint": [0.620, 0.600, 0.550],
         "note": "Dying. Head toward the west wall."},
        {"id": "Raynor_VS01", "name": "Raynor", "pose": "seated", "room": "R11_UpperChamber",
         "location": [orin_bed[0] + 1.8 * M, orin_bed[1], up_z + 0.6 * M], "yaw": 180.0,
         "vs": "VS-01", "scene": "VS01_OrinLastWords", "tint": [0.340, 0.070, 0.060],
         "note": "Seated at the bedside."},
        {"id": "Nurse", "name": "Nurse", "pose": "standing", "room": "R11_UpperChamber",
         "location": [orin_bed[0] - 0.4 * M, orin_bed[1] + 2.6 * M, up_z + 0.9 * M], "yaw": 270.0,
         "vs": "VS-01", "tint": [0.550, 0.520, 0.440],
         "note": "Silent. Never speaks."},

        # VS-02, audience hall: the will reading. Six sons, in birth order,
        # arranged so the camera reads the line before it reads a face.
        {"id": "Nofan", "name": "Nofan", "pose": "standing", "room": "R2_AudienceHall",
         "location": [group_cx - 4.2 * M, dais_y - 3.0 * M, 0.9 * M], "yaw": 0.0,
         "vs": "VS-02", "scene": "VS14_NofansKindness",
         "tint": [0.420, 0.280, 0.070],
         "note": "Eldest. Closest to the dais on the left. Speaks VS-14 here at the end."},
        {"id": "Raynor", "name": "Raynor", "pose": "standing", "room": "R2_AudienceHall",
         "location": [group_cx + 4.2 * M, dais_y - 3.0 * M, 0.9 * M], "yaw": 0.0,
         "vs": "VS-02", "tint": [0.340, 0.070, 0.060],
         "note": "Named heir. Takes the seal here."},
        {"id": "Darius", "name": "Darius", "pose": "seated", "room": "R2_AudienceHall",
         "location": [group_cx - 4.8 * M, dais_y - 6.5 * M, 0.6 * M], "yaw": 15.0,
         "vs": "VS-02", "tint": [0.050, 0.070, 0.110],
         "note": "Seated. Stillness is the character."},
        {"id": "Evan", "name": "Evan", "pose": "standing", "room": "R2_AudienceHall",
         "location": [group_cx + 1.4 * M, dais_y - 7.5 * M, 0.9 * M], "yaw": 0.0,
         "vs": "VS-02", "tint": [0.130, 0.190, 0.110],
         "note": "The player watches from here in VS-02."},
        {"id": "Malik", "name": "Malik", "pose": "standing", "room": "R2_AudienceHall",
         "location": [group_cx - 1.6 * M, dais_y - 7.5 * M, 0.9 * M], "yaw": 0.0,
         "vs": "VS-02", "scene": "VS08b_MalikAtTheDoor", "tint": [0.060, 0.080, 0.160],
         "note": "Ink on his hands."},
        {"id": "Soren", "name": "Soren", "pose": "standing", "room": "R2_AudienceHall",
         "location": [group_cx + 5.0 * M, dais_y - 6.5 * M, 0.9 * M], "yaw": 340.0,
         "vs": "VS-07", "scene": "VS07_SorenWillNotLook", "tint": [0.300, 0.420, 0.520],
         "note": "Will not look at Evan."},
        {"id": "Leila", "name": "Leila", "pose": "standing", "room": "R2_AudienceHall",
         "location": [group_cx + 2.8 * M, dais_y - 1.6 * M, 0.9 * M], "yaw": 200.0,
         "vs": "VS-03", "scene": "VS03_EyesInTheHall",
         "tint": [0.720, 0.660, 0.440],
         "note": "Not blood. Unsmiling. Stands apart, not behind."},

        # The house at work - the wing has to feel inhabited between scenes
        {"id": "Yasmin", "name": "Yasmin", "pose": "standing", "room": "R8_Yard",
         "location": [34.0 * M, 10.0 * M, 0.9 * M], "yaw": 180.0,
         "vs": "VS-08", "scene": "VS08_YasminPerformedJoy", "tint": [0.450, 0.190, 0.110],
         "note": "Performed joy at the kitchen yard edge."},
        {"id": "ServantA", "name": "Servant", "pose": "standing", "room": "R2_AudienceHall",
         "location": [group_cx - 4.0 * M, hy + 6.0 * M, 0.9 * M], "yaw": 90.0,
         "vs": "VS-05", "scene": "VS05_LearnTheHouse", "tint": [0.240, 0.180, 0.120],
         "note": "Setting bowls for a celebration nobody feels."},
        {"id": "ServantB", "name": "Servant", "pose": "standing", "room": "R2_AudienceHall",
         "location": [group_cx + 3.5 * M, hy + 7.5 * M, 0.9 * M], "yaw": 270.0,
         "vs": "VS-05", "tint": [0.240, 0.180, 0.120],
         "note": "Cloth for the empty heir chair."},
        {"id": "GuardGate", "name": "House guard", "pose": "standing", "room": "R1_Dock",
         "location": [6.0 * M, 12.5 * M, 0.9 * M], "yaw": 180.0,
         "vs": "VS-09", "tint": [0.160, 0.120, 0.090],
         "note": "Canal gate watch. The roster was altered here."},
        {"id": "GuardHall", "name": "House guard", "pose": "standing", "room": "R2_AudienceHall",
         "location": [hx + 1.6 * M, hy + 2.4 * M, 0.9 * M], "yaw": 45.0,
         "vs": "VS-02", "tint": [0.160, 0.120, 0.090],
         "note": "Hall door."},
        # VS-11. He exists so the line has a mouth. There is no patrol, no
        # perception and no combat behind him - see the scene file's _note.
        {"id": "GuardCourt", "name": "House guard", "pose": "standing", "room": "R9_StorageCourt",
         "location": [34.8 * M, 20.0 * M, 0.9 * M], "yaw": 180.0,
         "vs": "VS-11", "scene": "VS11_TheManInTheCourt",
         "tint": [0.160, 0.120, 0.090],
         "note": "Storage court. Challenges Evan. Dialogue only."},
        {"id": "Boatman", "name": "Boatman", "pose": "standing", "room": "R1_Dock",
         "location": [3.0 * M, 3.5 * M, 0.9 * M], "yaw": 90.0,
         "vs": "VS-09", "scene": "VS09_TheCanalClasp", "tint": [0.170, 0.200, 0.140],
         "note": "At the dock. Saw the boat leave."},
    ]

    # -----------------------------------------------------------------
    # Practical lighting.
    #
    # The wing shipped with exactly four light actors: a sun, a sky light, an
    # atmosphere and a fog. Every roofed room therefore rendered black, and the
    # player spawns in R4, which is roofed. The browser preview had already hit
    # this and solved it by hiding ceilings and adding a headlamp; the real
    # level had no such trick, so Play showed nothing at all.
    #
    # Lights are generated from L.ceilinged, the list the geometry builder fills
    # in, so a future roofed room cannot be added without one.
    # Spacing: one lamp per 6 m of run, never closer than 1.2 m to a wall.
    # -----------------------------------------------------------------
    lights = []

    def lamp(name, x, y, z, lumens=2600.0, radius=9.0 * M, colour=(255, 196, 132)):
        lights.append({
            "name": name,
            "location": [round(x, 2), round(y, 2), round(z, 2)],
            "lumens": lumens,
            "attenuation_radius": round(radius, 2),
            "color": list(colour),
        })

    for c in L.ceilinged:
        inset = 1.2 * M
        usable_w = max(c["w"] - 2 * inset, 0.0)
        usable_d = max(c["d"] - 2 * inset, 0.0)
        nx = max(1, int(usable_w // (6.0 * M)) + 1)
        ny = max(1, int(usable_d // (6.0 * M)) + 1)
        # Hang them just under the ceiling, but never above head height + 1.6 m,
        # so a 5.5 m corridor is not lit from a point nobody can perceive.
        z = c["z_base"] + min(c["height"] - 0.5 * M, 3.4 * M)
        for i in range(nx):
            for j in range(ny):
                fx = 0.5 if nx == 1 else i / float(nx - 1)
                fy = 0.5 if ny == 1 else j / float(ny - 1)
                lamp("{0}_Lamp_{1}{2}".format(c["room"], i, j),
                     c["x"] + inset + usable_w * fx,
                     c["y"] + inset + usable_d * fy,
                     z)

    # The audience hall is not in L.ceilinged - it has clerestory windows and no
    # roof box - but 9 m of height eats daylight, and the dais is the one place
    # the camera must read faces. Four hanging lamps and a warmer pair on the dais.
    for i in range(2):
        for j in range(2):
            lamp("R2_Lamp_{0}{1}".format(i, j),
                 hx + hw * (0.3 + 0.4 * i), hy + hd * (0.28 + 0.36 * j), 5.0 * M,
                 lumens=5200.0, radius=14.0 * M)
    lamp("R2_DaisLamp_W", hx + hw * 0.5 - 3.0 * M, dais_y, 4.2 * M, 3400.0, 11.0 * M, (255, 186, 120))
    lamp("R2_DaisLamp_E", hx + hw * 0.5 + 3.0 * M, dais_y, 4.2 * M, 3400.0, 11.0 * M, (255, 186, 120))

    # The canal gate at dusk: one lamp on the quay so the clasp is findable.
    lamp("R1_QuayLamp", 6.0 * M, 10.5 * M, 3.0 * M, 2200.0, 10.0 * M, (255, 206, 150))

    return {
        "name": "L_VS_Palace_EastWing",
        "spec": "docs/PALACE_WING_SPEC.md",
        "units": "cm",
        "footprint_m": [40, 55],
        "player_start": {"location": [19.5 * M, 15.0 * M, 1.2 * M], "yaw": 90.0},
        "sun": {"pitch": -14.0, "yaw": 125.0, "intensity": 42000.0, "units": "lux",
                "color": [1.0, 0.84, 0.67]},
        "fog": {"density": 0.035, "height_falloff": 0.15},
        "rooms_of_interest": {
            "R2_AudienceHall": [hx + hw * 0.5, hy + hd * 0.5, 1.7 * M],
            "R4_FamilyCorridor": [cx + cw * 0.5, cy0 + cd * 0.5, 1.7 * M],
            "R5_EvanChamber": [cx - 4.0 * M, cy0 + 5.5 * M, 1.7 * M],
            "R7_Study": [cx + 1.5 * M, cy0 - 4.0 * M, 1.7 * M],
            "R1_Dock": [6.0 * M, 6.0 * M, 1.7 * M],
            "R11_UpperChamber": [orin_bed[0] + 2.5 * M, orin_bed[1], up_z + 1.7 * M],
        },
        "boxes": L.boxes,
        "lights": lights,
        "interactables": interactables,
        "characters": characters,
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
