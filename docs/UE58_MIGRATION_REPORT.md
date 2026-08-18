> **HISTORICAL DOCUMENT.** Kept as a record of the 5.7 to 5.8 migration.
> The engine is now locked to **5.8.1** and every 5.6/5.7 file has been deleted
> from the repository. See `docs/ENGINE_VERSION_LOCK.md` for the live truth.

# UE 5.8 MIGRATION REPORT

> **HISTORICAL / ARCHIVED.** First dual-tree attempt (root 5.7 + `TheBetrayedWill_UE58/`).  
> **Superseded** by `UE58_FULL_MIGRATION_REPORT.md`. The repository is now a **single UE 5.8 project** at the root.

**Project:** The Betrayed Will / وصية الغدر  
**Date:** 2026-08-13  
**Decision:** Explicit architecture change — target **Unreal Engine 5.8**, not 5.7, not 5.6.  
**Reason given:** 5.8 is already installed on the production machine; avoid a second engine install.

---

## 1. Trees

| Tree | Path | EngineAssociation | Role |
|---|---|---|---|
| Live 5.7 source | repository root | **5.7** | Untouched gameplay/config lock |
| 5.7 backup | `backups/UE57_preserved/ (DELETED 2026-08-18)` | **5.7** | Frozen snapshot + `BACKUP_MANIFEST.txt` |
| 5.8 migration copy | `TheBetrayedWill_UE58/` | **5.8** | Only place EngineAssociation was changed |

The 5.7 source state was **not** overwritten.

---

## 2. Compatibility review (evidence only)

Sources consulted:

- 5.7→5.8 upgrade notes: no hard breaking C++ changes for a well-structured project.  
- C++ migration note (Jun 2026): set `DefaultBuildSettings = V7` and `IncludeOrderVersion = Unreal5_8`.  
- Preview notes: `SetMovementMode` and a `FText::FromStringTable` overload deprecated. **We do not call either.**

### 5.7-specific items found in our tree

| Item | Where | Action in 5.8 copy |
|---|---|---|
| `"EngineAssociation": "5.7"` | `.uproject` | Changed to `"5.8"` |
| `EngineIncludeOrderVersion.Unreal5_7` | both `Target.cs` | Changed to `Unreal5_8` |
| `BuildSettingsVersion.V5` | both `Target.cs` | Changed to `V7` (documented 5.8 default) |
| `TBW_ENGINE_LOCK "5.7"` | `TBWVersion.h` | `"5.8"` |
| Docs / validators expecting 5.7 | copy only | Updated in the copy |

### C++ / systems reviewed — **no code change**

Enhanced Input (`UInputMappingContext::MapKey`, modifiers), `UWorldSubsystem`, `ACharacter` crouch/sprint, `AHUD::DrawHUD`, `FAutoConsoleCommand*`, interact interface, identity component, `ATBWDevSandbox`.

No invented API rewrites. No plugin added. No GAS, Steam, combat, AI, dialogue, quest, or save.

### Windows toolchain (not applied here)

5.8 expects a newer MSVC toolset than 5.7. That is a **production-machine** install item, not a source change.

---

## 3. Exact Unreal Engine 5.8 version / hotfix

| Field | Value |
|---|---|
| Target minor | **5.8** |
| Exact hotfix on **this** host | **NOT INSTALLED — UNKNOWN** |
| Exact hotfix on the production machine | **NOT VERIFIED FROM THIS HOST** |

This Arena sandbox has no `UnrealEditor`, no Epic Launcher, and ~20 GB free (too small for an engine).  
**Generate project files, compile, PIE, and package were not run here.** Do not treat them as passed.

On the production machine, write the banner string (Help → About Unreal Editor) into this table, e.g. `5.8.0-XXXXXXX+++UE5+Release-5.8`.

---

## 4. Required runtime steps — status

| Step | Status on this host |
|---|---|
| 1. Generate UE 5.8 project files | **NOT RUN** |
| 2. Compile `TBW` with the 5.8 toolchain | **NOT RUN** |
| 3. Launch Unreal Editor | **NOT RUN** |
| 4. Open `L_Dev_Sandbox` | **NOT RUN** (runtime actor `ATBWDevSandbox` still stands in; no `.umap`) |
| 5. PIE | **NOT RUN** |
| 6. Move / Look / Sprint / Crouch / Interact / Primary / Secondary / flags / debug HUD | **NOT RUN** |
| 7. Windows Development Build | **NOT RUN** — script in the copy points at `UE_5.8` / `UE58_ROOT` |
| 8. Launch packaged exe | **NOT RUN** |

Structural check of the 5.8 copy: see below.

---

## 5. Structural validation of the 5.8 copy

Ran: `python3 TheBetrayedWill_UE58/tools/validate_phase1.py`

Result recorded at commit time in the session log.

Root 5.7 tree validators still expect `5.7` and still pass on the preserved project.

---

## 6. Errors / warnings

| Item | Detail |
|---|---|
| Engine missing here | Cannot compile or package. Not a code defect. |
| Migration not accepted | Success criteria require compile + PIE + packaged Dev launch. Those did not happen on this host. |
| Possible first-compile warning | If `BuildSettingsVersion.V7` is rejected by an older 5.8 preview, fall back to `V5` and record it — do not guess further. |
| Possible include-order warning | If `Unreal5_8` enum is missing on a given 5.8 build, use `EngineIncludeOrderVersion.Latest` and record the exact compiler error. |

No C++ compile errors were observed because no compile ran.

---

## 7. Gameplay architecture

Unchanged. One pawn, identity component, world flags, Dev sandbox, Enhanced Input. No Phase 2, no combat, no Vertical Slice.

---

## 8. Success criteria

The migration is successful **only** when, on a machine with UE 5.8:

1. `TBW` compiles  
2. Editor launches  
3. PIE works in the sandbox  
4. Packaged Development Build launches  

**That has not happened on this host. STOP.**

Next authorized act: open `TheBetrayedWill_UE58/TheBetrayedWill.uproject` with the installed 5.8, run the list above, and append the hotfix string and any UBT errors to this file. Do not start Phase 2.
