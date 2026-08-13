# The Betrayed Will / وصية الغدر

Cinematic third-person story-driven action adventure.  
Ancient Babylon. A family. A will. A betrayal.

**Platform:** Windows PC  
**Distribution:** Steam  
**ENGINE: Unreal Engine 5.8**  
**Playtime:** 6–8 hours  
**Scope:** Small-to-medium independent / AA-style. Not an open world. Not an RPG. Not a live service.

This repository is the production home of the game. It is **not** an Islamic-palace simulator.

## Current status

**Authoritative engine: Unreal Engine 5.8 only.**  
Version: `0.1.0-phase1-ue58`  
One project: `TheBetrayedWill.uproject` at the repository root.

Do **not** install or use Unreal Engine 5.6 or 5.7 for this project.

- Player: walk / sprint / crouch / look / examine  
- `L_Dev_Sandbox` runtime stand-in (floor, lights, two flag tests)  
- Console: `tbw.Flags.Set`, `tbw.Flags.List`  
- Package script: `tools/package_win_dev.ps1` (requires UE 5.8 on Windows)

Read: [`docs/UE58_FULL_MIGRATION_REPORT.md`](docs/UE58_FULL_MIGRATION_REPORT.md) · [`docs/README.md`](docs/README.md)

## Project layout

```
TheBetrayedWill.uproject
Source/TBW/
Content/TBW/
Config/
docs/
tools/
VERSION
```

`backups/` is **ARCHIVED / NON-AUTHORITATIVE**. Do not open those `.uproject` files as the game.

## Recommended next action

On a Windows PC with Unreal Engine **5.8**: generate project files for `TheBetrayedWill.uproject`, PIE the sandbox, then `tools/package_win_dev.ps1`.  
Do not start Phase 2. Do not start combat.

## License / legal

No copyrighted assets. Shipping title: **The Betrayed Will** / **وصية الغدر**.
