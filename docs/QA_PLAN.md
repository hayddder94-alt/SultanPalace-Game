# QA PLAN

**Project:** The Betrayed Will / وصية الغدر  
**Status:** Locked for planning  
**Law:** A short game that crashes or softlocks is a worse product than a shorter game that finishes. QA is a phase and a habit.

---

## 1. Purpose

Prove, repeatedly:

1. The spine can be finished by a stranger.
2. Saves are sacred.
3. Combat and stealth are fair on all three difficulties.
4. EN/AR UI does not break.
5. The Windows shipping build is boringly stable.

## 2. When QA happens

| Window | What |
|---|---|
| Phases 2–8 | Engineer acceptance + a 1-page smoke per system |
| Phase 9 | First “can a friend play 15 minutes” |
| Phase 10 | Formal VS test: 10 internal runs, 3 external if possible |
| Phase 11 | Chapter sign-off when each chapter goes content-complete |
| Phases 12–14 | Full regression, the real QA phase |
| Phase 15–16 | Certification-style and Steam |

Do not wait for Phase 14 to write cases. Cases below are written now and stay.

## 3. Roles

On a small team, the Producer seat owns the sheet. A person who **did not implement the feature** runs the pass when possible. External playtesters are instructed, not left to wander without a feedback form.

## 4. Severity

| Sev | Meaning | Ship rule |
|---|---|---|
| **S** Blocker | Crash, hardlock, save wipe, unwinnable spine | Cannot ship, cannot call a phase accepted |
| **A** Major | Softlock with workaround, broken cinematic, wrong language, unreadable combat tell | Cannot ship VS or 1.0 |
| **B** Minor | Ugly, rare AI fidget, typo, temp asset in a side room | Allowed with a list |
| **C** Polish | Wish, juice, timing taste | Backlog |

If a bug destroys a save, it is **S** even if rare.

## 5. Environments

- Editor PIE (dev only)
- Packaged Development
- Packaged Shipping (from Phase 13 on)
- Hardware: at least one “min” and one “target” PC, specs written in the Steam plan after Phase 13 data
- Input: KBM, Xbox-layout pad, DualSense if affordable
- OS: Windows 10 and 11, 64-bit
- Display: 1080p and 1440p, windowed and fullscreen
- Language: English and Arabic, including a Windows locale set to AR

## 6. Smoke (run every packaged build)

Time box: **25 minutes**.

1. Fresh install / unpack. Boot to title. No crash.
2. New game, Standard. Complete 5 minutes of the current start.
3. Manual save. Kill process. Continue. Position and objective correct.
4. Die on purpose. Reload checkpoint.
5. Pause, flip language, confirm UI layout.
6. Swap input device, confirm prompts.
7. Alt+Tab, Alt+Enter, resume.
8. Quit to title, quit to OS.
9. Load the slot again.
10. Open the latest chapter door via `tbw.Quest.SkipTo` (Development only) and walk 2 minutes.

Fail any step → build is not a candidate.

## 7. System suites

### 7.1 Player / camera

- Walk, crouch, sprint, mantle at every authored mantle in the current maps
- Camera through doorways and against the canal railing
- No look-through of hero meshes
- Identity swap (when it exists): mesh, anim, combat, stealth, VO comment

### 7.2 Combat

- Each verb vs. dummy
- 1v1, 1v2, 1v3 token rule
- Death, finisher, parry success/fail
- All three difficulties on the annex fight and on Darius
- No invincible enemy, no player stuck in hit react

### 7.3 AI / stealth

The ten cases in `AI_DESIGN.md`, plus:

- Body hide / not hide
- Lamp extinguish
- Soft-reset named-brother volume fires once
- Alert does not leak across unloaded districts

### 7.4 Investigation / dialogue / quest

- Every clue can be picked once and re-read
- Auto-connect fires once
- Every flagged choice sets the documented flag
- Every segment start/end
- Every softlock fallback pointer
- SkipTo each segment, then play to the next

### 7.5 Save

- Three slots independent
- Overwrite confirm
- Autosave backup after killed process
- Version migrator refuses a future slot
- No-save volume toast
- Settings survive a deleted slot
- Cloud (Phase 16): save on A, download on B

### 7.6 UI / UX / loc

- All pause tabs
- RTL Arabic: pause, dialogue choices, board, subtitles
- No clipped text at 1080p 150% subtitle size
- Prompt icons follow last device
- Hold-to-skip
- Empty inventory state

### 7.7 Cinematics

- Each CIN id plays, sets its flags, returns control
- Skip policy honored
- No HUD during
- Audio duck / unmute
- Cannot save mid-sequence
- CIN-14 and CIN-15 cannot be skipped on first play

### 7.8 World / streaming

- Every connector
- Lighting state matches chapter
- No falling through canal
- No visible load pop on boat/door
- Navmesh: AI can path the whole of each shipped space

### 7.9 Performance / stability

- 30-minute soak in palace and in Kurnah
- Memory does not climb without bound
- Benchmark targets from Roadmap Phase 13
- No `Ensure` spam in Development logs on the spine

### 7.10 Accessibility

- Toggle crouch/sprint
- Parry assist on Story
- Subtitle background
- Colorblind detection
- Disable finisher camera
- Remap a combat key and play a fight

## 8. Spine playthrough matrix

Before 1.0, the following full runs are required:

| Run | Identity of tester | Difficulty | Language | Notes |
|---|---|---|---|---|
| 1 | Internal, knows the game | Standard | EN | Happy path |
| 2 | Internal | Story | AR | Miss optional |
| 3 | Internal | Hard | EN | Combat focus |
| 4 | External | Standard | EN | No hints |
| 5 | External | Standard | AR if available | No hints |
| 6 | Internal | Standard | EN | Kill Nofan, break LeilaTrust, kill Malik |
| 7 | Internal | Standard | EN | Spare / bind, earn Leila, expose Malik |
| 8 | Internal | Standard | EN | Mixed flags |

Runs 4–5 are the honesty test. If they bounce in the first 20 minutes, the VS gate style rule applies even late: fix feel, do not add a map.

## 9. Softlock register (must stay empty)

Maintain a living list of “player can get stuck if…” and a fallback for each. Starting list:

| Risk | Fallback |
|---|---|
| Cannot find the clasp | Boatman points after one full dock loop |
| Cannot find barracks | Soren or Yasmin line after two palace loops |
| Miss the grain clerk | Market objective mark after one market loop |
| Die forever at last stand | Checkpoint at stand start; difficulty reminder on second death |
| Raynor AI blocks a door in MQ-06c | He teleports if separated > 20 s |
| Tablet path broken if Leila withdrawn | Soren / temple public path still opens |

## 10. Regression

Any bug fix that touches Save, Identity, Quest flags, or Streaming triggers:

- Smoke
- The failed case
- One full chapter that owns the system

A spreadsheet or issue tracker is enough. Do not build a QA platform.

## 11. Crash policy

- Shipping writes a crash folder.
- Every crash in Phase 14+ is Sev S until proven unique and vanishingly rare.
- “I can’t reproduce” is not a close reason without a log.

## 12. Playtest script (external, VS and late)

Give testers:

- Nothing about the plot
- “Play until the mission complete toast or 50 minutes”
- A form: where you got lost, where it felt good, where it felt cheap, any crash, would you wishlist

Do not sit behind them and narrate. Watch if in the room. Notes over opinions.

## 13. Sign-off

A phase accepts only with:

- Smoke green on a packaged build
- Its acceptance list from the Roadmap checked
- No open S
- A written known-issues page

1.0 sign-off: Producer + Technical + Director. If one person is all three, wait 48 hours after the last fix and run the smoke one more time. Gold is a decision, not a feeling.

## 14. What QA will not do

- Balance the game into a 20-hour RPG
- Approve “temp is fine in the hall”
- Be the first people to play a chapter
- Maintain an automated UI bot as a Phase 1 project (a few smoke automations late are allowed)
