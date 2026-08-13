# UI / UX PLAN

**Project:** The Betrayed Will / وصية الغدر  
**Status:** Locked for planning  
**Law:** The interface is a servant of a cinematic game. If a menu looks like an RPG, it is wrong.

---

## 1. Experience principles

1. **During play, almost nothing.** Health only when damaged. Detection only when relevant. Interact only when near. Objectives only on change.
2. **Diegetic where it is free.** Wax-seal marks, oil-lamp light as restricted-space language. Do not fake a diegetic OS.
3. **Two languages from day one.** English and Arabic, including layout flip for Arabic.
4. **Gamepad and mouse are equal.** Prompts follow the last used device.
5. **Readable in motion.** Subtitles and prompts must work on a 14-inch laptop and a living-room TV.
6. **No shop. No skill tree. No loot toast rain.**

## 2. Screens

### 2.1 Boot and title

- Studio card (short)
- Title: **The Betrayed Will** / **وصية الغدر** with a still of the audience hall or the scar
- Continue / New / Load / Settings / Credits / Quit
- New game: difficulty select, then a short content warning (family murder, drowning, violence)

### 2.2 In-game HUD (hidden by default)

| Element | When visible |
|---|---|
| Interact prompt | Look-at valid target |
| Detection eye / meter | Awareness > 0 in stealth context |
| Health pips | After damage, 4 s, or when < full |
| Stamina | When not full |
| Objective toast | On change, 3 s |
| Button prompt for toss / focus | First hour, then fade unless re-enabled in settings |
| Subtitles | When anyone speaks |
| Cinematic letterbox | Sequences and some conversations |
| Save spinner | Tiny, corner, on autosave |

No minimap. No compass stack of icons. No XP. No weapon hotbar (one weapon). Holster is an input, not a UI slot.

### 2.3 Pause (tabbed, keyboard/gamepad)

1. **Story** — segment title, 1–3 objectives, one-sentence journal
2. **Board** — pin-board (People / Places / Lies)
3. **Place** — hand-authored plan of the **current** location, player mark, objective mark
4. **Inventory** — the five things you can actually hold, or empty state
5. **Settings**
6. **Save / Load**
7. **Quit**

Pause freezes the world. It is allowed during explore and combat. It is blocked during non-skippable beats of MQ-06c and the finale confrontation start (short).

### 2.4 Pin-board

A still, beautiful board. Clues appear as waxed scraps. When a pair auto-connects, a thread draws and a one-line conclusion writes itself. The player can re-read. The player cannot play string-teasing as a minigame.

Arabic: board layout mirrors.

### 2.5 Dialogue

- Speaker name
- Body text
- Optional voice
- 0–3 choices at the bottom
- Important (flag-changing) choices marked with a small seal
- Prompt to continue
- Skip line on input; skip speech if VO is long

Conversation camera is in-world. UI is a lower-third, not a visual novel full screen, except for the will-reading where a ceremonial treatment is allowed.

### 2.6 Examine

Object fills a simple turntable. One highlight. Name + short body. Exit.

### 2.7 Death / load

- Quiet fail screen, not a joke
- Last checkpoint name
- Reload / Quit to title
- Fast (target < 2 s from confirm on SSD for in-level checkpoint)

### 2.8 Cinematic skip

Hold to skip, with a hint. Some sequences skippable only on subsequent playthroughs (will reading, Evan’s death). Stored per slot.

### 2.9 Endings

Epilogue cards are typographic, bilingual option, on black or on a still. Credits after. Return to title. No forced second playthrough nag.

## 3. Visual language

| Token | Use |
|---|---|
| Lapis / glazed blue | Titles, seals, important marks |
| Oil-lamp amber | Prompts, warmth |
| Bone / papyrus | Body text backgrounds when needed |
| Iron oxide / dried blood | Damage, death, hard warnings — used sparingly |
| Type | A readable serif for titles; a highly readable sans for UI body and Arabic Naskh-equivalent licensed font |

No chrome sci-fi frames. No medieval parchment kitsch from a marketplace pack without a paint-over.

Safezone: 90% default, adjustable.

## 4. Localization UX

- Language switch in boot and settings. Apply immediately to UI; dialogue in the current conversation refreshes on next line.
- Arabic is RTL. Widgets that are lists or choice rows must flip. The HUD detection meter stays in a consistent physical corner so muscle memory survives.
- Fonts must cover Arabic fully. Test with long Arabic subtitles in Ch 8.
- Speaker names stay as locked player-facing names, with Arabic spelling decided in a string table (`راينور`, `إيڤان`, `نوفان`, `ليلى`, `أورين`, etc. — final forms in localization pass).
- Do not embed English in textures that matter.

## 5. Input UX

### 5.1 Gamepad (default suggestion)

| Button | Action |
|---|---|
| L stick | Move |
| R stick | Look |
| A / Cross | Interact / Confirm |
| B / Circle | Crouch toggle (or hold, setting) |
| X / Square | Light |
| Y / Triangle | Heavy |
| LT / L2 | Block |
| RT / R2 | Dodge (press) / also block-release parry window is on Block timing, not a third trigger |
| LB / L1 | Toss |
| RB / R1 | Focus |
| L3 | Sprint |
| R3 | Holster |
| Start | Pause |
| Select | Board shortcut (optional) |

Parry is a timed Block, not a separate face button. This keeps the verb set honest.

### 5.2 KBM (default suggestion)

WASD, mouse look, Shift sprint, Ctrl crouch, E interact, LMB light, RMB block, Mouse4/Q dodge, F heavy, G toss, MMB/Q-alt focus, Tab pause-board, Esc pause, R holster.

All remappable. Conflict warnings. Reset to default.

### 5.3 Prompt icons

Steam Input / generic Xbox + DualSense + KBM sets. Never show `Press E` while a gamepad is the last device.

## 6. Settings information architecture

**Display:** resolution, window, vsync, frame cap, brightness/gamma, HDR if trivial.

**Graphics:** preset (Low/Med/High/Epic/Custom), view distance, shadows, post, textures, effects, foliage, shading, Lumen, motion blur, film grain, chromatic (last two off by default).

**Audio:** master, music, sfx, vo, ambience, speaker/headphone.

**Gameplay:** difficulty, camera invert X/Y, sensitivities, toggle/hold, HUD verbosity, finisher camera, tutorial hints.

**Accessibility:** subtitle size/bg/speaker colors, colorblind-safe detection, parry assist, hold-to-skip time, reduce flicker, screen narration is **not** a 1.0 promise (document as post-1.0 if ever).

**Language:** UI + subtitles. VO language when more than one exists.

**Input:** remap, vibration.

Apply / Back. Graphics that need restart say so.

## 7. Save / load UX

- Three slots with: chapter name, identity, playtime, timestamp, a still.
- Continue on the title is last used slot.
- Autosave rotates a backup. UI never says “saving” for more than a moment.
- Overwrite confirm.
- Load of a newer/incompatible version: a clear error, no silent wipe.
- Manual save disabled toast with reason if in a no-save volume.

## 8. Feedback and juice (small list)

- Hit stop and a short camera nudge
- Damage vignette
- Detection click (audio + eye)
- Objective wax-stamp sound
- Successful parry spark (restrained)
- Save tick

No damage numbers. No combo counter. No “Sneak 100” toast.

## 9. Tutorials

Taught in Prologue and Ch 1 by doing, with one-line prompts.

| Skill | Where |
|---|---|
| Move, look, interact | Prologue walk |
| Conversation | Orin |
| Examine, focus, board | Raynor’s chamber |
| Crouch, detection | Canal / barracks |
| Takedown | Barracks, optional |
| Light / heavy / block / dodge | First fight or a training dummy in the court if the player has not fought by the annex |
| Toss | Same stealth room |
| Identity change | MQ-06c, a single line: his name, no skill recap dump |

A “hints” toggle kills almost all of this. Never a tutorial dungeon.

## 10. Vertical Slice UI acceptance

- Title → New → difficulty → into game
- HUD hidden until needed
- EN and AR subtitle paths both render (AR can be placeholder font if licensed font is late, but RTL must work)
- Pause with objectives + 4 clues on the board + save/load
- Death → reload checkpoint
- Continue from title after a cold quit
- Gamepad and KBM prompts swap
- No placeholder `Text Block` visible

## 11. What we will not ship in 1.0

- Photo mode
- New Game+ menu
- Photo/sticker inventory
- Crafting UI
- World map with 80 icons
- Twitch/overlay integration
- Store/DLC page inside the game (Steam is enough)
- Mod browser
