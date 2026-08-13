# QUEST STRUCTURE

**Project:** The Betrayed Will / وصية الغدر  
**Status:** Locked for planning  
**Main mission segments:** 18  
**Optional objectives:** 8, none of which inflate playtime past the cap  
**Law:** A mission can contain multiple objectives. Do not invent missions to make the number look bigger.

---

## 1. How a segment works

A **segment** is a named, shippable piece of the spine. It has:

- An owner chapter
- A playable identity
- A start flag and an end flag
- 2–5 objectives
- A failure policy
- A checkpoint plan
- A cinematic or conversation beat
- An estimated median time

The quest system tracks `SegmentId`, `ObjectiveId`, and world flags. UI shows **one** active title and up to three objective lines.

Naming: `MQ-P0` … `MQ-08c` for main, `OQ-01` … `OQ-08` for optional.

## 2. Spine overview

| ID | Chapter | Name | Who | Median |
|---|---|---|---|---|
| MQ-P0 | Prologue | The Last Night | Raynor | 18 m |
| MQ-01a | 1 | Morning Without Him | Evan | 20 m |
| MQ-01b | 1 | The Canal Clasp | Evan | 20 m |
| MQ-02a | 2 | The Empty Chair | Evan | 20 m |
| MQ-02b | 2 | Roster of the Night | Evan | 25 m |
| MQ-03a | 3 | Grain That Goes Nowhere | Evan | 25 m |
| MQ-03b | 3 | Three Names | Evan | 25 m |
| MQ-04a | 4 | A Note in Soren’s Hand | Evan | 15 m |
| MQ-04b | 4 | The River Storehouse | Evan | 25 m |
| MQ-05a | 5 | Spoken of in the Past | Evan | 20 m |
| MQ-05b | 5 | The Survey Mark | Evan | 25 m |
| MQ-06a | 6 | Water Gate | Evan | 20 m |
| MQ-06b | 6 | Unbroken | Evan | 15 m |
| MQ-06c | 6 | Hold the Gate | Evan → Raynor | 15 m |
| MQ-07a | 7 | Night of the Returned | Raynor | 35 m |
| MQ-07b | 7 | Brothers in Their Rooms | Raynor | 35 m |
| MQ-08a | 8 | The Second Writing | Raynor | 30 m |
| MQ-08b | 8 | The Audience Hall | Raynor | 45 m |

Median spine ≈ **433 minutes ≈ 7 h 13 m**.  
Optional cap ≈ **35 m**.  
This hits the 6–8 hour window without a ninth chapter.

## 3. Segment specs

### MQ-P0 — The Last Night

**Start:** New game. **End:** `WillRead`, `RaynorMissingSetup`.  
**Objectives:** Walk to Orin’s chamber → Speak to Orin → Attend the reading (cinematic) → Speak to Leila on the terrace → Walk to the canal gate.  
**Failure:** None. Soft walls.  
**Checkpoints:** After Orin, after the reading, after Leila.  
**Must prove:** Raynor feel, conversation camera, cinematic pipeline.

### MQ-01a — Morning Without Him

**Start:** Control is Evan. **End:** `EvanSuspicious`.  
**Objectives:** Wake in Evan’s room → Find Raynor’s unused chamber → Speak to Soren → Speak to Malik or Yasmin.  
**Unlocks:** Examine, pin-board, focus.  
**Clues:** Unused bed, scar-oil, performed celebration.

### MQ-01b — The Canal Clasp

**Objectives:** Ask at the service dock → Search the canal edge → Recover Raynor’s cloak clasp → Confront the official story once (Nofan or a guard).  
**End:** `ClaspFound`.  
**Combat:** None required.  
**Close:** Nofan asks Evan, kindly, to stop.

### MQ-02a — The Empty Chair

**Objectives:** Sit the public breakfast *or* refuse it (flavor) → Leave the hall without a scene fail → Reach the lower docks → Talk to the reed merchant.  
**End:** `EmptyChairLeft`.  
**Leila** can be visited early at the temple gate; she gives no location.

### MQ-02b — Roster of the Night

**Objectives:** Enter the barracks annex (stealth) → Read the altered duty roster → Escape → Examine the forged letter’s seal-pressure (found on Malik’s table or given by Yasmin).  
**Unlocks:** Crouch takedown, stone toss.  
**First combat** if detected: 2 house guards, non-lethal preferred.  
**End:** `LetterIsFalse`.

### MQ-03a — Grain That Goes Nowhere

**Objectives:** Market grain dock → Speak to Iltani → Optional street confrontation (Hazar’s men; sickle-sword unlock) → Temple archive stealth or granted entry → Recover the deleted *Kurnah* grain line.  
**End:** `KurnahNamed`.  
**Player still does not know where Kurnah is.**

### MQ-03b — Three Names

**Objectives:** Confront Malik (stonewall) → Press Soren (almost breaks) → Take the grain line to Leila (she goes still) → Pin-board auto-connects Darius / Malik / Nofan.  
**End:** `ThreeNamesPinned`.  
**No location yet.** This is intentional frustration with a reward of certainty.

### MQ-04a — A Note in Soren’s Hand

**Objectives:** Receive the note (servant or under the door) → Check Soren’s room (he is gone; ring is gone) → Optional: show Leila (she says it is a door, not a kindness).  
**End:** `TrapAccepted` (the player can delay, not skip).

### MQ-04b — The River Storehouse

**Objectives:** Approach by the canal path → Infiltrate the storehouse → Ambush (not a fail) → Playable struggle → Water blackout cinematic.  
**End:** `EvanBelievedDead`.  
**Checkpoint:** At the storehouse threshold; death during ambush restarts the ambush, not the chapter.  
**Must prove:** Betrayal as gameplay, not only as film.

### MQ-05a — Spoken of in the Past

**Objectives:** Wake at Nabeh’s shed → Hear NPCs speak of Evan as drowned → Reach the shrine without entering the palace → Find Soren.  
**End:** `SorenConfession` (ring stolen; childhood story of Kurnah).  
**Disguise flag:** a work wrap from Nabeh. Story flag, not a system.

### MQ-05b — The Survey Mark

**Objectives:** Survive or avoid a search party → Meet Leila (she finds him) → Receive temple token + survey mark → Reach the reed boat.  
**End:** `KurnahLocated`.  
**Unlocks:** House seal (from Soren) for family locks; temple token for the water gate.

### MQ-06a — Water Gate

**Objectives:** Boat transition → Reed wall stealth → Cisterns → Water gate with token.  
**End:** `InsideKurnah`.

### MQ-06b — Unbroken

**Objectives:** Read the food ledger and Nofan’s *keep him unbroken* letter → Reach the inner cells → Free Raynor (playable unlock + conversation).  
**End:** `RaynorFreed`.

### MQ-06c — Hold the Gate

**Objectives:** Escort to the inner gate (simple follow, Raynor is AI for 3 minutes) → Last stand as Evan (hold X seconds / Y enemies, then scripted spear) → Give the seal (forced conversation) → First inputs as Raynor on the marsh path.  
**End:** `EvanDead`, `PlayerIsRaynor`.  
**Failure:** Dying before the scripted spear restarts the stand, not the rescue.  
**Must prove:** Identity swap, emotional authorship, no softlock.

### MQ-07a — Night of the Returned

**Objectives:** Re-enter the city by Evan’s map → Visit one changed space (Raynor’s stripped room or the kitchen) → Speak to Soren *or* Leila first.  
**End:** `RaynorInCity`.  
**Unlocks:** Charge heavy, khopesh.

### MQ-07b — Brothers in Their Rooms

**Objectives (any order, all available):**

1. Darius — training court, combat required.
2. Malik — archive, bargain / expose / kill.
3. Captain Radd — optional-feeling but on the way; spare/kill.
4. Leila — trust conversation.

Nofan is at court and cannot be reached.  
**End:** `HouseholdBroken`, plus flags `DariusDown`, `MalikFate`, `LeilaTrust`.

### MQ-08a — The Second Writing

**Objectives:** Temple vault **or** Hazar’s townhouse (branch is a path split of 20 minutes, not a campaign branch) → Recover the second tablet → Learn Hazar’s role (letter or scene).  
**End:** `TabletInHand`.

### MQ-08b — The Audience Hall

**Objectives:** Approach through undercroft / court → Enter the hall → Confront Nofan (dialogue boss) → Optional short duel → Leila’s last scene → Epilogue cards.  
**End:** `Ending_*` one of the three states.  
**Checkpoints:** Threshold of the hall; after Nofan’s fate; before epilogue.

## 4. Optional objectives

None gate the spine. All die if the chapter that owns them ends.

| ID | Window | Name | Reward | Time |
|---|---|---|---|---|
| OQ-01 | Ch 1–2 | Yasmin’s inventory slip | Extra line on Malik | 4 m |
| OQ-02 | Ch 2 | Overhear two guards on the terrace | Confirms canal boat | 3 m |
| OQ-03 | Ch 3 | Help Iltani hide a short measure | Market shortcut later | 5 m |
| OQ-04 | Ch 3 | Read Bel-Iddin’s public edict | Hazar name as rumor | 4 m |
| OQ-05 | Ch 5 | Find Evan’s wrist bead returned by Nabeh | Examine comment as Raynor later | 3 m |
| OQ-06 | Ch 6 | Free one other prisoner in Kurnah | Epilogue card; not a system | 6 m |
| OQ-07 | Ch 7 | Spare Radd and send him to the temple | Changes one guard line in Ch 8 | 0 extra if done in MQ-07b |
| OQ-08 | Ch 7 | Return to Kurnah for Nofan’s full letter | Stronger Ch 8 dialogue | 8 m |

If production slips, cut OQ-06 and OQ-08 first.

## 5. Trigger grammar

All progression is flag-based.

```
Flag examples:
  ClaspFound
  LetterIsFalse
  KurnahNamed
  KurnahLocated
  EvanBelievedDead
  EvanDead
  PlayerIsRaynor
  DariusDown
  MalikFate = Exposed | Dead | Fled
  LeilaTrust = Closed | Allied | Withdrawn
  NofanAlive = Spared | Killed | Bound
  TabletInHand
  SorenRespected   # false if player chose cruel options twice
```

Quests listen. They do not poll the world. Designers add flags in data, not in C++.

Softlock rule: every investigate-to-progress beat has a fallback pointer after two failed searches or one full location loop (a servant, a mark on the pause-map, or Soren). QA will test this list by name.

## 6. Failure policies

| Context | On death | On detection | On “wrong” talk |
|---|---|---|---|
| Exploration / investigate | Checkpoint | n/a | New line, no fail |
| Stealth room | Checkpoint if killed | Combat, or reset if a named brother must not see you | n/a |
| Story ambush (MQ-04b) | Restart ambush | Intended | n/a |
| Last stand (MQ-06c) | Restart stand | n/a | n/a |
| Finale | Checkpoint at hall | n/a | Flags only |

No mission timer except the last stand and one search-party beat in Ch 5 (generous).

## 7. UI presentation

- HUD: location-sensitive. Hidden during explore unless damaged or detected.
- Objective toast on change, 3 seconds.
- Pause: current segment title, 1–3 objectives, pin-board tab, map of **current location only**.
- No quest log novel. Completed segments are a short journal in Evan’s or Raynor’s voice (one sentence each).

## 8. Writing budget (so quest count does not lie)

| Type | Count | Words each (order of) |
|---|---|---|
| Main conversations | ~40 | 150–400 |
| Barks / observe lines | ~200 | 8–20 |
| Clue bodies | 20–30 | 30–80 |
| Journal sentences | 18 | 20–40 |
| Ending cards | 3 sets | 80–120 |

This is a short game. Write less, rewrite more.

## 9. Vertical Slice quest

The slice plays a **compressed** MQ-P0 + MQ-01a + MQ-01b + a steal of MQ-02b’s barracks, not the whole campaign.

Slice objective list (player-facing):

1. Walk with Orin / witness the will (if slice includes prologue cinematic)
2. Search Raynor’s chamber
3. Speak to Soren and one other
4. Recover the clasp
5. Enter a restricted annex
6. Survive or sneak the guards
7. Pin the first lie
8. Save, quit, continue
9. Mission complete toast

Acceptance: a new player finishes in 30–45 minutes without a designer standing behind them.

## 10. What we will not do

- Side jobs for strangers
- Notice-board bounties
- “Collect 10 glazed tiles”
- A second spine
- Radiant “another brother is missing” repeats
- Post-game errands
