# TECHNICAL ARCHITECTURE

**Project:** The Betrayed Will / وصية الغدر  
**Code name:** TBW  
**Engine:** Unreal Engine **5.8** (locked — `ENGINE_VERSION_LOCK.md`)  
**Languages:** C++ for systems, Blueprints for content hookup and iteration  
**Target:** Windows PC (Steam), 64-bit  
**Status:** UE 5.8-only. Phase 1 source foundation. Runtime/Editor not executed on this host.

---

## 1. Principles

1. Prefer an Unreal system that already exists.
2. One game module. An editor module only if a tool proves it needs one.
3. Data-driven content (DataAssets, DataTables). Designers should not wait on a programmer to add a clue or a flag.
4. Clear interfaces, few dependencies. Combat does not include Dialogue headers. Quest does not include AI headers. Both talk through **flags and gameplay tags**.
5. Placeholder assets are replaceable without renaming the logical id.
6. No networking. No dedicated server. No GAS unless a written review overturns this.
7. Crash-safe first. Pretty second.

## 2. Module layout

```
/TheBetrayedWill/
  TheBetrayedWill.uproject
  /Source/
    /TBW/
      TBW.Build.cs
      TBW.h
      TBW.cpp                    # IMPLEMENT_PRIMARY_GAME_MODULE
      /Core/
      /Player/
      /Combat/
      /AI/
      /Interaction/
      /Investigation/
      /Dialogue/
      /Quest/
      /Save/
      /UI/
      /Audio/
      /Cinematics/
      /World/
      /Inventory/                # tiny: keys, bandages, tokens
      /Settings/
    /TBWEditor/                  # optional, only if needed
  /Content/                      # see §4
  /Config/
  /Docs/                         # this documentation set, mirrored or referenced
```

`TBW.Build.cs` public dependencies (starting set):

`Core`, `CoreUObject`, `Engine`, `InputCore`, `EnhancedInput`, `UMG`, `Slate`, `SlateCore`, `AIModule`, `GameplayTags`, `GameplayTasks`, `NavigationSystem`, `LevelSequence`, `MovieScene`, `CinematicCamera`, `PhysicsCore`.

Add `StateTreeModule` + `GameplayStateTreeModule` when AI work starts.  
Add `Steamworks` / `OnlineSubsystemSteam` only in Phase 16.  
Do **not** add CommonUI, GAS, or Chaos Vehicles “just in case.”

Target hardware class in DefaultEngine.ini: DX12, SM5/SM6, with a scalability path down to a mid-range 2018–2020 GPU. Lumen and Nanite are allowed on High/Epic and have fallbacks.

## 3. Class and subsystem map

All game classes use the `TBW` prefix. `A`/`U`/`F`/`I` Unreal prefixes stay.

### 3.1 Core

| Class | Role |
|---|---|
| `UTBWGameInstance` | Owns save/load orchestration, settings, language, identity-of-last-play |
| `ATBWGameMode` | Default mode. No respawn-as-a-service. Handles death → checkpoint. |
| `ATBWGameState` | Thin. Most state lives in subsystems. |
| `UTBWWorldStateSubsystem` | **The** flag store. `bool` / enum / int flags. Replicated nowhere. Saved. |
| `UTBWTagStatics` | Gameplay tag queries used as the shared language |
| `UTBWLog` | Category log macros: `LogTBWSave`, `LogTBWQuest`, `LogTBWAI`, … |

Flags are the bus. `UTBWWorldStateSubsystem` stores `FName → int32`. Typed overlay `ETBWWorldFlag` covers the Phase 1 examples (`WillWasRead`, `RaynorDisappeared`, `EvanInvestigating`, `ClueFound_01`, `GuardAlerted`) without blocking later data-driven names. Example: `WorldState.SetFlag(TEXT("ClaspFound"), 1)` → listeners react. This is not a quest system.

### 3.2 Player

| Class | Role |
|---|---|
| `ATBWPlayerCharacter` | Single pawn for both identities |
| `ATBWPlayerController` | Input routing, UI show/hide, cinematic disable |
| `UTBWPlayerIdentityComponent` | Evan / Raynor swap: mesh, anim, tunables, voice |
| `UTBWMovementComponent` | Extends `UCharacterMovementComponent` only if needed; prefer data |
| `UTBWFocusComponent` | Investigation focus readability |
| `UTBWStealthComponent` | Noise emitter, stance, body-drag |
| `UTBWHealthComponent` | Shared with AI |
| `UTBWStaminaComponent` | Player only |
| `UTBWCameraComponent` | Shoulder camera, conversation blend, combat tighten |

Identity swap is a function, not a possess-of-a-new-pawn, except in MQ-06c where a short possess of Raynor after Evan’s death is acceptable if swap-in-place proves messy. Prefer one pawn to keep save and camera simple.

### 3.3 Combat

| Class | Role |
|---|---|
| `UTBWCombatComponent` | Verbs, state (Idle/Attack/Block/Dodge/Stagger/Dead), hit rules |
| `UTBWWeaponActor` | Visual + trace sockets |
| `UTBWHitboxNotify` | Anim notify state |
| `FTBWAttackData` | DataAsset: frames, damage, stamina, launch |
| `UTBWDamageTypeMelee` | Distinguishes light/heavy/finisher |
| `ITBWDamageable` | Interface on player and AI |

No ability graph. Combos are indexes in the component.

### 3.4 AI

| Class | Role |
|---|---|
| `ATBWAICharacter` | Guard / enemy pawn |
| `ATBWAIController` | Possessor |
| `UTBWPerceptionComponent` | Wraps or extends AIPerception |
| `UTBWGuardStateTree` / schema | Guard loop |
| `UTBWCombatStateTree` / schema | Combat loop |
| `UTBWPatrolSpline` | Authoring |
| `UTBWAlertSubsystem` | Zone alarm level, body-found, search origin |

See `AI_DESIGN.md`.

### 3.5 Interaction / investigation

| Class | Role |
|---|---|
| `ITBWInteractable` | `CanInteract`, `GetPrompt`, `Interact` |
| `UTBWInteractorComponent` | Trace + prompt |
| `ATBWClueActor` | World clue |
| `UTBWClueData` | DataAsset |
| `UTBWInvestigationSubsystem` | Collected clues, auto-connect rules |
| `UTBWPinBoardPolicy` | Which pairs produce a conclusion |

### 3.6 Dialogue

| Class | Role |
|---|---|
| `UTBWDialogueData` | DataAsset: nodes, lines, choices, conditions, consequences |
| `UTBWDialogueSubsystem` | Runner |
| `UTBWDialogueWidget` | Subtitles + choices |
| `ATBWDialogueCameraRig` | Shot / reverse |

Conditions and consequences are **flag queries and flag sets**. Optionally a quest event. No Lua.

### 3.7 Quest

| Class | Role |
|---|---|
| `UTBWQuestData` | Segment + objectives |
| `UTBWQuestSubsystem` | Active segment, listeners |
| `UTBWObjectiveTrigger` | Volume / actor that sets flags |

### 3.8 Save

| Class | Role |
|---|---|
| `UTBWSaveGame` | Slot payload |
| `UTBWSaveSubsystem` | Manual, auto, continue, versioning, checksum |
| `FTBWSaveHeader` | Version, chapter, identity, timestamp, playtime |

Saved:

- World flags
- Collected clue ids
- Quest segment + objectives
- Player transform, identity, health, inventory (tiny)
- Difficulty, and a hash of content version
- Named actor states that opted in (`ITBWSaveable`)

Not saved: physics of debris, crowd positions, anim time.

Versioning: `SaveVersion` int. Migrators live in `UTBWSaveSubsystem::Migrate`. A failed migrate refuses to overwrite the slot and shows a UI error.

Autosave uses a rotating backup (`_bak`) so a crash mid-write cannot eat the only copy.

### 3.9 UI

| Class | Role |
|---|---|
| `UTBWHUD` | Minimal runtime |
| `UTBWPauseWidget` | Objectives, pin-board, map, settings, save |
| `UTBWPromptWidget` | Interact |
| `UTBWDetectionWidget` | Stealth meter |
| `UTBWCinematicSubtitleWidget` | Shared with dialogue |
| `UTBWSettingsWidget` | Graphics, audio, input, language, accessibility |

### 3.10 Audio / cinematics / world

| Class | Role |
|---|---|
| `UTBWAudioSubsystem` | States, music beds, stings (wraps MetaSounds) |
| `ATBWMusicTrigger` | Volume |
| `UTBWCinematicSubsystem` | Play Level Sequence, lock input, skip policy |
| `ATBWLocationVolume` | Location id + lighting state request |
| `UTBWStreamingLibrary` | Safe travel / sublevel |

### 3.11 Inventory (intentionally tiny)

`UTBWInventoryComponent`: a list of `FTBWItemId` with counts. Item types: `Key`, `Token`, `Bandage`, `Quest`. No weight. No rarity. No inspect stats.

## 4. Content folder structure

Phase 0 lock uses `Content/TBW/...` (brief). Full-game aliases below remain valid as *logical* groups inside that root.

```
/Content/TBW/
  /Characters/
    /Raynor/  /Leila/  /Evan/  /Nofan/  /Orin/  /Darius/  /Malik/  /Soren/
    /Guards/  /Civilians/  /Shared/
  /Environments/          # see World Design
  /Cinematics/
    /Prologue/ /Ch01/ ... /SharedShots/
  /UI/
    /HUD/ /Menus/ /Dialogue/ /Fonts/ /Icons/
  /Audio/
    /Music/ /SFX/ /VO/ /Ambience/ /MetaSounds/
  /FX/
  /Maps/
    /Persistent/
    /Palace/
    /Districts/
    /Kurnah/
    /Dev/
  /Blueprints/
    /Player/ /AI/ /Interact/ /World/ /Debug/
  /Data/
    /Clues/ /Dialogue/ /Quests/ /Combat/ /Items/ /Flags/
  /Animations/
    /Evan/ /Raynor/ /AI/ /Shared/ /Cinematic/
  /Materials/
    /Master/ /Instances/ /Functions/ /PostProcess/
  /Developers/            # per-user sandboxes, not shipped
```

Maps: `L_Palace_P`, `L_Market_P`, `L_Temple_P`, `L_Residential_P`, `L_Noble_P`, `L_Kurnah_P`, `L_MainMenu`, `L_VS_Palace` (slice).

## 5. Naming conventions

| Kind | Pattern | Example |
|---|---|---|
| C++ type | `ATBW…` / `UTBW…` / `FTBW…` / `ITBW…` | `UTBWQuestSubsystem` |
| Files | Match type | `TBWQuestSubsystem.h` |
| Blueprint class | `BP_TBW_Role` | `BP_TBW_Guard` |
| Anim BP | `ABP_TBW_Raynor` | |
| Input action | `IA_TBW_LightAttack` | |
| Input context | `IMC_TBW_Default` / `_Dialogue` / `_Cinematic` | |
| Data asset | `DA_Clue_CanalClasp` | |
| Flag tag | `TBW.Flags.ClaspFound` | |
| Gameplay tag | `TBW.Identity.Evan`, `TBW.Weapon.Khopesh` | |
| Level | `L_Location_P` / `_W` wing | `L_Palace_Family_W` |
| Static mesh | `SM_Pal_Column_A` | |
| Skeletal mesh | `SK_Raynor` | |
| Material | `M_` master, `MI_` instance | |
| Texture | `T_Name_D/N/RMA` | |
| Niagara | `NS_DustMote_Interior` | |
| Level sequence | `LS_P0_WillReading` | |
| Widget | `WBP_TBW_Pause` | |
| MetaSound | `MS_TBW_Footstep_Stone` | |
| VO wave | `VO_Evan_Ch1_Bed_01` | |

Never put spaces in content names. Never use `NewBlueprint_2`.

## 6. Input

Enhanced Input only.

Contexts (priority high to low):

1. `IMC_TBW_Cinematic` (skip)
2. `IMC_TBW_UI`
3. `IMC_TBW_Dialogue`
4. `IMC_TBW_Combat` (same keys, consumed when armed and an enemy is near — or simply always available)
5. `IMC_TBW_Default`

Phase 1 actions: Move, Look, Sprint, Crouch, Interact, Primary, Secondary, Pause.  
Primary/Secondary are reserved for Phase 3 combat and do nothing but log.  
Later: Block, Dodge, Focus, Toss, Holster. See `INPUT_MAP.md`.

Gamepad and KBM maps ship together. Mouse sensitivity and a separate aim-look for gamepad.

## 7. Animation architecture

- One AnimBP per identity, layered: locomotion, overlay (look/aim-ish), combat cache, stealth, full-body cinematic.
- Shared hit reacts and deaths where possible.
- Linked Anim Layers if it stays simple; do not over-engineer.
- Motion Matching is **out** for 1.0. Too much capture cost.
- IK: two-bone foot IK, simple hand IK for interact. No full Control Rig dependency for gameplay.
- Cinematics use Level Sequencer + the same skeletons. **Phase 0 lock:** MetaHuman DNA for the four primaries + Orin; custom period clothing; traditional modular civilians/guards. See `CHARACTER_PIPELINE.md`.

## 8. Dialogue data shape (minimum)

```
UTBWDialogueData
  DialogueId
  Participants[]   # speaker id, optional override mesh
  Nodes[]
    NodeId
    SpeakerId
    TextEN / TextAR
    VO
    DurationOverride
    Conditions[]   # flag == value
    Choices[]
      TextEN / TextAR
      NextNode
      Consequences[]  # set flag, give item, start quest event
      bMarkedImportant
    NextNodeIfNoChoice
    CameraShot
    bSkippable
```

A simple editor utility widget to create these assets is enough. Do not build a standalone dialogue application.

## 9. World state and streaming

- `UTBWWorldStateSubsystem` is the authority.
- Location volumes request lighting scenarios (`LS_Pal_NightOil`, etc.) via level streaming or sequencer.
- Named actors implement `ITBWSaveable` (`SaveId`, serialize a small struct).
- Travel: `UGameplayStatics::OpenLevel` only at major gates; palace wings are sublevels.

## 10. UI architecture

UMG. A thin `UTBWWidget` base for input routing and safezone.  
CommonUI is optional and only if we already know it; it is not a Phase 1 dependency.

All text goes through a string table (`ST_TBW_UI`, `ST_TBW_Dialogue`, `ST_TBW_Clues`) for EN/AR.

## 11. Audio architecture

MetaSounds for footsteps, combat one-shots, and looping ambience.  
Music: authored beds + stings, switched by `UTBWAudioSubsystem` on flags and location. See `AUDIO_PLAN.md`.

VO: wave + subtitle even when VO is missing (subtitle still shows). Never block progress on a missing VO file.

## 12. Cinematic architecture

Level Sequencer. `UTBWCinematicSubsystem::Play(Id, FOnFinished)`.  
Skip: hold to skip if `bSkippable`. Some sequences (Evan’s death, will reading) are skippable only after first play, stored in the slot.

Keep characters on the gameplay skeletons. Do not maintain a second cinematic-only rig unless a shot cannot work without it.

## 13. Settings and scalability

`UTBWSettingsSave` (not slot-bound):

- Resolution, window mode, vsync, frame cap
- Scalability group + individual: view distance, shadows, post, FX, textures, Foliage, shading
- Lumen on/off, Nanite is mostly automatic
- Gamma, motion blur, film grain, chromatic (default off for the last two)
- Audio buses: master, music, sfx, vo, ambience
- Language, subtitle size, accessibility flags
- Input maps

Apply without restart where Unreal allows. Require restart only for API / display adapter.

## 14. Build, versioning, crash

- Shipping, Development, DebugGame configs.
- `TBW_VERSION` in a version file written by the build script (semver + changelist).
- Crash reporter: Unreal’s crash uploader **or** a simple local crash folder the player can attach. Do not ship a silent crash.
- Logging in Shipping: a rotating log, capped, no secrets.
- Deterministic content cooking for Windows.

No anti-cheat. Single player.

## 15. Steam (planned, not early)

Phase 16 only:

- `OnlineSubsystemSteam`
- App ID in config, not hardcoded in many files
- Achievements via a `UTBWAchievementBridge` that listens to flags
- Steam Cloud on the save directory
- Overlay compatible (do not lock the mouse in a way that breaks it)

Do not compile Steam into the Vertical Slice unless it is free.

## 16. Tools (only what we need)

Phase 0–2:

- Editor utility: create clue / flag / quest stub
- Debug cheat overlay (Development): set flag, give item, swap identity, teleport, skip segment, god, kill AI
- PIEstart at each `L_VS_*` and chapter door

Later, only if pain is real:

- Dialogue preview widget
- AI perception debug that designers can read

No standalone launcher. No live-ops dashboard.

## 17. Testing hooks

Every subsystem exposes a Development console:

```
tbw.Flags.Set ClaspFound 1
tbw.Flags.List
tbw.Quest.SkipTo MQ-06c
tbw.Identity.Set Raynor
tbw.Save.Quick
tbw.Save.Load 0
tbw.AI.DumpPerception
```

QA scripts in `QA_PLAN.md` depend on these.

## 18. Performance budgets (PC, 1080p High, target 60)

| | Budget |
|---|---|
| GPU frame | 16.6 ms |
| Game thread | < 8 ms typical explore |
| Draw calls | Prefer instancing / Nanite; keep CPU draws sane in palace |
| Visible NPCs | ≤ 20 |
| Active AI full brain | ≤ 8 |
| Lights with shadow in a hero interior | authored, not dozens of movable |
| Streaming hitch | < 100 ms player-facing; hide in doors/boats |

Budgets are revisited after the Vertical Slice with actual numbers.

## 19. What we will not custom-build

- A new renderer
- A new animation engine
- A new physics engine
- A networking layer
- A full quest editor application
- A full dialogue editor application
- A save system that serializes the entire world actor graph
- A plugin marketplace stack that we cannot debug

## 20. Decision log (technical)

| Decision | Choice | Revisit if |
|---|---|---|
| Module count | 1 game module | Build times become painful |
| GAS | No | Status/combat complexity explodes (it must not) |
| AI | StateTree, BT fallback | StateTree blocks VS |
| UI | UMG | Team already fluent in CommonUI |
| Faces | **Hybrid:** MetaHuman DNA for 4 primaries + Orin; custom period clothes; traditional modular for guards/civilians | Phase 2 import test fails non-preset faces |
| Identity | One pawn + component | Swap bugs exceed possess bugs |
| Steam | Phase 16 | Publisher demands earlier (none yet) |
| Engine minor | **5.8 only** (not 5.7, not 5.6) | Production machine already has 5.8; dual-version support is forbidden |
| Content root | `Content/TBW/...` per Phase 0 brief | — |
| World flags | FName map + `ETBWWorldFlag` overlay | Quest system would be a different class |
| Phase 1 default map | Runtime `ATBWDevSandbox` (L_Dev_Sandbox stand-in). East Wing actor exists but is not the default. | First editor session can save a real `.umap` |
