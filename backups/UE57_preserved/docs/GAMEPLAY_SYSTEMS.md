# GAMEPLAY SYSTEMS

**Project:** The Betrayed Will / وصية الغدر  
**Status:** Locked for planning  
**Law:** Four pillars. No fifth pillar without a cut.

---

## 1. Pillars and mix

| Pillar | Evan (Ch 1–6) | Raynor (Ch 7–8) | Ship-or-cut test |
|---|---|---|---|
| Exploration | Primary | Secondary | Does this room change what I know or how I feel? |
| Investigation | Primary | Light | Does this clue change a flag, a door, or a conversation? |
| Stealth | Secondary | Primary | Can failure become combat without a mission fail? |
| Combat | Rare, sharp | Primary | Is a fight a sentence in the story, not a grind? |

There is no crafting, no hunting, no horse, no skill tree, no loot rarity, no companion command wheel.

## 2. Shared player controller

One pawn class: `ATBWPlayerCharacter`.  
A `UTBWPlayerIdentityComponent` swaps:

- Mesh / animation set
- Movement tuning
- Melee profile
- Stealth profile
- Voice / examine comments

Input map is identical for Evan and Raynor.

### 2.1 Move set

| Input | Action |
|---|---|
| Move | Walk / analog run |
| Sprint | Short burst, costs stamina, louder |
| Crouch | Stealth stance, slower, quieter |
| Interact / Examine | Context |
| Light attack | Fast string (2–3 hits) |
| Heavy attack | Slow, breaks guard if charged (Raynor charge from Ch 7) |
| Block | Hold, stamina drain vs. heavy |
| Dodge | I-frames, short, stamina cost |
| Parry / Counter | Timed block window → opening |
| Takedown | Stealth only, from behind, unaware |
| Weapon holster | Changes greetings and some NPC reactions |

No jump as a traversal verb except authored mantles (hold interact at a marker).  
No swim gameplay. Water is cinematic or a kill/transition volume (Ch 4).

### 2.2 Feel targets

- Input to attack: ≤ 80 ms to the first active frame feeling
- Dodge: readable, not a panic spam (recovery is real)
- Camera: over-shoulder, combat slightly tighter
- Hit stop: 2–4 frames on light, 6–8 on heavy/finisher
- Evan and Raynor must feel different in ten seconds of walking

### 2.3 Identity tuning (starting values, not sacred)

| | Evan | Raynor |
|---|---|---|
| Walk / run | Slightly faster | Slightly slower |
| Stamina | Lower | Higher |
| Detection radius | Smaller when crouched | Larger |
| Light damage | 0.8× | 1.0× |
| Heavy damage | 0.85× | 1.25× |
| Poise | Low | High |
| Takedown | Yes | Yes, slightly longer anim |
| Charge heavy | No | Yes (Ch 7) |

## 3. Exploration

Exploration is **room-scale authorship**, not a checklist.

Verbs:

- Walk and look
- Interact with highlighted and *some* unhighlighted objects (the important ones always have a tell on close approach)
- Overhear a conversation by standing still in a radius
- Climb an authored mantle
- Open a door the current identity / key item allows

Rules:

- No climbing tower. No synchronization stance. No eagle vision as a mode. A short “focus” hold can desaturate the room and lift clue silhouettes; it is a readability aid, not a superpower. Focus is available from Ch 1 and does not upgrade.
- Secrets are story-flavored (a letter, a hidden latch, a child’s drawing of Kurnah) and count toward optional objectives, not a collectible counter on the HUD.

## 4. Investigation

Investigation is the spine of Evan’s game and a seasoning in Raynor’s.

### 4.1 Clue object

`UTBWClue` data asset:

- `ClueId`
- `Title` / `Body` (EN + AR)
- `SpeakerComment` (Evan or Raynor line)
- `Tags`
- `RevealsFlags[]`
- `RequiresFlags[]`
- `PinBoardGroup`

There are **20–30 clues** in the whole game. That is the list. Not 200.

### 4.2 Pin-board (pause UI, not a room the player walks)

A simple board with three columns: **People**, **Places**, **Lies**.  
The player does not play a corkboard minigame. Connecting is automatic when the right pair of flags is on. A short sting and a one-line conclusion appear. That conclusion can open a quest gate.

This keeps the fantasy of connecting evidence without building a detective sim.

### 4.3 Examine camera

On interact, a short in-engine examine (rotate the object, one highlight). Exit returns to play. No separate “investigation mode” level.

### 4.4 NPC investigation reactions

- Some lines change after a clue flag.
- Some NPCs refuse a topic until a clue exists.
- Guards do not “know you investigated” unless you were seen in a restricted room.

### 4.5 What investigation is not

Not a notebook RPG. Not a fingerprint system. Not a chemistry table. Not a timeline reconstruction minigame.

## 5. Stealth

Simple and functional. If it takes more than a week to explain, it is too big.

### 5.1 Perception

Every `ATBWAICharacter` has:

- Sight cone (angle + range, reduced in low light)
- Hearing radius (footstep class: walk / sprint / crouch / action)
- Awareness value 0–100

States: see `AI_DESIGN.md` (Idle, Patrol, Suspicious, Alert, Search, Combat, Return).

### 5.2 Player tools

- Crouch
- Lean (light, peek from corners — no full cover shooter)
- Whistle / stone toss: **one** distraction verb, unlocked Ch 2. Not a gadget wheel.
- Silent takedown on unaware humanoid from behind
- Bodies: can be dragged a short distance into a volume tagged `HideBody`. Hidden bodies are not found. Unhidden bodies escalate a zone after a short timer.
- Light: standing in a lit volume increases sight range against the player. Extinguishing a lamp is allowed on tagged lamps only (not every light in the game).

### 5.3 Failure

Detection fills a meter, then converts to Alert → Combat.  
Almost no “mission failed, you were seen” screens. Exceptions: one prologue-adjacent servant scene (soft fail, restart beat) and any scene where a named brother must not see you yet (telegraph this clearly).

### 5.4 What stealth is not

Not social stealth with disguises as a system (one story disguise in Ch 5 is a quest flag, not a system).  
Not light-and-shadow as a binary vampire rule.  
Not a takedown combo tree.

## 6. Combat

Third-person melee. The verb list in the brief is the **entire** list.

### 6.1 Verbs

Light, Heavy, Block, Dodge, Counter/Parry, Finisher.

Finishers trigger when an enemy is staggered below a threshold and the player presses heavy. They are cinematic-short (2 seconds), not QTEs, and they are skippable by looking away in settings (accessibility).

### 6.2 Weapons (the entire inventory)

| Weapon | Owner | When | Role |
|---|---|---|---|
| Bronze dagger | Evan | Start | Fast, low damage, stealth |
| House sickle-sword | Evan | Ch 3 | Standard melee |
| Raynor’s khopesh | Raynor | Ch 7 | Heavy standard |
| Thrown stone | Both | Ch 2 | Distraction, not a weapon |

No bows. No spears as player weapons (enemies may use them). No magic. No dual wield. No weapon durability. No shops.

### 6.3 Resources

- Health: segments, restored at checkpoints, by a limited bandage item (max 3 carried, authored pickups), and after major story beats.
- Stamina: regenerates, blocks sprint/dodge/block.
- No mana. No rage meter. A short “pressure” window after a perfect parry is a status, not a resource bar.

### 6.4 Enemies (the entire roster)

| Archetype | Count of unique bodies | Behavior |
|---|---|---|
| House guard | 2 meshes | Sword + block |
| Hired blade (Hazar) | 2 meshes | Faster, less armor |
| Fortress keeper | 2 meshes | Spear, longer reach |
| Captain (elite) | 1 (Radd + reused) | Mix, can grab-break |
| Darius | 1 hero | Boss-lite, readable tells |
| Nofan | 1 hero | Optional short duel, more dialogue than health |

That is six combat identities. Named brothers use hero anims on those identities.

Simultaneous enemies: 1–3 standard, 4 only in the Ch 6 gate. Never a horde.

### 6.5 Damage philosophy

Readable, fast, lethal enough that three hits on Standard matter.  
No bullet-sponge. Boss-lite fights are about tells, not 12-minute bars.

### 6.6 Why not Gameplay Ability System

GAS is powerful and expensive to learn, debug, and content. For six verbs, two identities, and six enemy types, a custom `UTBWCombatComponent` + anim notifies is cheaper and clearer. Revisit only if replication or complex status stacking appears — it will not.

## 7. Progression

Story gates, four to seven unlocks, three weapons. Full list is in the GDD §15.

There is no XP, no levels, no gear score, no skill menu with locked squares.  
The pause screen can show “what you can do now” as a reference, not as a build planner.

Player knowledge *is* progression: knowing Malik lies, knowing the undercroft, knowing Soren will open a door.

## 8. Interaction

`ITBWInteractable` on actors.

Channels:

- Examine (clue)
- Talk (dialogue start)
- Door / latch
- Pickup (bandage, token, seal)
- Hide body
- Extinguish lamp
- Boat / transition trigger (often automatic)

Prompt: single button, context label. No radial menus.

## 9. Dialogue (player-facing)

See also Technical Architecture and UI plan.

- Conversation camera, not always full lock if the space is dangerous.
- Choices: 0–3 per node. Most conversations have zero or one real choice.
- Choices that change flags are visually marked with a small seal icon. Flavor choices are unmarked.
- No skill checks. No “intelligence 7 required.”
- Timeout choices: only in Ch 4 ambush aftermath and Ch 8 finale, telegraphed.

## 10. Quest

See `QUEST_STRUCTURE.md`. Systems view:

- `UTBWQuestSubsystem` holds active quest, objectives, and listeners for flags.
- Main quest is a spine of **18 segments**.
- Optional objectives hang off segments and never gate the spine except where a clue is the intended path (then a fallback pointer exists after a time/visit threshold so softlock is impossible).

## 11. Save

See Technical Architecture for the class list.

Player-facing:

- Manual save at any time the pawn is not in a “no-save volume” (active cinematic, active finisher, falling).
- Autosave on objective complete, on location transition, on identity swap, on death-adjacent checkpoints.
- Three slots + Continue (last used).
- Settings in a separate `TBWSettings` save, not in the slot.
- Saves survive restart. Saves are versioned. A failed load never silently wipes a slot.

## 12. Accessibility (minimum ship set)

- Subtitles EN/AR, size, background
- Hold-to-toggle for crouch and sprint
- Parry window assist (Story difficulty, optional on Standard)
- Colorblind-safe detection indicator
- Remappable input
- Camera shake / finisher disable
- Auto-succeed QTE if any QTE sneaks in (prefer none)

## 13. Controller and KBM

Both first-class. See UI/UX plan. No keyboard-only prompt on a gamepad session and the reverse.

## 14. Systems we evaluated and rejected

| System | Why rejected |
|---|---|
| Full GAS | Cost > need |
| Nemesis / recurring generated rivals | Scope, tone |
| Social stealth disguises as a loop | One story beat is enough |
| Branching campaign | Playtime and cinematic cost |
| Horse / chariot | World size temptation |
| Bow stealth-archer | Becomes a different game |
| Crafting / cooking | Does not serve story |
| Relationship meters | Flags are enough |
| New Game+ modifiers | Post-1.0 if ever |
| Photo mode | Polish-phase only if spare |

## 15. Vertical Slice system subset

Must work in the slice:

- Evan identity controller
- Examine + 4 clues + pin-board auto-connect
- One conversation with 1 real choice
- Crouch, detection, one takedown
- Light/heavy/block/dodge vs. 2 house guards
- One door key flag
- Manual save + checkpoint + load
- One cinematic in, one cinematic out

Raynor identity, charge heavy, Darius, Kurnah, and the control transfer are **not** slice requirements. They are Phase 11.

## 16. Implementation order (systems only)

1. Pawn, camera, movement, interact
2. Combat verbs vs. a dummy
3. AI perception + states
4. Clue + pin-board
5. Dialogue player
6. Quest flags
7. Save/load
8. Identity swap (after Evan feels good)
9. Polish windows, juice, accessibility

Do not start 8 before 1–7 have acceptance tests.
