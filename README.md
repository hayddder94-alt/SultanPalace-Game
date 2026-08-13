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

**Phase 0 — Pre-production lock — COMPLETE.**  
Full campaign implementation has **not** started. Phase 1 has **not** started.

- Engine: UE **5.7** locked in `TheBetrayedWill.uproject`
- C++ module `TBW` skeleton exists (identity + flags + pawn stubs only)
- Vertical Slice script timed to **~34 designed / ~41 first-time minutes**
- Character pipeline: hybrid MetaHuman faces + custom period clothing

Read: [`docs/PHASE0_REPORT.md`](docs/PHASE0_REPORT.md) · [`docs/README.md`](docs/README.md)

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

**Stop.** Await authorization for Phase 1 (install UE 5.7.x, first live compile, `L_Dev_Sandbox`).  
Do not build Chapters 1–8. Do not build seven locations. Do not implement Steamworks.

## License / legal

No copyrighted assets. Shipping title: **The Betrayed Will** / **وصية الغدر**.
