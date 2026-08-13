# The Betrayed Will — UE 5.8 migration copy

This directory is the **Unreal Engine 5.8** migration of the Phase 1 foundation.

The original **UE 5.7** project is preserved at:

- repository root (`EngineAssociation: 5.7`)
- `backups/UE57_preserved/` (complete snapshot)

Do not treat this copy as the Vertical Slice. Do not add combat, AI, dialogue, quests, save, or Steamworks here.

## Engine

- Target: **Unreal Engine 5.8**
- Exact hotfix: record on the production machine (`docs/UE58_MIGRATION_REPORT.md`)
- Not 5.6. Not 5.7.

## What changed from 5.7 (evidence-based only)

| File | Change | Evidence |
|---|---|---|
| `TheBetrayedWill.uproject` | `EngineAssociation` `5.7` → `5.8` | Required to open in 5.8 |
| `*.Target.cs` | `IncludeOrderVersion.Unreal5_7` → `Unreal5_8` | 5.7→5.8 C++ migration notes (Jun 2026) |
| `*.Target.cs` | `BuildSettingsVersion.V5` → `V7` | Same notes |
| `TBWVersion.h` / `VERSION` | lock string `5.8` | Project identity |

No gameplay C++ was rewritten. 5.7→5.8 has **no hard breaking API changes** for the systems we use (Enhanced Input, WorldSubsystem, Character, HUD).

## First actions on a machine that actually has UE 5.8

1. Right-click `TheBetrayedWill.uproject` → Generate Visual Studio project files  
2. Build Development Editor  
3. PIE (GameMode injects `ATBWDevSandbox`)  
4. Test move / look / sprint / crouch / interact / LMB / RMB / `tbw.Flags.List`  
5. `powershell -File tools/package_win_dev.ps1`  
6. Launch the staged exe  

If Generate/Compile/PIE/package are not run, the migration is **not** accepted.
