# PHASE 1 REPORT — TECHNICAL FOUNDATION

**Project:** The Betrayed Will / وصية الغدر  
**Version:** `0.1.0-phase1`  
**Engine lock:** Unreal Engine **5.7** (unchanged)  
**Date:** 2026-08-13  
**Status:** Code-first foundation complete. Live UBT compile and Windows `.exe` are **blocked on this host** (no Unreal Editor, no MSVC, 20 GB disk).

---

## Pipeline executed

```
Unreal 5.7
    ↓  locked, not upgraded
TBW Project
    ↓  module expanded, still one game module
Player
    ↓  one pawn, Evan identity by default
East Wing Sandbox
    ↓  runtime greybox of the ceremonial wing
Movement
    ↓  walk / sprint / crouch
Camera
    ↓  over-shoulder, crouch/sprint interp
Interaction foundation
    ↓  trace + prompt + examine flags
World State
    ↓  flags + console
Input
    ↓  Enhanced Input, runtime IMC
Compile
    ↓  structural validation PASS; UBT not available here
Windows Development Build
    ↓  script ready; cannot cook on this Linux host
```

---

## What was built

| Piece | Class / file | Notes |
|---|---|---|
| Input | `UTBWInputConfig` | WASD, mouse, gamepad. Sprint, crouch, interact, pause. No `.uasset` required. |
| Player | `ATBWPlayerCharacter` | Shared pawn. Capsule + preview body. |
| Identity | `FTBWIdentityFactory` | Runtime Evan (default) and Raynor (console only). |
| Camera | spring arm on pawn | 280 default, 340 sprint, 230 crouch. Light lag. |
| Movement | CMC + identity speeds | Walk 420, sprint 620, crouch 160 (Evan). No jump. |
| Interaction | `ITBWInteractable`, `UTBWInteractorComponent`, `ATBWInteractableActor` | Camera sphere trace, `[E]` prompt. |
| World state | `UTBWWorldStateSubsystem` | `Set/Get/Has/Clear` + debug lines. |
| Console | `TBWConsoleCommands.cpp` | `tbw.Flags.Set/Get/List`, `tbw.Identity.Set`, `tbw.Version` (non-Shipping). |
| Sandbox | `ATBWEastWingSandbox` | Hall, terrace, corridor, Evan, Raynor, study, yard, canal, court, annex, west-arch soft wall. |
| HUD | `ATBWHUD` | Title, prompt, controls, flag list. |
| GameMode | `ATBWGameMode` | Spawns sandbox, drops Evan in his chamber. |
| Package | `tools/package_win_dev.ps1` | RunUAT Win64 Development. |

Interactable examines (set flags):

| Object | Flag |
|---|---|
| Empty chair | `EmptyChairExamined` |
| Unused bed | `UnusedBed` |
| Scar-oil | `ScarOil` |
| Canal clasp | `ClaspFound` |
| Night roster | `RosterAltered` |
| Letter | `LetterIsFalse` |
| West arch | `WestArchTouched` |

---

## What was deliberately not built

- Combat, AI, dialogue runner, quests, save slots  
- Raynor as a default playable (console cheat only, Development)  
- Market, temple, noble, residential, Kurnah  
- Steamworks, GAS, StateTree, MetaHuman plugin  
- Will-reading cinematic  
- Authored `.umap` / Nanite art kit  

---

## Compile

`python3 tools/validate_phase1.py` — **0 failed**.

Live Unreal Build Tool compile: **not run**. This sandbox has no `UnrealEditor`, no Epic Launcher, and not enough disk for a 5.7 install.

First action on a Windows machine with UE 5.7.x:

1. Right-click `TheBetrayedWill.uproject` → Generate Visual Studio project files  
2. Build **Development Editor**  
3. Open the project. Ignore a missing `L_Dev_Sandbox`; GameMode injects the wing into `Template_Default`  
4. PIE: you should spawn in Evan’s chamber  
5. Walk the wing. Examine the clasp. `tbw.Flags.List` should show `ClaspFound = 1`  
6. `powershell -File tools/package_win_dev.ps1`

Record the exact 5.7.x hotfix in this file when that machine exists.

---

## Windows Development Build

| Item | State |
|---|---|
| Script | `tools/package_win_dev.ps1` |
| Config | Development, Win64, pak, stage, archive |
| Output | `Saved/StagedBuilds/Win64` |
| Produced on this host | **No** — no toolchain |

`tools/package_win_dev.sh` exits 2 on Linux on purpose so nobody files a fake exe.

---

## How to play the sandbox (once compiled)

| Input | Action |
|---|---|
| WASD / L-stick | Move |
| Mouse / R-stick | Look |
| Left Shift / L3 | Sprint |
| Ctrl or C / B | Crouch toggle |
| E / A | Examine |
| Esc / Start | Pause |

Path: Evan chamber → corridor → hall (chair) → Raynor (bed, oil) → study (letter) → yard → canal (clasp) → court → annex (roster).

---

## Acceptance (Phase 1 plan)

| Criterion | Here | On UE 5.7 machine |
|---|---|---|
| 5.7.x hotfix recorded | 5.7 locked; hotfix pending first install | required |
| `TBW` compiles | source complete | required |
| Editor opens project | n/a | required |
| PIE walk | sandbox coded | required |
| `tbw.Flags.Set ClaspFound 1` | command exists | required |
| Development packaged build | script exists | required |
| No plugin tourists | pass | pass |
| Performance baselines | cannot capture | Phase 1 hour after first PIE |
| No combat / AI / Steam / second district | pass | pass |

---

## Risks introduced

- Runtime IMC may need a modifier tweak after first PIE (WASD swizzle).  
- `Template_Default` path may differ on some 5.7 installs — create `L_Dev_Sandbox` empty and set it as GameDefaultMap if so.  
- Default surface material may ignore color parameters; rooms still read by label and scale.  
- Preview body is a cube, not a mannequin. Replace when the first mesh lands.

---

## Next (not started)

Phase 2 is **player feel** on this sandbox (animation, footsteps, tighter camera), not the campaign. Do not open Phase 3 combat until walking the wing for five minutes feels intentional.
