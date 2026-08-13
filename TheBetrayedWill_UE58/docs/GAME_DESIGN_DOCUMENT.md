# GAME DESIGN DOCUMENT

**Title:** The Betrayed Will  
**Arabic title:** وصية الغدر  
**Project code:** TBW  
**Genre:** Cinematic third-person story-driven action adventure  
**Setting:** Fictionalized Neo-Babylonian city (Ancient Mesopotamia)  
**Platform:** Windows PC  
**Distribution:** Steam  
**Engine:** Unreal Engine 5 (minor version locked in Phase 0)  
**Target playtime:** 6–8 hours  
**Scope class:** Small-to-medium independent / AA-style  
**Document status:** Production planning — not yet implementation-approved  
**Companion documents:** All files in `/docs`

---

## 1. One-sentence pitch

After a dying patriarch names the wrong son, you investigate a family conspiracy as the loyal brother who will not survive it — then finish the story as the imprisoned heir who must decide what the will was actually worth.

## 2. One-page pitch

Orin is dead. His six sons stand in the audience hall while a temple witness reads the will. The house, the eastern trade charter, and the irrigation concession go to **Raynor**, not to the eldest, **Nofan**.

The player spends a short prologue in Raynor’s body — a last walk with a dying father, a public naming, a private moment with **Leila**, the temple-born woman betrothed to him. Then Raynor vanishes.

The player becomes **Evan**, the overlooked son. He walks the same palace the morning after and feels the air change. Servants lie. Ledgers do not match. A brother smiles too quickly. Evan investigates the house, the market, the temple, and the noble district. He is betrayed by the family, left for dead, and believed drowned.

He survives. He finds Raynor alive in a fortress that the house was never supposed to still own. He frees him and dies holding the gate.

The player becomes Raynor. The second half is not a new game. It is the same city, the same rooms, the same brothers — now enemies or wreckage. Raynor’s revenge is a search for the full truth: a second tablet, a court patron, and a family that cannot be put back together.

The game is short, authored, and complete. It sells on story, faces, atmosphere, and a tight loop of explore → investigate → infiltrate → confront.

## 3. Fantasy to protect

Every feature must protect one of these fantasies:

1. I am inside a real family that is eating itself.
2. I am clever enough to catch a lie in a beautiful room.
3. I am dangerous enough to walk into a place that wants me dead.
4. I am watching a tragedy that could not have ended cleanly.

If a feature does not serve one of those four, it is cut.

## 4. Design pillars (priority order)

1. **Story quality** — every mission advances character or truth.
2. **Character relationships** — six brothers, one woman, one dead father.
3. **Cinematic presentation** — camera, lighting, faces, silence.
4. **Atmosphere** — heat, dust, water, incense, glazed brick, night oil-light.
5. **Responsive gameplay** — controls feel good in the first minute.
6. **Pacing** — no padding, no fetch, no open-world commute.
7. **Environmental storytelling** — rooms change meaning when you return.
8. **Vertical slice quality** — one 30–45 minute chunk at ship quality.
9. **Technical stability** — a crash-safe Windows build is a feature.
10. **Commercial feasibility** — a complete 6–8 hour product a small team can finish.

## 5. Anti-pillars

We will not chase: world size, NPC count, procedural content, huge inventories, weapon collections, skill trees, multiplayer, live service, side-quest bloat, or custom engine systems Unreal already provides.

## 6. Target audience

| Attribute | Target |
|---|---|
| Core | Players of story-driven third-person games who will finish a 7-hour campaign |
| Tone tolerance | Mature themes: betrayal, murder, family cruelty, political violence |
| Reference appetite | *A Plague Tale*, *The Last of Us* (structure, not scale), *Hellblade* (intensity), *Assassin’s Creed Origins/Mirage* (place, not systems), *God of War 2018* (camera and melee readability), *Dishonored* (authored spaces) |
| Language | English and Arabic UI/subtitles from day one |
| Platform habit | Steam, keyboard/mouse and gamepad |
| Not for | Open-world collectors, live-service players, competitive multiplayer players, historical-simulation purists |

Age rating target: **Mature / PEGI 18 / ESRB M** for violence and dark themes. No sexual content requirement. No gratuitous gore system.

## 7. Player fantasy by act

| Act | Body | Fantasy |
|---|---|---|
| Prologue | Raynor | I am the chosen son, and I do not yet know it is a death sentence. |
| Act I (Ch 1–3) | Evan | I can see the crack in the house before anyone else admits it. |
| Act II (Ch 4–6) | Evan | I was discarded. I am still the only person coming for him. |
| Act III (Ch 7–8) | Raynor | I will finish what Evan died to start, and I will look my brothers in the face. |

## 8. Core gameplay loop

The loop is the same for both playable characters. The **mix** changes.

```
Enter authored space
        ↓
Explore / read the room
        ↓
Investigate (objects, people, evidence)
        ↓
Choose approach: talk / sneak / fight
        ↓
Overcome a gate (lock, guard, brother, truth)
        ↓
Story state changes the same space
        ↓
Short cinematic or conversation
        ↓
Next objective
```

**Evan mix:** 50% explore/investigate, 25% stealth, 15% dialogue pressure, 10% combat.  
**Raynor mix:** 35% combat, 25% stealth, 20% explore, 20% confrontation dialogue.

Average time between story beats: 8–12 minutes.  
Average time between a memorable authored moment: 15–20 minutes.

## 9. Camera and feel

- Over-the-shoulder third person.
- Cinematic lock during conversations (shot / reverse shot, not always a full-screen talking head).
- Exploration camera slightly wider than combat camera.
- No lock-on required for a single enemy; soft lock for 2–3.
- No photo-mode requirement.
- Cover is contextual and light (a short lean), not a Gears cover shooter.

Feel words: **weight, heat, closeness, silence, sudden violence**.

Evan moves a fraction lighter and quieter. Raynor moves with more weight and a longer melee reach. Same input map.

## 10. Chapter map and time budget

Total authored playtime target: **7 hours 15 minutes** median.  
Optional content cap: **35 minutes**.  
Completionist ceiling: **under 8 hours 30 minutes**.

| Block | Playable | Median | Role |
|---|---|---|---|
| Prologue — The Last Night | Raynor → cinematic | 18 min | Meet Orin, Leila, the will |
| Ch 1 — The Named Son | Evan | 40 min | Tension, first cracks |
| Ch 2 — The Empty Seat | Evan | 45 min | Disappearance, first real investigation |
| Ch 3 — The House Lies | Evan | 50 min | Evidence, temple, market |
| Ch 4 — The Canal | Evan | 40 min | Betrayal, believed dead |
| Ch 5 — The Man Who Drowned | Evan | 45 min | Survival, discovery of the fortress |
| Ch 6 — The Gate | Evan → Raynor | 50 min | Infiltration, rescue, Evan’s death |
| Ch 7 — The Returned | Raynor | 70 min | Revenge begins, city re-enters changed |
| Ch 8 — The Second Tablet | Raynor | 75 min | Revelations, confrontations, ending |
| Optional objectives (distributed) | both | 35 min | Extra clues, one optional confrontation |

Details live in `QUEST_STRUCTURE.md` and `STORY_BIBLE.md`.

## 11. World, in one paragraph

Seven authored locations, interconnected by short authored transitions (streets, a canal boat, a service corridor), never by an open map. Locations are reused in at least two story states. The palace is the hero space and appears in six of nine blocks. See `WORLD_DESIGN.md`.

## 12. Systems, in one paragraph

Four pillars only: exploration, investigation, focused melee, simple stealth. Progression is story gates, four ability unlocks, and three weapon upgrades. Dialogue is data-driven and conditional. Quests are 18 main segments plus a small optional set. Save is manual + checkpoint + three slots + settings. See `GAMEPLAY_SYSTEMS.md`.

## 13. Characters, in one paragraph

Playable: Evan, then Raynor. Priority faces: Raynor, Leila, Evan, Nofan. Supporting: Orin (prologue + memory), Darius, Malik, Soren, a handful of named servants and guards, optional late character Lord Hazar. No romance minigame. No companion combat AI after Evan dies. See `CHARACTER_BIBLE.md`.

## 14. Failure and difficulty

- Death reloads the last checkpoint. Checkpoints are generous in investigation, tighter in set pieces.
- Stealth failure converts to combat; combat failure is death or a short flee window.
- There is no fail state for “wrong dialogue” except one late irreversible decision.
- Difficulty: Story, Standard, Hard. Story softens combat timing and detection. Hard does not add health sponges; it tightens windows.
- No permadeath. No durability. No hunger.

## 15. Progression (the entire list)

Story-gated, not XP-gated.

| Unlock | When | Who |
|---|---|---|
| Examine / evidence pin | Ch 1 | Evan |
| Quiet step / crouch takedown | Ch 2 | Evan |
| Counter window | Ch 3 | Evan |
| House seal (opens family locks) | Ch 5 | Evan |
| Raynor’s heavier string (charge heavy) | Ch 7 | Raynor |
| Leila’s temple token (temple + fortress doors) | Ch 7 | Raynor |
| Second tablet (final area access) | Ch 8 | Raynor |

Weapons: bronze dagger (start), house sickle-sword (Ch 3), Raynor’s scarred khopesh (Ch 7). No loot table.

## 16. UI, audio, cinematics

See `UI_UX_PLAN.md`, `AUDIO_PLAN.md`, `CINEMATIC_PLAN.md`. Non-negotiable: readable subtitles in EN/AR, a cinematic volume that does not stall gameplay for more than ~4 minutes at a stretch, and a music plan that can ship with a small licensed-or-original palette.

## 17. Technical one-pager

Unreal Engine 5. C++ for core systems, Blueprints for content hookup. Single game module `TBW`. No GAS unless a later review proves it cheaper. Enhanced Input. StateTree for AI. Common UI or a thin UMG layer. SaveGame + a WorldState subsystem. Steamworks only after the Windows build is stable. Full architecture: `TECHNICAL_ARCHITECTURE.md`.

## 18. Production one-pager

Sixteen phases from pre-production to Steam. The hard gate is **Phase 10 Vertical Slice**. Full production does not start if the slice is not fun, stable, and representative. Roadmap: `PRODUCTION_ROADMAP.md`. Scope knife: `SCOPE_CONTROL.md`. Risks: `PROJECT_RISKS.md`.

## 19. Commercial shape

- Premium-priced short cinematic campaign (not free-to-play, not early-access-as-a-service).
- Early Access is a **contingency**, not the plan. The plan is a finished 6–8 hour 1.0.
- Wishlist campaign begins at Vertical Slice trailer, not before there is footage that matches the promise.
- Arabic-speaking market is a first-class audience, not a localization afterthought.
- The product promise on the Steam page must be the game we can actually ship: a short, dark, authored family tragedy in Babylon — not an open-world ancient sandbox.

## 20. Success definition

The planning phase is successful when:

1. These documents are internally consistent.
2. A new hire can understand the game in one day of reading.
3. Every system has a reason to exist and a cost.
4. The Vertical Slice is specified tightly enough to build.
5. The team knows what will be cut first if the calendar slips.

The shipped game is successful when a player can finish a stable 6–8 hour story on Steam, remember the brothers’ faces, and recommend it as a complete experience — not as “ambitious unfinished.”

## 21. Document ownership

| Doc | Owner seat |
|---|---|
| GAME_DESIGN_DOCUMENT | Game Director |
| STORY_BIBLE / CHARACTER_BIBLE | Narrative Lead |
| WORLD_DESIGN | World Director |
| GAMEPLAY_SYSTEMS / AI_DESIGN | Game Designer + Technical Director |
| QUEST_STRUCTURE | Narrative + Design |
| TECHNICAL_ARCHITECTURE | Technical Director |
| UI_UX_PLAN | UI Lead (can be the same person) |
| AUDIO_PLAN / CINEMATIC_PLAN | Director + relevant lead |
| PRODUCTION_ROADMAP / SCOPE_CONTROL / PROJECT_RISKS | Producer |
| QA_PLAN | QA Lead / Producer |
| STEAM_RELEASE_PLAN | Producer |

On a 1–3 person AI-assisted team, one person may hold several seats. The seats still exist so decisions have a name.
