# PHASE 2 — PIE CHECKLIST (fill and paste back)

**Project:** The Betrayed Will / وصية الغدر · **Engine:** Unreal Engine **5.8 only**
**Target:** `TheBetrayedWillEditor` · Win64 · Development
**Machine:** i7-6820HQ · 16 GB · Quadro P3000 6 GB

> This checklist is the **Phase 2 Definition of Done**. Phase 3 (combat) does not start
> until every REQUIRED row is `PASS` or explicitly waived by you.

## How to run

```powershell
git pull origin arena/019ffc4c-sultanpalace-game
.\tools\phase2_build_and_check.ps1
# then open TheBetrayedWill.uproject in UE 5.8 and press Play (PIE)
```

## 0 · Build

| # | Check | Expected | Result |
|---|---|---|---|
| 0.1 | Engine version reported by the script | `5.8.x` | |
| 0.2 | Build exit code | `0` | |
| 0.3 | Errors in summary | `0` | |
| 0.4 | New warnings (other than `FSlateFontInfo`) | none | |

## 1 · Boot

| # | Check | Expected | Result |
|---|---|---|---|
| 1.1 | PIE starts in the dev sandbox | grey floor, walls, pillar | |
| 1.2 | Output log line | `The Betrayed Will 0.1.0-... Engine lock 5.8 ... L_Dev_Sandbox ready` | |
| 1.3 | Player spawns standing on the floor, not falling | yes | |
| 1.4 | Arabic title top-left | `وصية الغدر` readable, joined, right-to-left | |
| 1.5 | No `Mobility ... has to be 'Movable'` spam in the log | none (fixed this pass) | |

## 2 · Movement feel — REQUIRED

| # | Check | Expected | Result |
|---|---|---|---|
| 2.1 | WASD is camera-relative | W always goes where the camera looks | |
| 2.2 | Start of movement | brisk, no long ramp | |
| 2.3 | Release of key | stops in well under a metre, no ice-skating | |
| 2.4 | Diagonal movement | no speed boost, no snapping | |
| 2.5 | Tiny stick/key nudge | ignored (deadzone 0.12), no jitter | |
| 2.6 | Shift sprint | visibly faster, camera pulls back slightly | |
| 2.7 | Sprint while crouched | does NOT apply | |
| 2.8 | Ctrl crouch (toggle) | capsule shrinks, speed drops | |
| 2.9 | Character rotation | turns toward movement smoothly, no spin | |

## 3 · Camera — REQUIRED

| # | Check | Expected | Result |
|---|---|---|---|
| 3.1 | Look up limit | stops at +70°, no flip | |
| 3.2 | Look down limit | stops at −55°, no floor clipping | |
| 3.3 | Back into a wall | boom shortens, camera never enters geometry | |
| 3.4 | Leave the wall | boom returns smoothly, no pop | |
| 3.5 | Crouch | camera lowers and tightens | |

## 4 · Interaction — REQUIRED

| # | Check | Expected | Result |
|---|---|---|---|
| 4.1 | Look at the gold test clue | prompt appears **while looking**, before pressing E | |
| 4.2 | Look away | prompt disappears | |
| 4.3 | Debug HUD focus line | `focus <ActorName>` / `focus none` | |
| 4.4 | Press E on the clue | log `Interact: ...`, flag `ClueFound_01 = 1` | |
| 4.5 | Press E on the will | flag `WillWasRead = 1` | |
| 4.6 | Mash E | no double-fire (0.18 s cooldown) | |

## 5 · Phase 2 test rigs — REQUIRED (were dead until this pass)

| # | Check | Expected | Result |
|---|---|---|---|
| 5.1 | `CROUCH GATE` label exists at roughly X −700 | visible | |
| 5.2 | Walk into the gate standing | blocked by the lintel | |
| 5.3 | Crouch and move through | passes cleanly, no capsule snag | |
| 5.4 | Stand up under the lintel | stays crouched (engine prevents unstanding) | |
| 5.5 | `TEST DOOR` label exists at roughly X +500 | visible | |
| 5.6 | E on the door | rotates open smoothly, blocks then unblocks | |
| 5.7 | E again | closes | |
| 5.8 | `tbw.Flags.Get TestDoorOpen` | matches door state (1 open / 0 closed) | |

## 6 · Debug tools

| # | Check | Expected | Result |
|---|---|---|---|
| 6.1 | `tbw.Flags.List` | set flags **plus** the typed flag table | |
| 6.2 | `tbw.Flags.Set NotARealFlag 1` | warns that the name is unknown, still sets it | |
| 6.3 | `tbw.Debug.Hud` | toggles the debug overlay | |
| 6.4 | `tbw.Debug.Reset` | clears all flags | |
| 6.5 | `tbw.Identity.Set Raynor` | HUD identity changes, preview body changes | |
| 6.6 | `tbw.Identity.Set Evan` | returns to default | |
| 6.7 | `tbw.Version` | prints version + engine lock 5.8 | |

## 7 · Performance capture (numbers go into `PHASE2_PERFORMANCE_BASELINE.md`)

| Metric | How | Value |
|---|---|---|
| Viewport resolution | PIE window size | |
| Scalability preset | Settings ▸ Engine Scalability | |
| FPS standing still | debug HUD | |
| FPS walking | debug HUD | |
| FPS sprinting | debug HUD | |
| Frame time (ms) | debug HUD | |
| `stat unit` Game / Draw / GPU | console | |
| Process memory | Task Manager | |

**Policy reminder:** do not disable Nanite or Lumen because editor PIE sits near 30 FPS on a P3000.
The decision point is a cooked Development build, not the editor.

## 8 · Verdict

- [ ] All REQUIRED rows PASS → **Phase 2 DoD met**, Phase 3 scope talk can begin
- [ ] Something failed → paste the failing row numbers + log lines; fixes only, no new scope
