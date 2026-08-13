# PHASE 1 PLAN — TECHNICAL FOUNDATION

**Status:** Ready. **Do not start until Phase 0 is accepted.**  
**Calendar:** 3–4 weeks  
**Depends on:** Engine lock (done), skeleton (done), a machine with UE 5.7.x

---

## 1. Objectives

A clean UE 5.7 project that boots, compiles, packages a Development Windows build, and can set a world flag from the console.

---

## 2. Tasks (in order)

1. Install **UE 5.7.x** (exact hotfix written here on install day). Enable Git LFS.  
2. Generate project files. Compile `TBW` Editor and Game targets.  
3. Create `L_Dev_Sandbox` (basic floor, light, player start). Set it as GameDefaultMap / EditorStartupMap.  
4. Create Enhanced Input assets: `IA_TBW_Move`, `IA_TBW_Look`, `IMC_TBW_Default`. Assign on the player BP.  
5. Create `BP_TBW_GameMode`, `BP_TBW_Player` only if hookup needs it; C++ defaults already exist.  
6. Console cheats: `tbw.Flags.Set`, `tbw.Flags.List` on the WorldState subsystem.  
7. Confirm plugins: Enhanced Input on. Steam, GAS, StateTree, MetaHuman **off**.  
8. Scalability widget stub or engine defaults documented. Settings save can wait for Phase 8; a config.ini brightness is enough.  
9. One-button Development package script (`tools/package_win_dev.ps1`). Version file `0.1.0-phase1`.  
10. Capture the five empty-map baselines into `PERFORMANCE_TARGETS.md`.  
11. Smoke: packaged exe launches, PIE < 30 s, flag sets and logs, second machine if available.

---

## 3. Deliverables

- Compiling Editor + Game  
- `L_Dev_Sandbox`  
- Input context + two actions  
- Flag console  
- Package script  
- Updated validation log  
- No marketplace dependency  

---

## 4. Acceptance criteria (Phase 1 done when)

- [ ] Exact 5.7.x hotfix recorded  
- [ ] `TBW` compiles without warnings we do not understand  
- [ ] Editor opens this project  
- [ ] PIE into `L_Dev_Sandbox`, walk with the stub pawn  
- [ ] `tbw.Flags.Set ClaspFound 1` logs and `GetFlag` returns 1  
- [ ] Development packaged build runs on a machine without the editor  
- [ ] Plugin list matches the lock (no tourists)  
- [ ] Performance baseline numbers written  
- [ ] Still no combat system, no AI tree, no Steam, no second map district  

---

## 5. Explicitly not Phase 1

Player *feel* polish (Phase 2), combat (3), AI (4), clues (5), dialogue (6), quests (7), save slots (8), the palace wing art (9), the slice (10), the campaign (11).

If Phase 1 starts building the will-reading sequence, it has failed its own scope.
