# AUDIO PLAN

**Project:** The Betrayed Will / وصية الغدر  
**Status:** Locked for planning  
**Law:** Atmosphere is a pillar. Audio is half of it. We still will not build a reactive music MMO.

---

## 1. Role of sound

1. Make Babylon feel hot, dry, and near water.
2. Make the palace feel occupied even when it is lying.
3. Make stealth readable without looking at the HUD.
4. Make violence sudden and ugly, not sporty.
5. Carry grief after Evan dies, without scoring every corridor.

## 2. Buses and mix

| Bus | Role | Notes |
|---|---|---|
| Master | Output | Soft ceiling, no slamming limiter as “loudness” |
| Music | Beds + stings | Ducks under VO |
| VO | Dialogue + cinematic speech | Always intelligible |
| SFX | Gameplay one-shots | Hits, UI, interact |
| Ambience | Rooms + exterior beds | The soul of the mix |
| Foley | Footsteps, cloth, body | Player and near AI |
| Cin | Reserved ducking group during sequences | |

Target: living-room night session. Music should not fight subtitles.

A headphone / speaker toggle is enough. No 10-band EQ in 1.0.

## 3. Music

### 3.1 Approach

A small original palette. Not a 4-hour through-composed film score. Not licensed modern pop. Not generic “oriental” marketplace loops.

Instrumentation direction (reference, not a shopping list): frame drum, low strings, wood flute / ney-adjacent color, bronze percussion, human voice used sparingly as texture — **never** as fake sacred chant ripped from a real liturgy.

If a composer is not available early: a short temp suite (6–8 beds, 8–10 stings) that matches the palette. Temp must be legally usable in trailers or clearly replaced before any public footage.

### 3.2 Beds (the whole list)

| Id | Use |
|---|---|
| `mus_palace_night` | Prologue walk, Ch 7 return |
| `mus_palace_morning` | Ch 1 performed joy, slightly wrong |
| `mus_investigate` | Neutral search |
| `mus_temple` | Temple courts |
| `mus_market` | Sparse, human, percussion distant |
| `mus_marsh` | Approach to Kurnah |
| `mus_kurnah` | Interior fortress |
| `mus_stealth_pressure` | Layers on when awareness > 0 |
| `mus_combat` | Short loop, exits fast |
| `mus_grief` | Post MQ-06c, used rarely |
| `mus_hall_finale` | Audience hall Ch 8 |
| `mus_epilogue` | Cards + credits |

### 3.3 Stings (the whole list)

Will seal, clue connect, betrayal hit, drowning blackout, cell door, Evan’s last breath (very dry, almost no score), identity transfer, Darius down, tablet in hand, three ending cadences.

Music does not stinger every clue pickup. Only auto-connects and chapter gates.

### 3.4 Interactive logic

`UTBWAudioSubsystem` holds a music state:

`Explore / Investigate / StealthPressure / Combat / Cinematic / Grief / Silence`

Rules:

- Combat overrides.
- StealthPressure is a layer, not a hard cut.
- Silence is a valid state (Orin’s chamber, Evan under water, the hall before Nofan speaks).
- No 32-layer vertical remix system.

## 4. Ambience

Every major location has a day-state and a night-state bed, plus 2–4 spot emitters.

| Location | Signature |
|---|---|
| Palace interiors | Distant river, oil flicker, far footsteps that may be a lie |
| Palace terrace | Wind, city murmur, water |
| Residential | Courtyard voices, well pulley, dogs far away |
| Market | Flies, grain pour, bronze tap, shade-cloth |
| Temple | Echo, incense hiss, bare feet, bells used rarely |
| Noble street | Emptier, sandals on glazed brick |
| Kurnah | Reeds, mosquitoes, drip, iron, men trying not to talk |
| Canal / boat | Water against reed, oar, night insects |

Ambience does more world-building than extra streets will.

## 5. Foley and gameplay SFX

### 5.1 Footsteps

Surfaces: stone, glazed brick, mudbrick dust, wood, wet stone, reed/marsh, rug.  
Stances: walk, run, crouch.  
Identities: Evan lighter, Raynor heavier.  
AI uses a cheaper subset.

This is a stealth game. Footstep quality is not cosmetic.

### 5.2 Combat

Blade tap, blade bite, block, perfect parry (distinct, short), dodge cloth, stagger, body fall, finisher (ugly, short, no orchestra stab unless it is Darius or the finale).  
Player hit has a dull body thud, not a AAA meat-chorus.

### 5.3 Stealth

Detection tick (pitched with awareness), whisper barks, stone toss, lamp extinguish, body drag, door breath.

### 5.4 UI

Wax stamp (objective, save), soft ceramic (navigate), seal break (back/cancel), clue place. No modern whooshes.

## 6. Voice

### 6.1 Production stages

| Stage | What |
|---|---|
| Through Vertical Slice | Scratch or generated VO for principals in the slice script. Subtitles always complete. Will reading + Evan examine lines should be the best we can get. |
| Full production | Placeholder for all story conversations. Barks as text-to-speech or scratch. |
| Polish | Professional VO for Raynor, Leila, Evan, Nofan, Orin, and key supporting. |
| Arabic VO | Planned as day-one *or* first major patch. Decision in Phase 0. Subtitles are not optional. |

Generated voices are a scaffold. They must not ship if they embarrass the faces. A quiet subtitle-only pass is better than bad machine Orin in the prologue.

### 6.2 Cast priority for paid VO

1. Raynor
2. Leila
3. Evan
4. Nofan
5. Orin
6. Soren / Darius / Malik (can share a smaller session)
7. Barks pool (guards, market, servants)

### 6.3 Bark categories (keep small)

Idle, suspicious, alert, lost player, sight combat, hit, death, civilian flee.  
~12–16 lines per category per pool, not hundreds.

### 6.4 Language

Player-facing names stay. In-world tablets can have a short “translated from the tablet” treatment in UI, not a fake conlang the player must learn.

## 7. Cinematic audio

- Will reading: room tone, wax, a voice that fills the hall, Nofan’s silence.
- Drowning: almost no music, water interior, then air as pain.
- Evan’s death: dry, close, the seal in a hand, marsh night only after he is gone.
- Identity transfer: one low note, then Raynor’s heavier breath and footsteps teach the new body.

Mix cinematic VO louder and dryer than explore barks. Do not bury faces in score.

## 8. Implementation

- MetaSounds for footsteps, loops with parameters (wet, distance), and combat hits.
- Waves for VO and unique cinematics.
- Attenuation and occlusion: Unreal defaults + a few presets (hall, room, marsh).
- Audio volumes per wing.
- Missing VO never blocks; subtitle still plays for `DurationOverride` or a words-per-minute estimate.

Naming: `VO_Speaker_Segment_Line`, `SFX_Wpn_Block_01`, `AMB_Palace_Night`, `MUS_Hall_Finale`.

## 9. Legal

- No copyrighted music.
- No uncleared samples of real temple ritual.
- Marketplace ambience only with a license that allows Steam shipment and trailers.
- Composer contract must include Steam, trailers, and a short live session clip.

## 10. Vertical Slice audio acceptance

- Palace night and morning beds
- Footsteps on stone and rug that change when crouched
- Two combat hits + block
- Detection tick
- Interact / clue / save UI sounds
- Will-reading VO or a strong scratch
- A working subtitle path when VO is absent
- No looping click, no silent interact, no music that ignores pause (pause ducks or holds)

## 11. Cuts if the audio schedule dies

1. Unique ending cadences (reuse finale bed)
2. Market music (ambience only)
3. Professional barks (reuse a small pool)
4. Arabic VO day one (keep subtitles)
5. Composer-final combat loop (temp)

Never cut: footsteps, detection readability, prologue room tone, drowning, Evan’s last scene dryness.
