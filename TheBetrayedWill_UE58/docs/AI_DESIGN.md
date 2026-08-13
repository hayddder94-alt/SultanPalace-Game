# AI DESIGN

**Project:** The Betrayed Will / وصية الغدر  
**Status:** Locked for planning  
**Law:** Modular, reusable, story-supporting. No life-sim, no crowd brain, no social graph.

---

## 1. What AI exists for

1. Make restricted spaces feel watched.
2. Make combat readable and fair.
3. Make investigation feel like it can be interrupted.
4. Support a few authored brother confrontations.

If an AI feature does not do one of those four, it is cut.

## 2. Populations

| Population | Brain | Count alive at once | Notes |
|---|---|---|---|
| House / city / fortress guards | Guard StateTree | 2–8 | Primary stealth opposition |
| Combat enemies (same bodies, combat mode) | Combat StateTree | 1–4 | Often the same actor after Alert |
| Civilians | Ultra-thin | 4–16 | Idle, talk, flee, or cower. No jobs system. |
| Named brothers | Scripted + combat profile | 0–1 in play | Darius is a combat elite. Others are dialogue with optional combat. |
| Animals | None at 1.0 | 0 | Ambient birds are FX, not AI |

One civilian system: `UTBWAmbientHuman`. States: Idle, Converse, Flee, Cower. No schedules beyond a spline or a chair.

## 3. Shared perception

All thinking actors use one perception model.

### 3.1 Senses

| Sense | Data | Notes |
|---|---|---|
| Sight | Cone angle, range, half-range in low light, blocked by geometry | Reduced vs. crouched player in shadow volumes |
| Hearing | Event radius by noise class | CrouchStep, Walk, Sprint, Attack, BodyDrop, Whistle, Vessel |
| Damage | Instant max awareness + know attacker location | |
| Ally call | On Alert, nearby guards gain a search origin | Through `UTBWAlertSubsystem` |

Awareness 0–100:

| Range | State implication |
|---|---|
| 0–19 | Idle / Patrol |
| 20–49 | Suspicious (look, step toward) |
| 50–79 | Alert / Search |
| 80–100 | Combat if line of sight, else Search at last known |

Decay: slow in the player’s last known area, faster elsewhere. Story difficulty decays faster.

### 3.2 Light and stance modifiers

- Player in a `TBW.Light.Lit` volume: sight range against them × 1.25
- Player crouched in `TBW.Light.Shadow`: sight × 0.55, hearing step class drops
- Player sprinting: hearing class Sprint regardless of light
- Identity: Raynor is easier to see and hear than Evan (data, not a second system)

### 3.3 Friendly fire and civilians

Guards do not hunt civilians. Civilians do not report with a phone. A civilian who sees a body can raise a local shout (hearing event) and flee. That is the entire civilian detection loop.

## 4. Guard AI

### 4.1 States (required by the brief, locked)

```
Idle ⇄ Patrol → Suspicious → Alert → Search ⇄ Combat
                      ↓                   ↓
                   Return to patrol ← ← ← ┘
```

| State | Behavior | Exit |
|---|---|---|
| **Idle** | Stand, idle anim, optional small look | Timer → Patrol; stimulus → Suspicious/Alert |
| **Patrol** | Follow `ATBWPatrolSpline` or point list, pause at wait tags | Stimulus; spline end loops |
| **Suspicious** | Stop, look at stimulus, short investigate walk, “who’s there” bark | Awareness ≥ 50 → Alert; decay → Return |
| **Alert** | Draw weapon, move to last known, notify `AlertSubsystem` | Sight on player → Combat; timeout → Search |
| **Search** | Search pattern around origin (expanding points, 15–25 s) | Sight → Combat; timeout → Return |
| **Combat** | Hand off to Combat tree | Target dead / lost for X s → Search |
| **Return to patrol** | Holster if safe, walk back, drop zone alarm if last man | Arrive → Patrol/Idle |

Do not add: Sleep, Eat, Gossip, Arrest, Interrogate, Drag prisoner, Dual-wield stance, Call reinforcements from another district.

Reinforcements are **authored**: a trigger can spawn or enable a waiting pair. Not a simulation.

### 4.2 Restricted volumes

`ATBWRestrictedVolume` with access tags (`Family`, `Guard`, `Temple`, `Fortress`).  
If the player is inside without the tag and is seen, awareness jumps (Suspicious at the edge, Alert if deep).

Named-brother volumes: if Nofan must not see Evan in Ch 2, detection here is a soft reset with a line, not a death. Telegraph with a UI whisper.

### 4.3 Bodies

- Unhidden corpse in a guard’s sight: Alert at the corpse, zone alarm up one level.
- Hidden in `HideBody` volume: ignored.
- Drag is short-range and slow. It is a stealth tool, not a physics toy.

### 4.4 Takedowns

If unaware and attacked from a valid takedown angle: death or KO (Evan Ch 1–2 prefers KO).  
If aware: not a takedown, it is a combat opener.

## 5. Combat AI

Used by anyone who has entered Combat, including Darius and (optionally) Nofan.

### 5.1 States (required by the brief, locked)

```
Idle → Detect → Approach → Attack ⇄ Defend
                     ↓        ↓
                  Stagger    Retreat
                     ↓        ↓
                    Dead    (re-Approach or Dead)
```

| State | Behavior |
|---|---|
| **Idle** | Out of combat; not used once the fight has started except as a reset |
| **Detect** | Turn to target, bark, draw if needed |
| **Approach** | Close to weapon range, circle slightly, respect nav |
| **Attack** | Choose a light string or a telegraphed heavy from a tiny weighted table |
| **Defend** | Block or backstep when the player is mid-string or after a failed hit |
| **Stagger** | Hit react, vulnerable to finisher if health threshold met |
| **Retreat** | Step out when isolated and hurt, or when a spear user wants range |
| **Dead** | Ragdoll or death anim, then sleep. Opt-in save if the corpse matters |

### 5.2 Roles (data, not subclasses)

| Role | Attack | Defend | Notes |
|---|---|---|---|
| Sword guard | 2-hit light, rare heavy | Block | Default |
| Hired blade | Faster lights, sidestep | Dodge more than block | Hazar |
| Spear keeper | Poke, keep distance | Backstep | Kurnah |
| Captain | Mix + one grab-break | Aggressive defend | Radd |
| Darius | Long tells, punish dodge-spam, one unlockable slam | Rare retreat | Boss-lite |
| Nofan | Short, precise, talks between bouts | High defend | Optional duel |

Token system: only **one** enemy attacks at a time in a 1v3, a second may pressure (approach, not full string). This keeps fights readable and cheap.

### 5.3 Fairness

- All heavies have a wind-up the player can see.
- No 360 instant hits.
- Difficulty changes timing and damage, not health × 4.
- Navmesh must be clean; if an enemy cannot reach, they shout and hold, they do not rubber-band through walls.

## 6. Architecture

Prefer **Unreal StateTree** for Guard and Combat.

Fallback: Behavior Trees with the same state names, if StateTree blocks the Vertical Slice. The public state list does not change.

```
ATBWAICharacter
  UTBWHealthComponent
  UTBWCombatComponent          # same as player, different data
  UTBWPerceptionComponent
  UTBWIdentityOrRoleData       # mesh, role table
ATBWAIController
  StateTree (Guard or Combat)
  Blackboard / StateTree vars: Target, LastKnown, Home, Role
```

`UTBWAlertSubsystem` (world):

- Zone id → alarm level 0–2
- Last search origin
- Who is alive in the zone
- API: `Raise(Zone, Reason)`, `Clear(Zone)`

AI never includes Quest or Dialogue headers. A guard can fire a flag (`TBW.Flags.PlayerSeenInBarracks`) through a notify component.

## 7. Authoring tools for designers

Minimum:

- Patrol spline with wait points and look targets
- Restricted volume
- Hide-body volume
- Light volume
- Alert zone volume
- “Enable these AI on flag” actor
- Perception debug draw (Development)

A designer should be able to dress a stealth room without C++.

## 8. Named character AI

| Character | When they think | When they are scripted |
|---|---|---|
| Darius | Combat in MQ-07b and Ch 6 reinforcements | Entrances, the canal ambush |
| Captain Radd | Combat | Canal knife, Ch 7 meeting |
| Nofan | Optional short duel | Almost always dialogue |
| Leila / Soren / Malik / Orin | Never full combat AI | Sequencer + dialogue. Malik may flee (nav to a door). |
| Raynor as ally (3 min in MQ-06c) | Simple follow + limited combat | He must not steal kills or die. God-flag him if needed. |

No companion command system.

## 9. Difficulty

| | Story | Standard | Hard |
|---|---|---|---|
| Sight range | 0.8 | 1.0 | 1.15 |
| Awareness decay | Fast | Normal | Slow |
| Attack token cooldown | Long | Normal | Short |
| Damage taken by player | 0.6 | 1.0 | 1.35 |
| Parry window | Wide | Normal | Tight |
| Search time | Short | Normal | Long |

Hard does not add new types or health sponges.

## 10. Vertical Slice AI acceptance

- 2–3 guards on a patrol spline in the palace annex
- Sight and hearing readable (debug can be off; the detection widget is enough)
- Suspicious → Search → Return works without getting stuck
- Detection converts to combat
- Combat vs. 2 sword guards is winnable and losable
- One takedown on an unaware guard
- No navmesh holes in the slice map
- A civilian who sees violence flees and does not T-pose

## 11. Performance

- Full brain: ≤ 8
- Perception ticks staggered
- Sleep AI more than N meters from the player if not in an active alert zone
- No ticking crowd

## 12. Explicit non-goals

- Daily schedules for a city
- Relationship memory between random NPCs
- Stealth “social hide in crowd”
- Mounted AI
- Group tactics beyond the attack token
- AI director that scales spawns
- Learning / adaptive AI
- Recurring nemesis

## 13. Test cases (must be in QA)

1. Guard walks a spline for 5 minutes without jitter.
2. Player crouch-walks behind a guard; no false detect.
3. Player sprints in a hall; detect from behind on hearing.
4. Body found raises zone; body hidden does not.
5. After losing the player, Search then Return, holster, resume spline.
6. 1v3 token: only one full attack string at a time.
7. Spear user keeps distance on open floor.
8. Darius tells can be parried or dodged on Standard.
9. Soft-reset volume for a named brother fires once, not in a loop.
10. Identity swap does not leave AI ticking on a dead Evan pawn.
