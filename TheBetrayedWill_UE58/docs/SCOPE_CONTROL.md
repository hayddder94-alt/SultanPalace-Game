# SCOPE CONTROL

**Project:** The Betrayed Will / وصية الغدر  
**Status:** Binding  
**Owner:** Producer / Director  
**Law:** If a feature has low value and high cost, it is removed. If a change does not name a cut, it is rejected.

---

## 1. The product we are allowed to ship

A **stable, complete, 6–8 hour** cinematic third-person story on Windows Steam.

Quality over quantity. The palace, the brothers, the will, the death, the hall.

Anything that does not raise story, feel, atmosphere, or the chance of a finished 1.0 is a suspect.

## 2. The five questions (every feature)

1. Does it improve gameplay?
2. Does it improve story?
3. Does it improve commercial value?
4. What is its implementation cost (person-weeks, risk, dependencies)?
5. Can it be removed without damaging the game?

Score honestly. A cinematic family tragedy does not need a fishing minigame even if the canal is pretty.

## 3. Locked in (do not cut without replacing the pillar)

These *are* the game:

- Evan playable arc and death
- Raynor playable climax
- Orin, Leila, Nofan as written
- Palace as hero space, reused
- Investigation spine of 20–30 clues
- Focused melee verb set
- Simple stealth
- 18 main segments
- Hero cinematics: will, betrayal, seal, hall
- Save/load that survives restart
- EN + AR subtitles/UI
- Windows Shipping build + Steam preparation

Cutting any of these is a different product and needs a new GDD.

## 4. Locked out (do not add)

From the brief and the assumptions file, restated so no one “just prototypes” them:

- Open world, mounts, second city, wilderness
- Multiplayer, live service, seasons, gacha
- Huge skill trees, loot rarity, crafting, shops
- Hundreds of NPCs, full city simulation
- Hundreds of weapons
- Romance meters, dating, companion wheel
- Magic, bows-as-a-pillar, naval/chariot combat
- New Game+ as a launch requirement
- Photo mode as a launch requirement
- Mod tools
- Playable Leila / Nofan
- Sequel hook / post-credit teaser
- GAS, custom renderer, custom dialogue application
- Steamworks before Phase 16

If a prototype of a locked-out feature appears in the project, delete it in the same week.

## 5. Pre-authorized cut order

When the calendar slips, cut **down this list, in this order.** Do not invent a more expensive cut to save a pet feature.

| Priority | Cut | What remains | Saves (order of) |
|---|---|---|---|
| 1 | OQ-06, OQ-08 | Spine intact | 1–2 cw |
| 2 | Noble district as a space; Hazar letters only | Political pressure remains | 2–4 cw |
| 3 | On-screen Hazar | Letters | 1–2 cw |
| 4 | Poison-Orin optional thread | Illness stands | 1 cw |
| 5 | Unique ending cadences / extra epilogue art | Three text states | 1 cw |
| 6 | Professional barks; Arabic VO day-one | Subtitles; EN scratch/paid principals | 2–6 cw |
| 7 | Market side lane, rooftop runs, palace undercroft loop | Main streets and one corridor | 2–3 cw |
| 8 | Optional Ch 7 Kurnah return | Ch 6 remains hero | 1–2 cw |
| 9 | CIN-05, 07, 09, 16, 17 demoted to gameplay/UI | Four hero cinematics remain | 2–4 cw |
| 10 | Darius fight simplifies (same arena, fewer phases) | Still a named fight | 1 cw |
| 11 | MetaHuman abandoned for traditional hero heads | Faces still hero-authored | variable |
| 12 | Chapter 3 market confrontation becomes avoidable-only | Sickle-sword granted by Soren | 1 cw |

**Never cut** to save time: Evan’s death as a playable stand, the will reading, save integrity, the identity swap, or the hall.

If you have cut through item 12 and are still slipping, **slip the date**. Do not cut the death or inflate combat.

## 6. Feature evaluation log (starting set)

| Feature | Play | Story | $ | Cost | Removable? | Verdict |
|---|---|---|---|---|---|---|
| Dual identity | High | High | High | Med | No | Build |
| Pin-board auto-connect | High | High | Med | Low | No | Build |
| 20–30 clues | High | High | Med | Med | Shrink, don’t kill | Build |
| Focused melee | High | Med | High | Med | No | Build |
| Simple stealth | High | Med | High | Med | No | Build |
| Hero cinematics (4) | Med | High | High | High | No | Build |
| Extra cinematics | Low | Med | Low | Med | Yes | Cut first |
| Noble district | Low | Med | Low | Med | Yes | Cut early if needed |
| Hazar on-screen | Low | Med | Low | Med | Yes | Optional |
| Stone toss | Med | Low | Low | Low | Almost | Build (one verb) |
| Body hide | Med | Low | Low | Low | Almost | Build |
| Relationship meter | Low | Low | Low | Med | Yes | **Out** |
| Bow | Med | Low | Med | High | Yes | **Out** |
| Open streets between all hubs | Low | Low | Med | High | Yes | **Out** (use connectors) |
| Skill tree (8+ nodes) | Low | Low | Low | Med | Yes | **Out** |
| Photo mode | Low | Low | Low | Low–Med | Yes | Post-1.0 |
| Steam cards | Low | Low | Low | Low | Yes | Optional late |
| Arabic VO day one | Low | Med | High | High | Yes (defer) | Decide Phase 0 |
| GAS | — | — | — | High | Yes | **Out** |

## 7. Playtime control

Target median **7h 15m** spine + **35m** optional.

If playtests come in under 5 hours, **do not add a district.** Lengthen the hall, the investigation beats that already exist, or the last stand — or accept a tight 6 hours. Padding is obvious.

If playtests come in over 8.5 hours on the spine, cut walks and repeated rooms, not plot.

Chapter time budgets in the GDD are contracts. A chapter that wants 90 minutes must steal from another chapter in writing.

## 8. Content budgets (do not quietly exceed)

| Thing | Cap |
|---|---|
| Playable identities | 2 |
| Main segments | 18 |
| Optional objectives | 8 |
| Clues | 30 |
| Hero faces | 5 (Raynor, Leila, Evan, Nofan, Orin) |
| Combat identities | 6 |
| Major locations | 7 (one is a state) |
| Unique weapons the player uses | 3 + stone |
| Ability unlocks | ≤ 7 |
| Dialogue words | see Quest Structure |
| Simultaneous full AI | 8 |
| Ending paths | 1 path, 3 states |
| Steam achievements | 30 |

## 9. How a new idea enters the game

1. One-page note: the five questions, the cut that pays for it, the phase it lands in.
2. Director + Producer (can be one human with a night’s sleep) sign.
3. Update Assumptions, Scope, and the relevant bible **in the same change**.
4. If it threatens the VS date, it waits until after the gate.

Prototypes without a note are deleted.

## 10. Vertical Slice scope lock

The VS is a palace morning, not the campaign.

Allowed in VS: prologue cinematic short, Evan, wing + annex + canal, 4–6 clues, 2–3 NPCs, 1 stealth room, 1 fight, save/load, one transition, mission complete.

Not allowed in VS: Kurnah, Raynor gameplay, Darius, market district, skill menus, Steam overlay work, a second combat weapon tree.

If VS is late, cut cinematic length and secondary dressing, not the feel of walk / sneak / hit / save.

## 11. Production anti-patterns to call by name

- “While we’re here, let’s build a proper inventory.”
- “The city will feel empty without 200 NPCs.”
- “We’ll just use GAS, it’s free.”
- “We’ll make the second half open.”
- “Leila should be playable for a chapter.”
- “We can add a bow and leave it optional.”
- “Early Access will save us.”
- “The slice can be grey; we’ll art it later.” (The brief forbids this.)

Each of these is a known way to kill a 7-hour game.

## 12. Success metric for this document

The shipped game is smaller than the most ambitious paragraph in the original brief and better than the median paragraph. That is a win.
