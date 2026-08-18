# PHASE 2 EXECUTION

**Project:** The Betrayed Will / وصية الغدر  
**Engine:** Unreal Engine **5.8 only**  
**Date:** 2026-08-14  
**Scope:** Player feel only. No combat, campaign, AI, dialogue, quests, save, Steam.

---

## Changes made

| Area | Change |
|---|---|
| Movement | Higher accel (1800) and braking (2200), friction 10, min analog 20, move deadzone 0.12. Tiny `ETBWMoveState`. Sprint speed only on ground and not crouched. |
| Camera | Pitch −55…70 on `PlayerCameraManager`. Look scales 0.85 / 0.70. Probe 20, `ECC_Camera`, collision test on. Slightly tighter lag. |
| Interaction | Owner-chain resolve, 0.18 s cooldown, `GetStatusLine()`. |
| Test door | `ATBWTestDoor` — technical toggle, flag `TestDoorOpen`. Not a story door. |
| Sandbox | Crouch gate + test door. Still ugly greybox. No palace. |
| Identity | Same pawn. Preview cube tint/scale differs for Raynor debug only. |
| HUD | Extra move + focus lines. **Arabic Slate title untouched.** DejaVu font not replaced. |
| Input | Primary/Secondary still reserved. `INPUT_MAP.md` updated. |

## Files changed

- `Source/TBW/Public/Player/TBWPlayerCharacter.h` / `.cpp`
- `Source/TBW/Private/Player/TBWPlayerController.cpp`
- `Source/TBW/Public/Interaction/TBWInteractorComponent.h` / `.cpp`
- `Source/TBW/Public/Interaction/TBWTestDoor.h` (new)
- `Source/TBW/Private/Interaction/TBWTestDoor.cpp` (new)
- `Source/TBW/Public/World/TBWDevSandbox.h` / `.cpp`
- `Source/TBW/Public/Core/TBWWorldFlags.h` / `.cpp`
- `Source/TBW/Private/UI/TBWHUD.cpp`
- `docs/PHASE2_BASELINE.md`
- `docs/PHASE2_EXECUTION.md`
- `docs/PHASE2_PERFORMANCE_BASELINE.md`
- `docs/INPUT_MAP.md`

## Build result — **SUCCEEDED** (2026-08-18, user's Windows machine)

```
Engine         : 5.8.1  at  E:\UE_5.8   (installed build, InstalledBuild.txt present)
Target         : TheBetrayedWillEditor Win64 Development
Exit code      : 0  ->  BUILD SUCCEEDED
Duration       : 0.6 min (incremental; the first attempt built SharedPCH.UnrealEd)
Errors         : 0
Warnings       : 1  (FSlateFontInfo deprecation, known)
Output         : UnrealEditor-TBW.dll
```

Toolchain confirmed on the target machine: bundled .NET 10.0 win-x64, MSVC 14.44.35228,
Windows SDK 10.0.22621, ISPC 1.24, UHT, Unreal Build Accelerator local executor
(4 physical cores, 3 parallel actions under a 4.5 GB memory budget).

Two defects had to be cleared to get here, both found by real compilation, not guesswork:

1. `fatal error C1083: Cannot open include file: 'TBW.h'` — module header lived at the
   module root, which UBT does not publish as an include path. Moved to `Public/`/`Private/`.
   Offline audit rule **E8** now catches this class before it ever reaches a build machine.
2. Build tooling defects on the harness side (system .NET assumption, terminating
   NativeCommandError, Tee-Object leaking into a return value). See `docs/WINDOWS_SETUP.md`.

## PIE result

**PENDING.** The build is done; the gameplay pass is not. Checklist: `docs/PHASE2_PIE_CHECKLIST.md`.

## Tests performed

| Test | Here |
|---|---|
| Architecture audit | Done |
| EngineAssociation still 5.8 | Done |
| Single `.uproject` | Done |
| Structural compile of new files (syntax review) | Done |
| TheBetrayedWillEditor Win64 Development | **NOT RUN** |
| PIE WASD / look / sprint / crouch / E / door | **NOT RUN** |
| Identity swap `tbw.Identity.Set Raynor` | **NOT RUN** |
| Arabic title still Slate/DejaVu | Source verified unchanged path |

## Problems encountered

- No UE 5.8 toolchain here — cannot claim Succeeded compile or PIE.

## Problems fixed

- Movement slide risk (higher brake/friction).
- Unspecified pitch range.
- Interact miss when hitting a child mesh.
- No reusable interact-state toggle to validate the foundation.

## Remaining warnings

- `FSlateFontInfo(FString)` deprecation — **left in place** (Arabic HUD works).
- Editor ~29 FPS on P3000 — **not “fixed”**; baseline doc says measure cooked build first.

## Performance baseline

See `PHASE2_PERFORMANCE_BASELINE.md`. Numbers: **not captured here**.

## Follow-up pass — 2026-08-18 (offline audit)

Phase 2 source was audited before spending a build slot on the laptop. See
`docs/PHASE2_AUDIT.md`. Four real defects were found and fixed:

1. `UTBWInteractorComponent::TickComponent()` was declared but never defined — that alone
   would have failed the link (`LNK2019`).
2. Interact focus was never refreshed outside `TryInteract()`, so the prompt could not appear.
3. `AddCrouchGate()` and `AddTestDoor()` were never called — the two Phase 2 test rigs did
   not exist at runtime.
4. Runtime greybox boxes were transformed after being made `Static` (mobility log spam), and
   the test door never stopped interpolating.

New tooling:

- `tools/validate_phase2.py` — offline pre-compile audit (link completeness, UHT structure,
  UTF-8 / CP1252 traps, Build.cs module coverage, dead wiring). Currently **PASS**.
- `tools/phase2_build_and_check.ps1` — one command on Windows: generate project files, build
  `TheBetrayedWillEditor Win64 Development`, save the log, print a copy-pasteable summary.
- `docs/PHASE2_PIE_CHECKLIST.md` — the fillable Phase 2 Definition of Done.

## Git

Phase 2 feel work + this audit pass live on `arena/019ffc4c-sultanpalace-game`.
