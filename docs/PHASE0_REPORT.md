# PHASE 0 REPORT — STOP LINE

**Project:** The Betrayed Will / وصية الغدر  
**Date:** 2026-08-13  
**Status:** Phase 0 complete. **Phase 1 is not started.** Campaign is not in production.

---

## 1. Unreal Engine version selected

**Unreal Engine 5.7**  
Hotfix: latest 5.7.x on the day the production machine is imaged, then frozen.  
Encoded in `TheBetrayedWill.uproject` → `"EngineAssociation": "5.7"`.

## 2. Why it was selected

Not the newest. 5.8 (June 2026) is ~7 weeks old — too young for a Vertical Slice lock.  
Not 5.5 — we would start already planning an upgrade.  
Not 5.6 — runner-up; 5.7 gives the same in-editor MetaHuman path plus ~9 months of animation / StateTree / Substrate hardening.

5.7 scores on the required axes: stability, docs, plugin lag, Nanite, Lumen (software default), MetaHuman, Windows packaging, Sequencer, Control Rig. Hardware RT stays off for the slice.

Exception process to change minor version: written in `ENGINE_VERSION_LOCK.md`. Default answer is no.

## 3. Project structure

```
TheBetrayedWill.uproject
Config/DefaultEngine.ini  DefaultGame.ini  DefaultInput.ini  DefaultEditor.ini
Source/
  TheBetrayedWill.Target.cs
  TheBetrayedWillEditor.Target.cs
  TBW/
    TBW.Build.cs  TBW.h  TBW.cpp
    Public|Private / Core, Player, Combat, AI, Dialogue, Quests,
                     Interaction, Save, UI, Audio, Cinematics, World
Content/TBW/
    Core, Characters, Combat, AI, Dialogue, Quests, Interaction,
    Save, UI, Audio, Cinematics, Environment/Palace/EastWing,
    Data, Maps/Dev, Maps/VS, Materials, Development
docs/   production bible + Phase 0 lock set
tools/validate_phase0.py
```

**Implemented C++ (stubs only):** GameInstance, GameMode, PlayerController, PlayerCharacter (move/look), Identity component + data asset, WorldState flag subsystem, log categories.

**Not implemented:** combat, AI, dialogue runner, quests, save slots, Steam, seven locations, campaign.

**Live compile / editor / package:** not possible on this host (no Unreal). Structural validation: `python3 tools/validate_phase0.py`. First UBT compile is Phase 1 hour 1.

## 4. Vertical Slice script summary

Fifteen timed scenes. Opening is cinematic (Orin dies, six brothers, will, Raynor named, Leila’s unkind-eyes line, canal fade). **Control passes to Evan.** He walks the East Wing, talks (Soren + Yasmin), examines 4–5 clues, sneaks the barracks annex, fights 1–2 guards in the storage court, finds the false letter, watches the board connect: *this is the family.* Nofan’s kindness ends the slice.

Raynor is seen, never played. No Kurnah. No second district.

Full script: `VERTICAL_SLICE_SCRIPT.md`.

## 5. Estimated Vertical Slice duration

| | Time |
|---|---|
| Designed | **33:45** |
| First-time (includes one stealth fail) | **41:25** |
| Hard cap | **45:00** |

If a rehearsal exceeds 45, cut in the script’s §8 order. Do not add content if it comes in under 28 — lengthen faces, not maps.

## 6. Palace wing design

**East Ceremonial Wing** only. ~40×55 m. Rooms: canal gate, audience hall, terrace, family corridor, Evan’s chamber, Raynor’s chamber, steward study, kitchen-yard edge, storage court, barracks annex, upper chamber (cine), soft-walled west/north joints.

Two states: oil-night cine, white morning play. Modular mudbrick + glazed kit. Expansion joints exist; they are not built out.

Spec: `PALACE_WING_SPEC.md`.

## 7. Character pipeline recommendation

**Hybrid, locked.**

- MetaHuman DNA faces/bodies: Evan, Raynor, Leila, Nofan, Orin  
- Custom Neo-Babylonian clothing (no MH wardrobe)  
- Traditional modular: guards, civilians  
- Same skeleton / one pawn  
- Strand hair only if the frame allows; cards first  
- Do not enable MetaHuman plugin until first hero import  

Comparison: `CHARACTER_PIPELINE.md`.  
Leila is specified, not implemented: `LEILA_SPEC.md`.  
Identity: one pawn, two data assets: `IDENTITY_ARCHITECTURE.md`.

## 8. Asset count

Order-of-magnitude **~160** required items, mostly reusable or scratch. Hero spends: Evan, Nofan, Orin, hall light, will/seal, five clues, four sequences, footsteps. Caps in `VERTICAL_SLICE_ASSET_BUDGET.md`. If the list crosses ~200 without a cut, the slice is drifting.

## 9. Highest technical risks (slice)

1. Installing 5.8 “because it is new”  
2. Slice growing into Chapters 2–3  
3. Raynor gameplay leaking into the VS  
4. Faces that cannot carry the will reading  
5. Combat that feels like a bad prototype  
6. Saves that work in PIE and die packaged  
7. Investigation softlocks  
8. Plugin tourism  
9. Treating a greybox VS as the gate  
10. Building seven locations because empty folders exist  

Register: `PHASE0_RISK_REGISTER.md`.

## 10. Phase 1 plan

Install UE 5.7.x → compile `TBW` → create `L_Dev_Sandbox` → Enhanced Input assets → flag console cheats → Development package → write performance baselines. **3–4 weeks.** Acceptance in `PHASE1_PLAN.md`. Do not start it in this commit.

## 11. What remains intentionally unbuilt

- All of Phases 1–16 implementation work beyond the stubs above  
- Chapters 1–8 as playable products  
- Market, temple, noble, residential, Kurnah  
- Raynor gameplay  
- Combat / AI / dialogue / quest / save *systems*  
- Steamworks  
- Romance, skill trees, bows, horses, magic, GAS, open world  
- MetaHuman plugin enabled “just in case”  
- A packaged Windows exe (no engine on this host)

---

## Phase 0 acceptance checklist

- [x] Unreal version locked (5.7)  
- [x] Project skeleton exists  
- [x] Technical architecture validated structurally; live UBT deferred to Phase 1 with cause  
- [x] Vertical Slice script complete  
- [x] Vertical Slice timed 30–45 minutes  
- [x] Palace wing specification complete  
- [x] Character pipeline comparison complete + recommendation  
- [x] Asset budget complete  
- [x] Scope gate complete (Green / Yellow / Red)  
- [x] Phase 1 tasks defined  
- [x] No full campaign implementation has started  

**STOP.**
