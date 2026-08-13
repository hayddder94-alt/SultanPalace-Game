# The Betrayed Will / وصية الغدر

Cinematic third-person story-driven action adventure.  
Ancient Babylon. A family. A will. A betrayal.

**Platform:** Windows PC  
**Distribution:** Steam  
**Engine:** **Unreal Engine 5.7** (locked — see [`docs/ENGINE_VERSION_LOCK.md`](docs/ENGINE_VERSION_LOCK.md))  
**Playtime:** 6–8 hours  
**Scope:** Small-to-medium independent / AA-style. Not an open world. Not an RPG. Not a live service.

This repository is the production home of the game. It is **not** an Islamic-palace simulator.

## Current status

**Phase 1 — Technical foundation — code-first COMPLETE.**  
Live Unreal compile and Windows `.exe` require a machine with UE 5.7.  
Full campaign implementation has **not** started.

- Engine: UE **5.7** locked  
- Version: `0.1.0-phase1`  
- Player: walk / sprint / crouch / look / examine  
- `L_Dev_Sandbox` runtime stand-in (floor, lights, two flag tests)  
- Console: `tbw.Flags.Set`, `tbw.Flags.List`  
- Package script: `tools/package_win_dev.ps1`

Read: [`docs/PHASE1_REPORT.md`](docs/PHASE1_REPORT.md) · [`docs/README.md`](docs/README.md)

## Project layout

```
TheBetrayedWill.uproject
Source/TBW/          C++ game module
Content/TBW/         game content (East Wing only for the slice)
Config/
docs/                production bible
tools/validate_phase0.py
```

## Recommended next action

On a Windows PC with Unreal Engine **5.7.x**: generate project files, PIE the sandbox, then `tools/package_win_dev.ps1`.  
Do not build Chapters 1–8. Do not add combat until walking the wing feels right.

## License / legal

No copyrighted assets. Shipping title: **The Betrayed Will** / **وصية الغدر**.
