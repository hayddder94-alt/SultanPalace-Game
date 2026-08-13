# PRODUCTION ROADMAP

**Project:** The Betrayed Will / وصية الغدر  
**Status:** Locked for planning  
**Team model:** Small independent / AI-assisted (treat estimates as **1–3 full-time equivalents**, with AI accelerating art, boilerplate, and iteration — not replacing direction)  
**Calendar target:** **18–24 months** to a stable Windows Steam 1.0  
**Hard gate:** Phase 10 Vertical Slice. Full production does not start if the slice fails acceptance.

Estimates are in **person-weeks** (pw) and **calendar weeks** (cw) assuming a 2-person-equivalent core. They overlap on purpose. They are not a promise; they are a planning surface.

---

## 0. How to read this roadmap

Each phase lists:

- Objectives
- Deliverables
- Dependencies
- Risks
- Estimated effort
- Acceptance criteria

A phase can start when its **dependencies** are met, not when the previous number is pretty. Several systems phases (2–8) overlap after Phase 1.

**Kill rule:** If a phase’s acceptance is failed twice, cut scope using `SCOPE_CONTROL.md` before retrying.

---

## PHASE 0 — Pre-production

**Calendar:** 4–6 weeks  
**Effort:** 8–12 pw

### Objectives

- Lock the documents in `/docs` as the production bible.
- Lock Unreal version, source control LFS policy, naming, and the MetaHuman-or-not face decision.
- Lock the Vertical Slice script and shot list.
- Build a reference board (lighting, faces, palace materials) that is legal to use.
- Name the team seats even if one person sits in three chairs.

### Deliverables

- This documentation set, signed
- `00_ASSUMPTIONS_AND_GAPS.md` unchanged or formally revised
- Engine version decision recorded
- VS script (player-facing, timed)
- Art / camera / audio reference (non-copyrighted)
- Empty UE project with folder skeleton and `.gitignore` for UE
- Risk register v1

### Dependencies

- This planning phase

### Risks

- Treating pre-production as “more documents forever”
- Choosing MetaHuman and then discovering four hero faces are a trap
- Unreal version churn

### Acceptance

- [ ] A new reader can pitch the game in one minute
- [ ] VS script timed to 30–45 minutes
- [ ] Engine version written in the project README
- [ ] No open-world or RPG language remaining in official docs

---

## PHASE 1 — Technical foundation

**Calendar:** 3–4 weeks  
**Effort:** 6–8 pw

### Objectives

- A clean UE5 project that boots to a persistent test map.
- Module `TBW`, Enhanced Input, GameplayTags, log categories, developer cheats.
- CI-or-at-least-a-one-button package script for Windows Development.
- Scalability skeleton and a settings save.

### Deliverables

- `TBW` module compiling
- `L_Dev_Sandbox`
- Input contexts
- `UTBWGameInstance`, `UTBWWorldStateSubsystem` (empty flags)
- Console cheats stub
- Build script + version file
- Folder structure from Technical Architecture

### Dependencies

- Phase 0 engine lock

### Risks

- Plugin tourism
- Spending the phase on a custom framework

### Acceptance

- [ ] Package a Development build, run it on a second machine without the editor
- [ ] PIE into the sandbox in < 30 s on the dev PC
- [ ] A flag can be set from console and logged
- [ ] No marketplace pack is a hard dependency

---

## PHASE 2 — Player controller

**Calendar:** 3–4 weeks  
**Effort:** 6–10 pw

### Objectives

- Evan-feel third-person controller that is already a pleasure to walk with.
- Camera, crouch, sprint, mantle, interact trace.
- Placeholder palace corridors feel good.

### Deliverables

- `ATBWPlayerCharacter` + controller + camera
- `BP_TBW_Evan` with temp mesh
- Interact prompt
- Footstep hooks
- A walkable grey palace wing

### Dependencies

- Phase 1

### Risks

- Over-tuning before combat exists
- Jump/parkour creep

### Acceptance

- [ ] 5 minutes of walking feels intentional
- [ ] Crouch and sprint are readable
- [ ] Interact finds a door and a clue dummy
- [ ] Gamepad and KBM both work

---

## PHASE 3 — Combat

**Calendar:** 4–6 weeks  
**Effort:** 8–12 pw

### Objectives

- The full verb set vs. a dummy and vs. one sword AI (can be scripted).
- Juice: hit stop, block, parry window, death.
- Tuning document with Story/Standard/Hard numbers.

### Deliverables

- `UTBWCombatComponent`, attack data assets
- Dagger + sickle-sword placeholders
- Dummy + 1 enemy BP
- Health / stamina
- Debug damage overlay (dev)

### Dependencies

- Phase 2

### Risks

- Building GAS “because Unreal has it”
- 12-hit combos
- Animation poverty making combat feel like a prototype forever

### Acceptance

- [ ] Light / heavy / block / dodge / parry / finisher all function
- [ ] A Standard fight vs. 1 enemy is winnable and losable
- [ ] No ability graph required to add a new attack datum

---

## PHASE 4 — AI

**Calendar:** 4–5 weeks (overlaps late Phase 3)  
**Effort:** 8–10 pw

### Objectives

- Guard StateTree with the locked state list.
- Perception readable by a player without debug.
- Combat AI token for 1v2.

### Deliverables

- Guard + combat StateTrees (or BT fallback)
- Patrol spline, restricted / hide / light volumes
- `UTBWAlertSubsystem`
- Detection widget hooked to awareness

### Dependencies

- Phase 2; Phase 3 for combat handoff

### Risks

- StateTree learning tax
- Perception false positives that make stealth hateable

### Acceptance

- [ ] The ten AI test cases in `AI_DESIGN.md` pass on the sandbox annex
- [ ] No mission-fail-on-seen in the sandbox

---

## PHASE 5 — Interaction

**Calendar:** 2–3 weeks  
**Effort:** 4–6 pw

### Objectives

- One interact channel that covers doors, clues, lamps, bodies, pickups.
- Authored mantle and simple key flags.

### Deliverables

- `ITBWInteractable` implementers
- Door / key / lamp / hide-body
- Examine turntable v1

### Dependencies

- Phase 2

### Risks

- Interaction framework abstraction for its own sake

### Acceptance

- [ ] A locked door opens after a flag
- [ ] A body can be hidden and stays hidden after PIE save (even if full save is later)
- [ ] Prompts are single-button and localized-ready

---

## PHASE 6 — Dialogue

**Calendar:** 3–4 weeks  
**Effort:** 6–8 pw

### Objectives

- Data-driven conversations with conditions, choices, consequences, subtitles EN/AR layout.
- Conversation camera rig.

### Deliverables

- `UTBWDialogueData` + subsystem + widget
- 3 sample conversations (Orin, Soren, Leila stub)
- String tables
- Skip / continue

### Dependencies

- Phase 2, Phase 5

### Risks

- Building a dialogue editor product
- Visual-novel UI that fights the cinematic pillar

### Acceptance

- [ ] A choice sets a flag that a second conversation reads
- [ ] Arabic RTL choice rows do not break
- [ ] Missing VO still shows subtitles and yields

---

## PHASE 7 — Quest system

**Calendar:** 3–4 weeks  
**Effort:** 6–8 pw

### Objectives

- Segments, objectives, listeners, toasts, journal one-liners.
- Softlock fallback pointer support.

### Deliverables

- `UTBWQuestSubsystem` + data assets for VS segments
- Objective triggers
- Pause Story tab
- `tbw.Quest.SkipTo`

### Dependencies

- Phase 1 flags, Phase 6 nice-to-have

### Risks

- Generic MMO quest log

### Acceptance

- [ ] Completing A starts B
- [ ] Reload (even a temp save) restores objective UI
- [ ] SkipTo is usable by QA

---

## PHASE 8 — Save system

**Calendar:** 2–3 weeks  
**Effort:** 5–7 pw

### Objectives

- Manual, autosave, continue, 3 slots, settings persistence, versioning, crash-safe write.
- Survives editor restart **and** packaged restart.

### Deliverables

- `UTBWSaveGame` + subsystem
- Slot UI
- Backup rotation
- Migrator stub
- No-save volumes

### Dependencies

- Phase 1; wants 2, 5, 7 to have something worth saving

### Risks

- Serializing the universe
- Slots that work in PIE and die in Shipping

### Acceptance

- [ ] Cold boot Continue restores transform, flags, clues, quest, identity, inventory
- [ ] Kill the process mid-autosave; backup still loads
- [ ] Incompatible version refuses and does not wipe

---

## PHASE 9 — First playable level

**Calendar:** 4–6 weeks  
**Effort:** 10–14 pw

### Objectives

- A real palace wing, not a sandbox: walk, talk, sneak, fight, save.
- Grey-to-first-art pass. Lighting direction planted.

### Deliverables

- `L_VS_Palace` (or `L_Palace_P` wing subset)
- Patrols, 2 conversations, 4 clues, 1 fight, 1 restricted annex
- Day and oil-night lighting scenarios
- Temp music beds

### Dependencies

- Phases 2–8 functionally present (even if ugly)

### Risks

- Building the whole palace instead of one wing
- Art that cannot be replaced (wrong scale, unnamed megascans)

### Acceptance

- [ ] A designer can play 15 minutes without console cheats
- [ ] Navmesh and streaming do not hitch visibly
- [ ] The space photographs like Babylon, not a medieval stone kit

---

## PHASE 10 — Vertical Slice

**Calendar:** 6–8 weeks  
**Effort:** 16–24 pw

### Objectives

- 30–45 minutes at **intended final quality** for that chunk.
- The slice is the hiring reel, the wishlist reel, and the go/no-go.

### Deliverables

- Playable VS build (packaged Windows)
- Opening cinematic (shortened will reading)
- Evan in the palace
- Exploration, NPC talk, investigation, dialogue, combat, stealth, AI, quest, save/load, a cinematic transition, mission complete
- Known-issues list of 1 page
- Trailer capture possible from this build

Full VS definition: § Vertical Slice in this file and `GAME_DESIGN_DOCUMENT.md`.

### Dependencies

- Phase 9 accepted

### Risks

- “Representative quality” slipping into greybox + promises
- Scope of VS growing into Chapters 1–3

### Acceptance (gate)

- [ ] New player, no designer present, finishes in 30–45 min
- [ ] Save / quit / continue works on a packaged build
- [ ] Both pillars of feel (walk + one fight + one stealth room) are fun
- [ ] Faces and lighting look like the shipping bible
- [ ] Crash rate: zero in a 10-run internal playtest
- [ ] Scope Control reviewed: nothing extra sneaked in

**If this gate fails: do not open Phase 11.** Iterate or cut.

---

## PHASE 11 — Full game production

**Calendar:** 20–28 weeks  
**Effort:** 50–80 pw

### Objectives

- Build remaining locations and all 18 spine segments.
- Identity swap and Raynor feel.
- Remaining hero cinematics (betrayal, death, hall).
- Content complete at the end: all segments playable, temp permitted on secondary art.

### Deliverables

- All maps in World Design (noble district optional)
- MQ-P0 through MQ-08b playable
- Raynor identity + Ch 7–8 combat
- CIN-08, CIN-14, CIN-19 blocked and timed
- Flag list complete
- VO scratch for all main conversations
- Localization string freeze candidate

### Dependencies

- Phase 10 **passed**

### Risks

- Second-half game (Raynor) less fun than Evan
- New locations that are used once
- Content complete declared while Ch 8 is a hallway

### Acceptance

- [ ] A full playthrough is possible in 6–8 hours without cheats
- [ ] Identity swap is stable
- [ ] Three ending states reachable
- [ ] No blocker softlock on the spine
- [ ] Cut list applied if over time (see Scope Control)

Work in chapter slices: finish Ch 2 to a playable, then 3, etc. Do not “systems all chapters at 40%.”

---

## PHASE 12 — Polish

**Calendar:** 6–8 weeks  
**Effort:** 16–24 pw

### Objectives

- Faces, cameras, mix, animation, juice, conversation timing.
- Replace the worst temps.
- Paid VO pass if budget exists; otherwise best available on the four principals.

### Deliverables

- Cinematic polish on the four hero spends
- Animation pass Evan / Raynor / guards
- Audio mix
- UI final art
- Tutorial and subtitle pass

### Dependencies

- Phase 11 content complete

### Risks

- Polish as endless art
- VO arriving so late it breaks lip and timing

### Acceptance

- [ ] No mannequin in a hero shot
- [ ] No temp music in CIN-02/08/14/19
- [ ] A third-party friend says “this feels finished” on a 40-minute sample

---

## PHASE 13 — Optimization

**Calendar:** 4–6 weeks (overlaps late 12)  
**Effort:** 10–16 pw

### Objectives

- 1080p High ≈ 60 on the reference mid/high PC; a Low preset that is playable on the minimum spec (define in Steam plan).
- Hitch hunt. Texture streaming. AI sleep. Lighting cost.

### Deliverables

- Scalability tested
- Profiling notes
- Min spec written from data, not hope
- Shader permutation sanity

### Dependencies

- Content mostly unchanging

### Risks

- Lumen-only lighting that cannot scale
- Optimizing before the slice taught us the real cost (this is why VS exists)

### Acceptance

- [ ] Benchmark level (palace hall + market + Kurnah) hits targets on two hardware classes
- [ ] No hitch > 100 ms on authored transitions
- [ ] Low preset disables Lumen and remains readable

---

## PHASE 14 — QA

**Calendar:** 6–8 weeks (overlaps 12–15)  
**Effort:** 16–24 pw (includes external playtesters if any)

### Objectives

- Spine completion on fresh saves, all difficulties.
- Softlock hunt, save hunt, localization hunt, input hunt, crash hunt.
- Accessibility pass.

See `QA_PLAN.md`.

### Deliverables

- Test case runs (pass/fail)
- Crash database
- Balance notes
- Certification-style Windows checklist

### Dependencies

- Playable complete build (start on Phase 11 end, intensify after 12)

### Risks

- QA as “the intern plays it”
- Fixing by adding systems

### Acceptance

- [ ] Three unaffiliated playthroughs finish the spine
- [ ] Zero blocker, zero save-destroyer
- [ ] Known C-bugs documented, not forgotten

---

## PHASE 15 — Windows release build

**Calendar:** 2–3 weeks  
**Effort:** 4–6 pw

### Objectives

- Shipping configuration, splash, legal, credits, versioning, crash folder, install size sanity.
- A build that can be handed to Steam without shame.

### Deliverables

- Shipping exe + pak
- `Credits`
- EULA / third-party notices
- Version stamped
- Clean first-boot path

### Dependencies

- Phase 14 blocker-free

### Risks

- Debug cheats left on
- Debug maps cooked in

### Acceptance

- [ ] No console, no `tbw.` cheats, no `L_Dev_*` in Shipping
- [ ] Clean install on a fresh Windows machine
- [ ] Continue works; settings persist; Alt+Enter survives

---

## PHASE 16 — Steam preparation

**Calendar:** 3–4 weeks (page work can start at Phase 10)  
**Effort:** 6–10 pw

### Objectives

- Steamworks integration as planned: achievements, cloud, overlay, input, depot, page, recap/trailer, tags, Arabic+English store text.
- Do not implement Steamworks before Phase 15 is in sight.

See `STEAM_RELEASE_PLAN.md`.

### Deliverables

- App depot
- 20–30 achievements wired to flags
- Cloud saves verified
- Store page ready
- Launch checklist

### Dependencies

- Phase 15; store creative can start after Phase 10

### Risks

- Early Steamworks contaminating the project
- Wishlist campaign with non-representative footage

### Acceptance

- [ ] A Steam build downloads, boots, clouds a save, unlocks a test achievement, and overlays
- [ ] Store page does not promise an open world

---

## Overlap diagram (calendar, indicative 20-month plan)

```
M1    P0
M2    P1 -------- P2
M3         P2 ---- P3
M4              P3 -- P4 -- P5
M5                   P6 -- P7 -- P8
M6                        P9
M7                        P9 / P10
M8                             P10   *** GATE ***
M9-15                          P11 (chapter by chapter)
M16-17                         P12 -- P13
M18-19                         P14 -- P15
M20                            P16 launch window
```

A 24-month plan is the same with more air in P11 and P12. A 14-month plan is a lie unless the team is larger or the noble district, Hazar, poison thread, and optional quests are pre-cut.

## Vertical Slice (definition for the gate)

**Length:** 30–45 minutes  
**Identity:** Evan (plus optional shortened Raynor prologue cinematic)  
**Space:** Palace hero wing + annex + canal edge  
**Must include (brief, locked):**

- Opening cinematic (will reading, shortened is OK)
- Evan
- Palace environment
- Exploration
- NPC interaction
- Investigation
- Dialogue
- Basic combat
- Basic stealth
- Enemy AI
- Quest progression
- Save / load
- Cinematic transition
- Mission completion

**Must represent final quality** in lighting, camera, control feel, and two hero faces (Evan + one of Orin / Soren / Leila).

**Must not include:** Kurnah, identity swap, Darius boss, skill trees, a second district “because we can.”

## Staffing suggestion (seats, not headcount)

| Seat | Critical phases |
|---|---|
| Director / Narrative | 0, 10, 11, 12 |
| Technical | 1–8, 13, 15 |
| Gameplay | 2–7, 10, 11 |
| World / lighting | 9–13 |
| Character / cine | 10–12 |
| Audio | 10, 12 |
| Producer / QA / Steam | 0, 10, 14–16 |

AI assistance is assumed in boilerplate C++, placeholder art, greybox dressing, scratch VO, and documentation. It is not assumed in taste.

## Milestone names for external use

| Name | Internal |
|---|---|
| Foundation | End of Phase 1 |
| First Feel | End of Phase 3 |
| First Hour Grey | End of Phase 9 |
| Vertical Slice | End of Phase 10 |
| Content Complete | End of Phase 11 |
| RC1 | End of Phase 14 |
| Gold / Steam | End of Phase 16 |
