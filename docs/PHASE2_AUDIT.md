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

## Second pass — first real compile on the target machine (2026-08-18)

The Windows build finally reached the compiler and failed on one thing:

```
Source\TBW\Private\Core\TBWConsoleCommands.cpp(16,1):
fatal error C1083: Cannot open include file: 'TBW.h': No such file or directory
```

**Cause.** `TBW.h` and `TBW.cpp` sat at the module root, `Source/TBW/`. UBT publishes a
module's `Public/` and `Private/` folders as include roots — never the module root itself
once those folders exist. So `#include "TBW.h"` resolved for nobody. Fourteen files did it.

**Fix.** Moved to the standard layout, no Build.cs hacks:

| Was | Now |
|---|---|
| `Source/TBW/TBW.h` | `Source/TBW/Public/TBW.h` |
| `Source/TBW/TBW.cpp` | `Source/TBW/Private/TBW.cpp` |

**Audit rule added — E8.** The offline checker now resolves every project `#include` the
way UBT does (Public/, Private/, the including file's own folder). Verified by reintroducing
the bug: it reports all 14 failures and names the unreachable header. This class of error
will never again reach the build machine.

Confirmed working on the target machine in this run: UE 5.8.1 at `E:\UE_5.8`, bundled
.NET 10.0 win-x64, MSVC 14.44, Windows SDK 10.0.22621, UHT, UBA local executor,
project file generation (`Result: Succeeded`).

## What this audit does NOT prove

- It is **not** a compile. MSVC and UHT can still reject something a regex cannot see.
- It is **not** PIE. Movement feel, camera collision and the crouch gate geometry are
  judgements only a human can make, on the target hardware.
- Performance numbers remain empty.

Build status on this host: **NOT RUN** (no toolchain here).
Build status on the target machine: **SUCCEEDED**, exit code 0, 0 errors, 1 known warning.
PIE status: **PENDING** — that is the next gate.

## Next step (single, concrete)

On the Windows UE 5.8 machine:

```powershell
git pull origin arena/019ffc4c-sultanpalace-game
.\tools\phase2_build_and_check.ps1
```

then work through `docs/PHASE2_PIE_CHECKLIST.md` and paste back the SUMMARY block plus the
checklist verdict. Phase 3 scope is discussed only after that.

---

## E10 — a type that is only forward declared (added 2026-08-20)

```
TBWAnimLibrary.cpp(160,34): error C2027: use of undefined type 'UAnimSingleNodeInstance'
TBWAnimLibrary.cpp(159,13): error C2737: 'bSameClip': const object must be initialized
```

Twenty minutes of build to learn that one `#include` was missing.
`Components/SkeletalMeshComponent.h` forward declares `UAnimSingleNodeInstance`,
so `GetSingleNodeInstance()` returns a perfectly good pointer — and the moment
you call a method on it the compiler needs the definition. The second error is
a cascade of the first.

**The first version of this rule did not work**, and the failure is instructive.
It searched the source for the type name. The mistake never writes the type
name:

```cpp
Mesh->GetSingleNodeInstance()->GetAnimationAsset()
```

The trigger is not naming a type — it is **calling a method on what an accessor
returned**. So the rule is keyed on the accessor:

| Call | Returns | Header it needs |
|---|---|---|
| `GetSingleNodeInstance()->` | `UAnimSingleNodeInstance` | `Animation/AnimSingleNodeInstance.h` |
| `GetAnimInstance()->` | `UAnimInstance` | `Animation/AnimInstance.h` |
| `GetSkeletalMeshAsset()->` | `USkeletalMesh` | `Engine/SkeletalMesh.h` |
| `GetCharacterMovement()->` | `UCharacterMovementComponent` | `GameFramework/CharacterMovementComponent.h` |
| `GetCapsuleComponent()->` | `UCapsuleComponent` | `Components/CapsuleComponent.h` |
| `GetStaticMesh()->` | `UStaticMesh` | `Engine/StaticMesh.h` |
| `GetSkeleton()->` | `USkeleton` | `Animation/Skeleton.h` |

Verified by deleting the include and watching E10 name the right file and line,
then restoring it.

The table is a ledger of what has actually cost this project a build, not an
attempt at completeness. Add a row the first time a C2027 gets through.

## Exit code 6 is not ours

The editor pre-flight used to `exit 6`. UnrealBuildTool also returns 6 for
ordinary compile errors, so a run with two genuine C2027s was announced as
**BUILD BLOCKED - close the Unreal editor**, which is advice for a completely
different problem. The pre-flight now exits **90**.
