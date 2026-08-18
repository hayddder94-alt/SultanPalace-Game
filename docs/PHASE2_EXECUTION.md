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

## Build result

**NOT RUN — Unreal Editor / UBT not available on this host.**  
User machine previously compiled Phase 1 successfully. Phase 2 is incremental C++ on the same module.

## PIE result

**NOT RUN** on this host.

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
