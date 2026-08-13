# PHASE 1 EXECUTION

**Project:** The Betrayed Will / وصية الغدر  
**Date:** 2026-08-13  
**Owner:** Technical Director  
**Version:** `0.1.0-phase1`

This is the Phase 1 acceptance record. It separates what was implemented from what was actually run.

---

## 1. Exact Unreal version

| Field | Value |
|---|---|
| Locked minor | **5.7** |
| Exact hotfix on this host | **NOT INSTALLED — UNKNOWN** |
| Association | `"5.7"` in `TheBetrayedWill.uproject` |
| 5.8 | Not used |

The production machine must write the hotfix here on first install, for example `5.7.2`. Until that happens the lock is the minor version only.

---

## 2. Environment validation

| Check | Result |
|---|---|
| Unreal Engine launches | **NOT TESTED** — `UnrealEditor` is not on this host |
| Project opens in Editor | **NOT TESTED** |
| C++ module compiles with UBT | **NOT TESTED** |
| Editor loads without critical errors | **NOT TESTED** |
| Create / open a test map | **NOT TESTED** — no `.umap` authored; `ATBWDevSandbox` is a runtime stand-in for `L_Dev_Sandbox` |
| Development build generated | **NOT TESTED** |

Disk on this host: 20 GB free. A UE 5.7 install is larger than that. No Epic Launcher, no MSVC.

**Editor-dependent work was stopped.** Nothing below claims a live PIE or packaged exe.

---

## 3. What was implemented (source)

Foundation only.

| Requirement | Implementation |
|---|---|
| GameInstance | `UTBWGameInstance` |
| GameMode | `ATBWGameMode` — spawns `ATBWDevSandbox` |
| PlayerController | `ATBWPlayerController` + runtime IMC |
| PlayerCharacter | `ATBWPlayerCharacter` — one pawn |
| Identity | `UTBWPlayerIdentityComponent` + `FTBWIdentityFactory` (Evan default, Raynor via console) |
| World state | `UTBWWorldStateSubsystem` + `ETBWWorldFlag` |
| Input | `UTBWInputConfig` — see `INPUT_MAP.md` |
| Dev sandbox | `ATBWDevSandbox` — floor, lights, two test objects |
| Debug | `ATBWHUD` (Shipping strips overlay) + console |

**Not implemented (later phases):** combat, AI, dialogue, quest, save, inventory, Steamworks, Vertical Slice art, East Wing as the default map.

The East Wing greybox actor still exists in source (`ATBWEastWingSandbox`) for later phases. Phase 1 GameMode does **not** spawn it.

---

## 4. World-state foundation

API:

- `SetFlag` / `ClearFlag` / `HasFlag` / `GetFlag` (FName)
- `SetWorldFlag` / `ClearWorldFlag` / `CheckWorldFlag` / `GetWorldFlag` (`ETBWWorldFlag`)
- `ResetTestState`

Typed flags include the required examples:

`WillWasRead`, `RaynorDisappeared`, `EvanInvestigating`, `ClueFound_01`, `GuardAlerted`

Arbitrary FName flags still work so quests do not need a C++ change.

Console (Development / DebugGame only):

```
tbw.Flags.Set <Name> <Int>
tbw.Flags.Get <Name>
tbw.Flags.Check <Name>
tbw.Flags.Clear <Name>
tbw.Flags.List
tbw.Debug.Reset
tbw.Debug.Hud 0|1
tbw.Identity.Set Evan|Raynor
tbw.Version
```

---

## 5. L_Dev_Sandbox

No binary `.umap` was created (Editor unavailable).

Runtime stand-in `ATBWDevSandbox`:

- 20 m floor, low walls
- Directional + sky light
- Player spawn
- Pillar marker
- Test clue → `ClueFound_01`
- Test will → `WillWasRead`
- Debug HUD: FPS, coordinates, identity, flags, system status

Placeholder geometry only. Not the palace. Not final art.

---

## 6. Build configuration

| Config | Script | Run on this host |
|---|---|---|
| Development Editor | Generate from `.uproject` | No |
| Development Game Win64 | `tools/package_win_dev.ps1` | No |
| Shipping Win64 | same script `-Config Shipping` | No — scheduled as Phase 2 acceptance if still blocked |

`tools/package_win_dev.sh` exits 2 on Linux so CI cannot fake an exe.

---

## 7. Tests performed

### Structural (ran)

`python3 tools/validate_phase1.py` — **0 failed** after this update.

Checks: uproject, module, directories, naming, no GAS/Steam/quest/combat/save tokens, sandbox source present, console commands present.

### Runtime (did not run)

| Test | Status |
|---|---|
| Editor open | not possible |
| Compile | not possible |
| PIE spawn / move / look / sprint / interact | not possible |
| Flag set visible on HUD | not possible |
| Development exe launch | not possible |
| Shipping exe launch | not possible |
| FPS / memory / startup timing | not possible |

---

## 8. Tests not possible

Everything that requires Unreal Editor, UBT, MSVC, or a cooked pak.

---

## 9. Errors encountered

| Error | Outcome |
|---|---|
| No Unreal binary on host | Stopped Editor-dependent work. Documented. |
| No disk for a 5.7 install | Did not attempt a partial download. |
| Previous GameMode spawned the East Wing as the Phase 1 map | Corrected: default is now `ATBWDevSandbox`. |

---

## 10. Errors fixed

- Default sandbox is the systems test, not the palace wing.
- Input now includes Primary / Secondary (reserved, no combat).
- World state has typed `ETBWWorldFlag` plus FName.
- Debug overlay is compile-gated out of Shipping.
- Reset test state exists (`tbw.Debug.Reset`).

---

## 11. Remaining risks

1. First UBT compile may need a modifier tweak on WASD swizzle.  
2. `Template_Default` map path may differ on some 5.7 installs.  
3. Engine cube `/Engine/BasicShapes/Cube` must cook — listed in `DefaultGame.ini`.  
4. Exact 5.7.x hotfix still unknown.  
5. Packaging surprises (shader compile, missing default map) will only appear on the Windows machine.  
6. Identity swap is architecture + console only — not a player-facing feature.

---

## 12. Phase 1 acceptance gate (honest)

| Gate | Met here? |
|---|---|
| Project opens | **No** (no Editor) |
| C++ module compiles | **Source ready, compile not run** |
| No critical compile errors | **Unknown until UBT** |
| Player spawns / moves / camera / sprint / input | **Coded, not run** |
| Dev sandbox / flags / interaction | **Coded, not run** |
| Windows Development Build launches | **Script only** |
| Shared player + identity + world-state architecture | **Yes, in source** |
| Phase 1 report + engine version + limitations | **Yes** (hotfix unknown) |

**Phase 1 is NOT fully accepted.** The foundation is implemented and structurally validated. The runtime/build gate remains open until a UE 5.7 Windows machine exists.

Do not start Phase 2, combat, AI, or the Vertical Slice until that gate is actually run.

---

## 13. Architectural change (documented)

`ETBWWorldFlag` was added as a typed overlay on the existing FName map. This does not replace data-driven flags. See `TECHNICAL_ARCHITECTURE.md` decision log.
