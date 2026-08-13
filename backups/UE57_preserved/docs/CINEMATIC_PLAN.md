# CINEMATIC PLAN

**Project:** The Betrayed Will / وصية الغدر  
**Status:** Locked for planning  
**Law:** Cinematics are extremely important. Gameplay is still the default sentence. A cutscene that can be a playable walk should be a playable walk.

---

## 1. Role

Cinematics exist to do things the player cannot be asked to act:

- A public ritual with six faces in one room
- A betrayal the player is inside of but does not control
- A death that must land
- A transfer of the body the player inhabits
- An ending that must resolve

They do **not** exist to move the player down a hallway, introduce a button, or show a door opening.

## 2. Duration rules

| Rule | Number |
|---|---|
| Default max continuous non-interactive | 4 minutes |
| Director exception required above | 4 minutes |
| Hard cap without a gameplay insert | 6 minutes |
| Time between memorable authored moments | 15–20 minutes |
| Skip | Hold to skip; some locked until seen once |

If a sequence is longer than 90 seconds, look for a gameplay insert (a walk, a look-at, a single interact).

## 3. The required list (from the brief, expanded only as needed)

These are the cinematics we are willing to spend hero time on. Everything else is conversation camera or in-game.

| ID | Beat | Chapter | Type | Target length | Skip after first? |
|---|---|---|---|---|---|
| CIN-01 | Orin’s last conversation close / death breath | P0 | Hybrid: playable talk + short sequence | 90–150 s sequence portion | Yes |
| CIN-02 | The will reading | P0 | Hero cinematic | 3–4 min | Yes |
| CIN-03 | Terrace with Leila | P0 | Conversation cinema | 60–90 s | Yes |
| CIN-04 | Raynor walks to the canal (disappearance button) | P0 | Playable with cinematic exit | 20 s exit | Yes |
| CIN-05 | Empty chair breakfast (optional short) | Ch 2 | In-game staged | 30–45 s | Yes |
| CIN-06 | Raynor’s disappearance confirmed (Evan at the dock, if needed) | Ch 2 | Short | 20–40 s | Yes |
| CIN-07 | Three names on the board (can be UI-cinema) | Ch 3 | UI + camera | 15 s | Yes |
| CIN-08 | The canal betrayal / stab / water | Ch 4 | Hero cinematic | 2–3 min | Yes |
| CIN-09 | Nofan is told; lamp in Evan’s room | Ch 4 | Hero short | 45–70 s | Yes |
| CIN-10 | Reed cutter’s hands; Evan lives | Ch 4–5 | Short | 30–40 s | Yes |
| CIN-11 | Leila finds Evan | Ch 5 | Conversation cinema | 60–90 s | Yes |
| CIN-12 | Boat toward Kurnah | Ch 5 | Transition cinema | 20–40 s | Yes |
| CIN-13 | Cell: Evan finds Raynor | Ch 6 | Hybrid playable + close-ups | 60–90 s | Yes |
| CIN-14 | Evan’s death / the seal | Ch 6 | Hero cinematic after playable stand | 90–150 s | **No** until seen |
| CIN-15 | Raynor on the path (identity transfer) | Ch 6 | Short + control return | 20–30 s | No until seen |
| CIN-16 | Raynor’s transformation (cloak, scar, city) | Ch 7 open | Short montage, mostly playable dress | 40–60 s | Yes |
| CIN-17 | Darius down (if not a gameplay finisher) | Ch 7 | Optional 8 s | — | Yes |
| CIN-18 | Tablet in hand | Ch 8 | Short | 20–30 s | Yes |
| CIN-19 | Final confrontation | Ch 8 | Hybrid: dialogue cinema + optional duel | 4–6 min including player choices | After first ending |
| CIN-20 | Ending / epilogue cards | Ch 8 | Typographic + stills | 90–120 s + credits | Credits skippable |

**CIN-02, CIN-08, CIN-14, CIN-19** are the four hero spends. If the cinematic budget melts, protect those four and degrade the rest to conversation camera.

The brief’s list is covered:

| Brief item | ID |
|---|---|
| Orin’s death | CIN-01 |
| Will reading | CIN-02 |
| Raynor’s disappearance | CIN-04 / CIN-06 |
| Evan’s betrayal | CIN-08 |
| Evan discovering Raynor | CIN-13 |
| Evan freeing Raynor | Playable + CIN-13 |
| Evan’s death | CIN-14 |
| Raynor’s transformation | CIN-16 |
| Final confrontation | CIN-19 |
| Ending | CIN-20 |

## 4. Hybrid over film

Prefer this pattern:

```
Player walks into a trigger
    → light camera take-over (still in-engine)
    → conversation or action the player can still fail or choose
    → short locked shot for the thing that must be exact
    → control back, world state changed
```

Examples:

- **Will reading:** player is seated or standing in a mark; they can look slightly; full lock only for the naming and Nofan’s face.
- **Betrayal:** player infiltrates the storehouse in gameplay; lock only at the knife and the water.
- **Rescue:** player unlocks the cell; conversation is playable; lock for the first clear look at Raynor’s face.
- **Death:** last stand is gameplay; lock for the spear that cannot be a random death, and the seal.
- **Finale:** Nofan’s dialogue is a cinematic conversation with choices; the possible duel is gameplay; the last line is locked.

## 5. Camera language

- Close on eyes and hands (scar, seal, wax, clean hands).
- Wider only for ritual and for the marsh fortress as a threat.
- Motivated moves: a servant passing, a lamp, a door. No orbit porn.
- Height: human. We are not a drone over Babylon.
- Combat finishers: 2-second inserts, disable-able.
- Never steal the camera for more than a beat to show a collectible.

Shot list discipline: every hero cinematic has a written shot list before anyone keys a sequencer.

## 6. Technical

- Level Sequencer + cinematic cameras.
- Gameplay skeletons. Face: pose assets + visemes; MetaHuman only if Phase 0 chooses it for the four hero heads.
- `UTBWCinematicSubsystem` owns input lock, HUD hide, skip, and the finished callback that sets flags.
- Spawn or teleport named actors to marks. Do not maintain a parallel cinematic-only world.
- Audio: see Audio Plan. VO + subtitle always.
- Lighting: cinematic shots may enable a small extra key light; they must still look like the gameplay lighting family so the cut back is not a different game.
- Save: a sequence mid-play is a no-save volume. Checkpoints sit on either side.

## 7. Performance and capture

For a small AI-assisted team:

- Block with mannequins and temp cameras in pre-production of each chapter.
- Body: key + a small mocap subset if a cheap suit or AI mocap becomes reliable enough. Do not wait on a stage for VS.
- Faces: hero anims on Raynor, Leila, Evan, Nofan, Orin. Everyone else can live on a shared face set.
- Crowd in CIN-02: 6 sons + Leila + witness + 4–6 staff. That is the crowd.

Do not plan a 40-actor will reading.

## 8. Writing for image

Cinematic dialogue is shorter than novel dialogue.

- Orin talks about water and sons, not fate.
- The will is legal and specific (see Story Bible).
- Evan’s last lines are few and concrete (the seal, Soren, the truth).
- Nofan in the hall is sure, not loud.
- Leila’s last scene is one decision, not an essay.

If a line explains a shot, cut the line.

## 9. Vertical Slice cinematics

Must be in the slice at **representative quality**, not full campaign quantity:

1. Will reading (can be a shortened CIN-02, ~2 min) **or** a high-quality still sequence if animation is late — but faces and lighting must be the real target.
2. One conversation cinema (Soren or Leila).
3. One exit sting when the clasp is found or the annex is cleared.

Acceptance: a stranger believes this is the game we intend to ship, not a greybox trailer with a different lighting bible.

## 10. Production order

1. Shot lists for CIN-02, CIN-08, CIN-14, CIN-19
2. Camera and lighting bible in the palace hall (this is also gameplay)
3. Sequencer pipeline + skip + subtitle
4. VS versions of CIN-02 + one conversation
5. Full CIN-08 / 14 / 19 during the chapters that own them
6. Polish pass on faces and mix last

## 11. Cuts

If cinematic schedule dies:

1. CIN-05, CIN-07, CIN-17 become pure gameplay/UI
2. CIN-09 becomes a overheard conversation
3. CIN-16 becomes a playable wardrobe interact
4. CIN-02 shortens to the naming and three faces
5. Never cut the knife, the seal, or the hall

## 12. What we will not do

- Pre-rendered film files as the pipeline (in-engine only, unless a trailer needs a take)
- A 12-minute opening we do not let the player touch
- QTE sword spectacles
- A mid-credits sequel tease
- Cinematic-only characters who never exist in gameplay
