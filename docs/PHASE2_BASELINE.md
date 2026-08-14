# PHASE 2 BASELINE

**Project:** The Betrayed Will / وصية الغدر  
**Date:** 2026-08-14  
**Engine:** Unreal Engine **5.8 only**  
**Branch:** `arena/019ffc4c-sultanpalace-game`  
**Pre-Phase-2 tip:** `039f418` — Arabic HUD fix  
**Hardware (dev machine, user-confirmed):** i7-6820HQ / 16 GB / Quadro P3000 6 GB / Win64

This file records the system **before** Phase 2 feel work. Phase 2 must not replace these classes.

---

## Current architecture

One pawn. One identity component. Runtime Enhanced Input. World flags. Dev sandbox greybox. No combat.

```
ATBWGameMode
  └── ATBWDevSandbox          runtime L_Dev_Sandbox
ATBWPlayerController
  └── UTBWInputConfig         IMC built in C++
ATBWPlayerCharacter           THE only pawn
  ├── USpringArmComponent
  ├── UCameraComponent
  ├── UStaticMeshComponent    preview body
  ├── UTBWPlayerIdentityComponent
  └── UTBWInteractorComponent
UTBWWorldStateSubsystem
ATBWHUD                       Canvas debug + Slate Arabic title
```

## Existing inputs

| Action | Binding | Behavior at Phase 1 |
|---|---|---|
| Move | WASD / L-stick | Camera-yaw relative |
| Look | Mouse / R-stick | Unclamped beyond PlayerCameraManager defaults |
| Sprint | Shift / L3 hold | Sets MaxWalkSpeed if not crouched |
| Crouch | Ctrl/C toggle | Character::Crouch |
| Interact | E / Face South | Camera sphere sweep |
| Pause | Esc | UGameplayStatics pause |
| Primary / Secondary | LMB/RMB | Log only — reserved |

## Existing classes / components

`ATBWPlayerCharacter`, `ATBWPlayerController`, `UTBWPlayerIdentityComponent`, `UTBWIdentityData`, `FTBWIdentityFactory`, `UTBWInputConfig`, `UTBWInteractorComponent`, `ATBWInteractableActor`, `ITBWInteractable`, `ATBWDevSandbox`, `ATBWEastWingSandbox` (not spawned), `ATBWHUD`, `UTBWWorldStateSubsystem`, `ETBWWorldFlag`.

## Dependencies (`TBW.Build.cs`)

Core, CoreUObject, Engine, InputCore, EnhancedInput, UMG, Slate, SlateCore, GameplayTags, NavigationSystem, LevelSequence, MovieScene, CinematicCamera.  
Runtime dependency: `Content/TBW/UI/Fonts/DejaVuSans.ttf`.  
No GAS. No Steam. No StateTree.

## Known warnings / debt

| Item | Severity | Phase 2 action |
|---|---|---|
| `FSlateFontInfo(FString path)` may warn deprecated | Low | **Leave.** Arabic HUD works. |
| Canvas debug uses default font (English only) | Low | Leave |
| No explicit move-state enum | Med | Add a tiny enum |
| Braking 1400 / friction 8 can slide | Med | Tune |
| Pitch not explicitly limited on PC | Med | Clamp ViewPitch |
| Interact ignores parent if child mesh hit | Low | Walk owner chain |
| No test door | Med | Add technical door |
| PIE FPS ~29 reported by user (editor) | Info | Measure, do not strip Lumen |

## Current build status (user machine)

Editor compile **Succeeded**. PIE **works**. Movement / look / sprint / crouch / interact / Evan / Arabic title **confirmed**.  
This host: Unreal **not installed** — Phase 2 compile/PIE marked NOT RUN here.
