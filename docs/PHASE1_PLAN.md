# PHASE 1 PLAN — TECHNICAL FOUNDATION

> **HISTORICAL / ARCHIVED** schedule text. Remaining live compile uses **UE 5.8**, not 5.7.

**Status:** Code-first complete. Live UBT / packaged exe pending a UE **5.8** Windows machine.  
**Calendar:** 3–4 weeks (remaining: first compile day + smoke)  
**Depends on:** Engine lock (done — now 5.8), skeleton (done)

---

## 1. Objectives

A clean UE 5.7 project that boots, compiles, packages a Development Windows build, and can set a world flag from the console.

---

## 2. Tasks

1. ~~Engine lock~~ Now **UE 5.8 only**. Exact 5.8.x hotfix written on first install day.  
2. ~~Module + targets~~ Done. First UBT generate/compile is the remaining live step.  
3. ~~Playable sandbox~~ `ATBWEastWingSandbox` injects the East Wing at runtime. Optional: save `L_Dev_Sandbox` from the editor.  
4. ~~Enhanced Input~~ Runtime `UTBWInputConfig` (IA + IMC). Editor assets optional later.  
5. ~~GameMode / Player C++ defaults~~ Done, including HUD.  
6. ~~Console cheats~~ `tbw.Flags.Set/Get/List`, `tbw.Identity.Set`, `tbw.Version`.  
7. ~~Plugin policy~~ Enhanced Input only.  
8. Scalability: engine defaults. Settings save is Phase 8.  
9. ~~Package script~~ `tools/package_win_dev.ps1`.  
10. Performance baselines: capture after first PIE (cannot on this host).  
11. Smoke on packaged exe: after first Windows cook.

---

## 3. Deliverables

- [x] `TBW` source for Editor + Game  
- [x] `ATBWDevSandbox` runtime stand-in for `L_Dev_Sandbox`  
- [x] Input context + actions  
- [x] Flag console  
- [x] Package script  
- [x] Validation `tools/validate_phase1.py`  
- [ ] Live compiled editor  
- [ ] Packaged Win64 Development exe  
- [x] No marketplace dependency  

---

## 4. Acceptance criteria

- [x] Engine minor locked at **5.8**  
- [ ] Exact 5.8.x hotfix recorded (first install)  
- [ ] `TBW` compiles without warnings we do not understand  
- [ ] Editor opens this project  
- [ ] PIE into the sandbox, walk the wing  
- [x] `tbw.Flags.Set` / `List` implemented  
- [ ] Development packaged build runs on a machine without the editor  
- [x] Plugin list matches the lock  
- [ ] Performance baseline numbers written  
- [x] Still no combat system, no AI tree, no Steam, no second map district  

---

## 5. Explicitly not Phase 1

Combat (3), AI (4), authored clues as data assets (5), dialogue (6), quests (7), save slots (8), palace art kit (9), the cinematic Vertical Slice (10), the campaign (11).
