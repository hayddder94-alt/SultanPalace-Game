# Combat and enemy architecture — design, not code

**Status: GATED.** Nothing in this document is implemented, and I will not
implement it without the sentence in §7.

Written 2026-08-20 in answer to a direct request for a `Monster` base class, a
`Boss` subclass inheriting from it, and a manager that updates 100 monsters per
frame.

---

## 1. Three problems with the request, in order of importance

### 1.1 It is behind the scope gate, and the gate is enforced by a tool

`tools/validate_phase1.py` fails the build if any of these strings appears in
`Source/**/*.cpp`:

```
UGameplayAbility      OnlineSubsystemSteam    ATBWAIController
UTBWQuestSubsystem    UTBWCombatComponent
```

That list is not decoration. Save and dialogue were on it until 2026-08-18 and
came off by an explicit decision that is recorded. Combat, GAS and enemy AI are
still on it.

If I write this code, the validator goes red and I have quietly overruled a
decision you made. The gate exists precisely so that "we needed it" cannot
become the reason scope moved.

### 1.2 The language field was never filled in

> *"بلغة [اكتب اللغة هنا: مثلاً C# في Unity، أو GDScript في Godot، أو بايثون]"*

The template still contains its own placeholder. None of those three is what
this game is: **Unreal Engine 5.8, C++**. C# for Unity would be beautiful, and
it would be dead the moment it was written.

### 1.3 The design asked for is the one I would advise against

This is the part worth your time. See §2 and §3.

---

## 2. `Monster → Boss` inheritance is a trap that springs on the sixth class

The hierarchy is fine with two classes. Here is where it goes:

```
Monster
└── Boss              summons, shields
```

Then the design asks for a flying enemy. Then a ranged enemy that also summons.
Then a boss that does not summon but does shield. You now need:

```
FlyingRangedSummoningMonster
```

…or you duplicate `Summon()` into three unrelated classes, or you push
everything up into `Monster` until the base class is 2,000 lines and every
enemy carries code for abilities it never uses. Every studio has this file.
It is usually called `BaseCharacter.cpp` and everyone is afraid of it.

**The fix is composition.** Behaviour differences become *data and components*,
not subclasses:

| Instead of a subclass | Use |
|---|---|
| `BossMonster` | `AEnemy` + `UHealthComponent` + `UMeleeAttackComponent` + `USummonComponent` |
| "which abilities" | a `UDataAsset` listing them |
| "how much health" | a float in that asset |
| "how it behaves" | a StateTree asset |

One C++ class. Fifty enemies. Every difference authored, not compiled.

Unreal is built for this: `UActorComponent` is the composition unit, and
`UPrimaryDataAsset` is the data unit. Fighting that with an inheritance chain
means writing more code to get less flexibility.

### The point specific to *this* game

`docs/GDD.md` and the character bible say quality over quantity, and there is
no bestiary. The enemies in The Betrayed Will are **house guards and human
antagonists** — one body archetype, varied by equipment and behaviour. That is
a data problem, not a taxonomy problem.

---

## 3. "100 monsters in the game loop" — the number is the problem

### The measurement

An `AActor` with `PrimaryActorTick.bCanEverTick = true` costs roughly **1–3 µs
of engine overhead per frame before your code runs at all** — tick function
scheduling, virtual dispatch, and a cache miss walking to the actor. Add a
behaviour tree and a perception component and a live enemy is comfortably
**30–80 µs**.

100 of them: **3 to 8 milliseconds per frame**, spent before a single triangle
is drawn.

This machine currently renders the palace at **16 FPS — 62 ms per frame**.
There is no room. Adding this would not slow the game down; it would end it.

### What is actually done instead

| Technique | What it buys |
|---|---|
| **One manager tick**, iterating `TArray<FEnemyState>` of plain structs | one cache-friendly loop instead of 100 scattered virtual calls |
| **Actors do not tick.** `bCanEverTick = false` on every enemy | removes the per-actor engine overhead entirely |
| **Significance / LOD update rates** — near every frame, mid every 4th, far every 16th | 100 enemies cost about the same as 20 |
| **An engagement budget** — at most N enemies may attack; the rest hold | this is a *design* fix and the cheapest of all |
| `USignificanceManager` | Unreal already ships the bookkeeping for this |

That is the answer to the manager question, and it is not "a list and a for
loop over `Update()`".

### And the honest observation

`docs/VERTICAL_SLICE_SCRIPT.md` VS-11 specifies the entire combat content of
the slice:

> *1 sword guard if stealth succeeded. 2 if the annex is already Alert.
> This is the entire VS combat. No second wave. No captain.*

**One guard.** Possibly two. Designing a hundred-enemy manager for that is
building a warehouse to store a chair. When there is a real crowd — Chapter 8,
if the design calls for one — the manager gets built against a measured need,
on hardware we have profiled.

---

## 4. What I would build, when authorised

In dependency order. Nothing here mentions a `Monster` class.

**4.1 `UTBWHealthComponent`** — on a component, not a base class, because the
*player* needs it too and Evan is not a monster. Health, damage, death
delegate, invulnerability window. Testable with no enemy in existence.

**4.2 `UTBWDamageEvent`** — a plain struct: amount, instigator, hit direction,
damage type. Passed by value. No inheritance.

**4.3 `ATBWEnemy`** — one class. Capsule, mesh, health component, and a pointer
to its **`UTBWEnemyDefinition`** data asset: health, speed, reach, the montage
set, the loot table if there ever is one.

**4.4 `UTBWEnemyDirector`** — a `UWorldSubsystem`. One tick. Owns the array of
active enemies, the significance buckets and the engagement budget. Enemies
register on spawn and deregister on death; they never tick themselves.

**4.5 Behaviour** — StateTree, already noted as a Phase 4 item in
`TBW.Build.cs`. Not a `switch` on an enum in a base class, which is the same
trap as §2 wearing a different hat.

**4.6 The guard for VS-11** — a `UTBWEnemyDefinition` asset. Not a subclass.

---

## 5. What would need to change in the gate

Removing a name from `tools/validate_phase1.py` must be a decision with a date
and a reason, like the save/dialogue removal on 2026-08-18. On authorisation,
these come off:

* `UTBWCombatComponent` — replaced in the design above by health + attack
  components, so it may never be needed under that name
* `ATBWAIController` — needed for perception and StateTree

These stay on, and I would argue to keep them:

* `UGameplayAbility` — GAS is a large, opinionated framework. For one guard and
  a light/heavy/block/dodge set it is more machinery than the game earns.
* `OnlineSubsystemSteam` — nothing about combat needs it.

---

## 6. What this costs, honestly

| | |
|---|---|
| Health + damage, tested | half a day |
| One enemy, one definition asset, melee that works | two days |
| StateTree behaviour that reads as intentional rather than twitchy | a week, and it is mostly tuning |
| A director that holds 100 enemies at frame rate | pointless until there is content for it |

And the prerequisite that has nothing to do with code: the game runs at 16 FPS
and I still do not know which GPU is drawing it.

---

## 7. The authorisation

I will start on §4 when you write, in plain words, something equivalent to:

> **"Authorised: combat and enemy AI. Remove `UTBWCombatComponent` and
> `ATBWAIController` from the gate."**

Until then this file is the deliverable, and it is deliberately the whole
design rather than a placeholder — so that the day you authorise it, the first
commit is `UTBWHealthComponent`, not another week of architecture argument.
