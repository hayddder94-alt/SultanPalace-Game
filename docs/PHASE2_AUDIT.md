# PHASE 2 — OFFLINE CODE AUDIT

**Date:** 2026-08-18 · **Engine:** Unreal Engine **5.8 only** · **Branch:** `arena/019ffc4c-sultanpalace-game`
**Ran on:** Arena sandbox — **no Unreal toolchain**. Nothing here was compiled or played.

## Why this pass exists

Phase 2 source was finished but never built. A UBT build on an i7-6820HQ laptop costs real
time, so before spending it we ran a structural audit over the whole module to remove the
defects that a build would have found the slow way, plus one that a build would **not** have
found at all.

Tool: `tools/validate_phase2.py` — checks link-level completeness, UHT structure, encoding,
module dependencies and dead wiring. Run it any time with `python3 tools/validate_phase2.py`.

## What the audit found

| ID | Severity | Finding | Consequence if shipped to the build machine |
|---|---|---|---|
| A1 | **Error** | `UTBWInteractorComponent::TickComponent()` declared in the header, never defined | `LNK2019 unresolved external symbol` — the editor module would fail to link. Build wasted. |
| A2 | **Bug** | Focus was only resolved inside `TryInteract()` | The interact prompt would never appear while looking at an object; it would only flash after pressing E. The whole Phase 2 interaction feel was untestable. |
| A3 | **Bug** | `ATBWDevSandbox::AddCrouchGate()` and `AddTestDoor()` were written but never called from `Rebuild()` | The two things Phase 2 was supposed to prove — crouch traversal and a door interaction — would simply not exist in PIE. |
| A4 | Risk | `AddBox()` set `Static` mobility, then scaled the component after registration | `Mobility of ... has to be 'Movable'` warning per box (about 10 per PIE session) polluting the log we need to read. |
| A5 | Risk | `ATBWTestDoor` root mobility not declared, and it interpolated its rotation every tick forever | Static-mobility warnings, plus a blocking actor being moved every frame even when settled. |
| A6 | Polish | `FTBWWorldFlags::TryParse()` / `GetKnownNames()` existed but nothing used them | The typed flag enum was decorative; `tbw.Flags.Set` accepted typos silently. |

## Fixes applied

| ID | Fix | Files |
|---|---|---|
| A1 + A2 | Implemented `TickComponent`, throttled to `FocusRefreshInterval = 0.05 s` (20 Hz) and calling `RefreshFocus()`. `TryInteract()` still refreshes on press, so the trace is always fresh. | `Interaction/TBWInteractorComponent.h/.cpp` |
| A3 | `Rebuild()` now calls `AddCrouchGate()` and `AddTestDoor()`. Sandbox label updated to `PHASE 2 PLAYER-FEEL TEST`. | `World/TBWDevSandbox.cpp` |
| A4 | `AddBox()` sets `Movable` → applies mesh/scale/material → locks back to `Static`. | `World/TBWDevSandbox.cpp` |
| A5 | Door root explicitly `Movable`; `Tick` early-outs once the yaw has settled within 0.05°. | `Interaction/TBWTestDoor.cpp` |
| A6 | `tbw.Flags.Set` warns on an unknown flag name (still sets it, ad-hoc flags stay legal). `tbw.Flags.List` now also prints the typed flag table with current values. | `Core/TBWConsoleCommands.cpp` |

**Nothing else changed.** No combat, no AI, no dialogue, no quests, no save, no Steam.
The Arabic HUD path, `DejaVuSans.ttf` and the Slate title were not touched.

## Audit result after fixes

```
ERRORS   0
WARNINGS 1   FSlateFontInfo(FString) deprecation — accepted technical debt, Arabic HUD works
RESULT   PASS (structural only)
```

Also re-ran: `tools/validate_ue58.py` → PASS, `tools/validate_phase1.py` → 0 structural failures.

## What this audit does NOT prove

- It is **not** a compile. MSVC and UHT can still reject something a regex cannot see.
- It is **not** PIE. Movement feel, camera collision and the crouch gate geometry are
  judgements only a human can make, on the target hardware.
- Performance numbers remain empty.

Build status on this host: **NOT RUN.** PIE status: **NOT RUN.**

## Next step (single, concrete)

On the Windows UE 5.8 machine:

```powershell
git pull origin arena/019ffc4c-sultanpalace-game
.\tools\phase2_build_and_check.ps1
```

then work through `docs/PHASE2_PIE_CHECKLIST.md` and paste back the SUMMARY block plus the
checklist verdict. Phase 3 scope is discussed only after that.
