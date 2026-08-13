# PALACE WING SPEC — EAST CEREMONIAL WING

**Project:** The Betrayed Will  
**Map:** `L_VS_Palace`  
**Status:** Locked for Vertical Slice  
**Law:** This is the **only** environment built for the slice. It is modular so later wings bolt on. It is not the palace.

---

## 1. Why this wing

The East Ceremonial Wing holds every room the slice script needs:

- ritual (audience hall)
- family privacy (corridor, two chambers)
- paper (steward study)
- work and violence (kitchen-yard edge, storage court)
- restriction (barracks annex)
- water (canal gate)

The west residential mass, the north temple processional, the undercroft spine, and the upper harem/archive are **soft-walled**. Their doorways exist as geometry so the wing can grow. They are not playable.

---

## 2. Plan (schematic, not to scale)

```
                         [ UPPER CHAMBER ]   cine only / later empty
                                |
                         stair (locked after VS-03)
                                |
 [TERRACE]----[ AUDIENCE HALL ]----[ WEST ARCH ]  soft-wall
     |                |
     |         [ FAMILY CORRIDOR ]
     |            /        \
     |     [EVAN]          [RAYNOR]     investigation
     |            \        /
     |           [ STUDY ]              letter
     |                |
 [CANAL GATE]   [ KITCHEN YARD EDGE ]
     dock              |
                [ STORAGE COURT ]       combat
                       |
                [ BARRACKS ANNEX ]      stealth / restricted
```

Footprint target: **~40 × 55 m** playable, two floors only above the hall (upper chamber).  
Walk time hall → canal: ≤ 25 s. Hall → annex: ≤ 40 s. No mount, no load inside the wing.

---

## 3. Rooms

| ID | Room | Function | Play / cine | Hero art? |
|---|---|---|---|---|
| R1 | Canal gate + dock | Entrance, Clue C | Play | Medium (water, light) |
| R2 | Audience hall | Will, empty chair, Nofan close | Cine + play | **Hero** |
| R3 | Terrace | VS-03 only | Cine, later look | Medium |
| R4 | Family corridor | Circulation, Soren | Play | Medium |
| R5 | Evan’s chamber | Spawn | Play | Low-medium |
| R6 | Raynor’s chamber | Clues A/B | Play | **Hero props** |
| R7 | Steward study | Clue E | Play | Medium |
| R8 | Kitchen yard edge | Yasmin | Play | Low (facade, not a kitchen sim) |
| R9 | Storage court | Combat | Play | Low, clean nav |
| R10 | Barracks annex | Stealth, Clue D | Play | Medium |
| R11 | Upper chamber | Orin’s death | Cine | **Hero** (small room) |
| R12 | West arch / north door | Future bolts | Soft-wall | Cheap dressed door |

That is the entire VS space.

---

## 4. Required spatial features (from the Phase 0 brief)

| Brief item | Room |
|---|---|
| Entrance | R1 canal gate |
| Main hall | R2 |
| Private corridor | R4 |
| Study | R7 |
| Courtyard | R9 (combat) + R8 (social edge) |
| Restricted area | R10 |
| Investigation location | R6 (primary), R7 (second) |
| Stealth route | Servant run R8 → R9 → R10, plus one lamp and a crate alley |
| Combat encounter | R9 |

---

## 5. Story states (two only in the slice)

| State | When | Light | Occupancy |
|---|---|---|---|
| `P_DyingNight` | VS-01–03 | Oil, cool lattice, river | Few bodies, ritual |
| `P_NamedMorning` | VS-04–14 | White morning, heat | Performed joy, extra cloth, canal bright |

Same meshes. Different lighting scenario, different dress sublevel, different NPC spawns.

Later game will add `P_MourningEvan`, `P_ReturnedNight`, `P_LastHall` on **this same hall**. That is why the hall is modular and must not be a cinematic-only set.

---

## 6. Modular kit

Build a small Babylonian kit, not unique meshes per wall.

| Family | Pieces (order of) | Notes |
|---|---|---|
| Mudbrick wall | 8 | 2/4/8 m, corner, arch |
| Glazed ceremonial | 6 | Hall only: lion-frieze, blue field, gold trim |
| Floor | 4 | Brick, rug, glazed, wet stone |
| Column | 2 | Hall, corridor |
| Door / lattice | 5 | Open, locked, soft-wall, terrace, cell-style annex |
| Trim / bitumen | 4 | |
| Water | 2 | Canal piece, dock step |
| Dressing | 20 | Jars, lamps, chests, bowls, cloth |

Naming: `SM_Pal_Wall_4m_A`, `SM_Pal_Door_Lattice_B`.  
Master materials: mudbrick, glazed, bronze, linen, water. Instances only after that.

No medieval crenellation. No Islamic geometric palace kit. No 1:1 Ishtar Gate.

---

## 7. Lighting

- Two baked/scenario setups, not a day/night cycle.
- Hall: one hero key (clerestory), bounce, practical oil (night).
- Annex: cheap, readable stealth light volumes (`TBW.Light.Lit` / `.Shadow`).
- Canal morning: sun disk, heat, water sparkle without a custom water sim.
- Movable shadow casters: keep few. Practicals first.

---

## 8. Navigation, volumes, streaming

- Single navmesh for playable floor.
- Volumes: restricted (annex), hide-body (crate alley), light × several, soft-wall × west/north, no-save (cine), checkpoint × 6, music, location.
- Streaming: one persistent. Optional dress sublevels `SL_VS_NightDress`, `SL_VS_MorningDress`. No world partition required.

---

## 9. Expansion joints (do not build now)

| Joint | Later destination |
|---|---|
| West arch | Family residences / later chapters |
| North door | Processional to temple |
| Stair past upper chamber | Roof / unused |
| Canal east | Residential connector (Ch 2) |
| Annex south | Training court (Ch 7 Darius) |

Each joint is a real doorway with a blocker volume and a dressed view, so adding a wing is a streaming add, not a rebuild.

---

## 10. Performance budget for this wing (1080p High, target 60)

| | Cap |
|---|---|
| Visible nanite instances | Comfortable; no unique 8k hero walls |
| Shadow-casting movables | ≤ 6 in the hall, ≤ 2 in annex |
| Simultaneous AI | ≤ 4 |
| Overdraw | Watch glazed + foliage (there is almost no foliage) |
| Water | Plane + material, not a lake actor |

Baselines are captured in Phase 1 on `L_Dev_Sandbox`, then again when the wing is grey, then at VS.

---

## 11. What we will not build in this map

- A second courtyard “for variety”
- A throne of the King
- A full kitchen sim
- A museum wing
- Rooftop parkour
- A garden maze
- Any space used only once that is not R11 (Orin)

---

## 12. Greybox acceptance (Phase 9)

- A stranger can walk the script path without falling out
- Soft-walls are obvious
- Combat court is readable
- Annex has a stealth path and a fail path
- Hall photographs as a hall, not a warehouse
- Scale: Evan’s walk from bed to clasp < 90 seconds if he does not stop
