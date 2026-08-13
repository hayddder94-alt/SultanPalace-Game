# SCOPE GATE — VERTICAL SLICE

**Project:** The Betrayed Will  
**Status:** Binding until the Phase 10 gate  
**Owner:** Producer / Director  
**How to use:** Every new idea is Green, Yellow, or Red. Red is not “later maybe in the slice.” Red is out of the slice.

---

## GREEN — allowed in the Vertical Slice

Build these. They *are* the slice.

- UE 5.8 project, TBW module, Enhanced Input  
- Evan pawn, shared identity component, Evan data  
- East Ceremonial Wing only  
- Walk, run, crouch, sprint, look, interact, examine, Focus  
- 4–5 clues, pin-board auto-connect  
- Dialogue with one real choice (Soren)  
- Simple stealth: perception, detection widget, stone, optional takedown  
- Simple melee: light, heavy, block, dodge, (parry if cheap), 1–2 sword guards  
- Quest toasts for the VS segments  
- Manual save, autosave, continue, 3 slots  
- Sequencer: Orin, will, canal fade, Nofan close  
- EN/AR subtitles  
- Settings: graphics presets, language, volume, subtitles  
- Scratch / generated VO  
- Modular palace kit + two lighting states  
- Hybrid character pipeline test on Evan (+ Nofan/Orin as able)

---

## YELLOW — allowed only if essential to a Green item

Must name which Green item dies without it.

| Yellow | Allowed if |
|---|---|
| Parry | Combat feels unfinished without it; still not a skill tree |
| Takedown | Stealth room has a behind-angle; can cut if timing slips |
| Body hide | One crate volume; not a physics toy |
| Lamp extinguish | One tagged lamp in the annex |
| Focus desaturate | Readability; not eagle vision upgrades |
| Malik two-liner | Only if the slice is *under* time |
| MetaHuman plugin | When importing a hero face, not before |
| Niagara dust | If the hall looks dead without it; one system |
| Water plugin | **No** — a plane is enough |
| Control Rig IK | Foot IK only if sliding is embarrassing |
| CommonUI | Only if UMG blocks us (unlikely) |
| StateTree | Phase 4; BT fallback if it taxes the slice |
| Arabic font final | Placeholder RTL is acceptable if licensed font is late |
| Extra servant bark | Flavor only after Green is fun |

Yellow requires a one-line note in the weekly review. Two Yellows that do not serve Green are cut.

---

## RED — forbidden during the Vertical Slice

Do not prototype. Do not “just see.” Delete if it appears.

- Raynor gameplay / identity swap as a toy  
- Kurnah, marsh, reed boat level  
- Market, temple, noble, residential as playable maps  
- Second city, open world, world partition landscape  
- Horses, chariots, boats as vehicles  
- Bows, spears-as-player-weapons, magic  
- Skill trees, XP, loot, rarity, shops, crafting  
- Inventory beyond: dagger, clasp, nothing else heavy  
- Romance meter, gifts, playable Leila  
- Relationship system implementation (flags in data are enough to *define*)  
- Steamworks, overlay, achievements, cloud  
- Multiplayer, networking, GAS  
- Procedural generation, PCG worlds, crowds  
- Photo mode, NG+, recap editor  
- Full 18-segment quest authoring  
- Full save of every pot  
- Dialogue editor application  
- Hazar on-screen  
- Poison-Orin thread  
- Chapter 2 breakfast, Chapter 3 grain dock  
- Any plugin enabled “because it exists”

---

## Evaluation stamp (copy into PRs)

```
Feature:
Serves Green item:
Color: GREEN / YELLOW / RED
If Yellow, what breaks without it:
If Red, close this PR.
```

A feature without a stamp is Red.

---

## After the slice

Passing Phase 10 does **not** turn Red into Green. Post-VS, Red items are re-evaluated against `SCOPE_CONTROL.md`. Many stay dead forever (open world, GAS, romance, Steam-in-the-slice, bows).
