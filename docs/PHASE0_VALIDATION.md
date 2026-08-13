# PHASE 0 TECHNICAL VALIDATION

**Date:** 2026-08-13  
**Host:** Linux planning sandbox (no Unreal Editor, no Epic Launcher, no MSVC)

---

## 1. What could be validated here

| Check | Result |
|---|---|
| `TheBetrayedWill.uproject` is valid JSON | **PASS** — EngineAssociation `5.7`, module `TBW`, plugin Enhanced Input only, target Windows |
| Target files exist (Game + Editor) | **PASS** — `IncludeOrderVersion.Unreal5_7` |
| `TBW.Build.cs` exists, no GAS/Steam/StateTree | **PASS** |
| C++ module entry + log categories | **PASS** — `IMPLEMENT_PRIMARY_GAME_MODULE` |
| GameInstance / GameMode / Controller / Character | **PASS** — stubs only |
| Identity component + data asset type | **PASS** — shared pawn architecture |
| WorldState subsystem flags | **PASS** — name→int bus |
| Content tree matches Phase 0 brief | **PASS** — under `Content/TBW/...` |
| Source tree has system folders without fake systems | **PASS** — Combat/AI/Dialogue/Quest/Save are README-only |
| Config maps, renderer, packaging keys | **PASS** |
| `.gitignore` excludes Binaries / Intermediate / Saved / DDC | **PASS** (updated) |
| `.gitattributes` LFS policy for uasset/umap | **PASS** |
| No marketplace pack as a dependency | **PASS** |
| No Steamworks | **PASS** |
| No seven-location maps | **PASS** |
| No campaign implementation | **PASS** |

Automated: `python3 tools/validate_phase0.py`

---

## 2. What could NOT be validated on this host

| Check | Status | When |
|---|---|---|
| Project opens in Unreal Editor | **BLOCKED** — editor not installed | Phase 1 hour 1 |
| C++ module compiles with UBT | **BLOCKED** — no engine toolchain | Phase 1 hour 1 |
| Editor launches this .uproject | **BLOCKED** | Phase 1 |
| Empty test map PIE | **BLOCKED** — `L_Dev_Sandbox` is created in-editor | Phase 1 |
| Packaging pipeline runs | **BLOCKED** — script will be written in Phase 1; config keys are present | Phase 1 |
| Live plugin audit inside the editor | **DEFERRED** — uproject plugin list is clean | Phase 1 |

This is an environment limit, not an incomplete Phase 0 design. The lock document already states the first live compile is the first hour of Phase 1.

Default maps currently point at `/Engine/Maps/Templates/OpenWorld` so a first open does not hard-fail on a missing `L_Dev_Sandbox`. Phase 1 replaces that path the same day the sandbox map is created.

---

## 3. Plugin policy (validated by inspection)

**Enabled:** Enhanced Input.

**Explicitly disabled / absent:** GameplayAbilities, CommonUI, OnlineSubsystemSteam, StateTree, Water, ChaosVehicles, MetaHuman, PCG, Modeling as a runtime dependency.

---

## 4. Architecture validation (design)

The skeleton matches `TECHNICAL_ARCHITECTURE.md` with Phase 0 reductions:

- One module `TBW`  
- Flags as the bus  
- One pawn + identity component  
- No GAS  
- No custom renderer  
- Folders ready for later phases without implementing them  

Deviation: Content root is `Content/TBW/...` as required by the Phase 0 brief (architecture doc originally said `Content/Characters` etc.). Architecture decision log is updated. Both trees are the same idea; the brief wins.
