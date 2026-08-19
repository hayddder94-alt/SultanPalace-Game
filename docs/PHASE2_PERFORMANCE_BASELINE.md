# PHASE 2 PERFORMANCE BASELINE

**Hardware (user machine):** Intel i7-6820HQ · 16 GB RAM · NVIDIA Quadro P3000 6 GB · Samsung 512 GB SSD · Windows 64-bit  
**Project settings (unchanged):** DX12, software Lumen on, Nanite on, HW ray tracing **off**, Virtual Shadow Maps on.

## This host

Unreal Editor is **not installed** here. The following numbers were **NOT captured** in this environment.

## What to record on the first Phase 2 PIE (Windows)

| Metric | How | Value |
|---|---|---|
| Resolution | Editor viewport | *fill* |
| Scalability | Editor | *fill* |
| Rendering | Lumen software / Nanite | as shipped |
| FPS (PIE, sandbox idle) | Debug HUD | *fill* — user previously reported ~29 in editor |
| Frame time | 1000/FPS | *fill* |
| Memory | Task Manager / `stat memory` | *fill* |
| After walk+sprint+door | HUD | *fill* |

## Policy

Do **not** disable Nanite or Lumen because the editor sits near 30 FPS. Editor PIE on a P3000 is expected to be heavier than a cooked Development build. Measure cooked Dev build before any renderer cut.

Phase 2 added: one test door interp, one crouch gate (3 boxes), slightly higher camera probe. No new lights with shadows. No new post-process. Expected cost: negligible vs Phase 1 sandbox.

---

## First real measurement — 2026-08-19 (screenshot evidence)

| Metric | Value | Source |
|---|---|---|
| Map | `NewMap` (empty default map + `tbw.Dev.Sandbox` greybox: one cube, one floor) | editor tab |
| FPS | **12** | debug HUD |
| Frame time | **81.94 ms** | debug HUD |
| `stat unit` breakdown | **not captured yet** | — |

**Read this number correctly.** 82 ms for an empty grid with a single cube is
not a content cost — there is no content. Ranked by likelihood:

1. **Wrong GPU.** This is an Optimus laptop: Intel HD 530 *and* Quadro P3000.
   An earlier `-nullrhi` run already logged `GPU: Intel HD 530`. If the editor
   is rendering on the iGPU, 12 FPS in an empty Lumen scene is exactly what you
   would get, and no amount of scalability tuning will fix it.
   Check: `.\tools\collect_pie_log.cmd` → section **GPU / RHI adapter**.
   Fix: NVIDIA Control Panel → *Manage 3D Settings* → *Program Settings* →
   add `E:\UE_5.8\Engine\Binaries\Win64\UnrealEditor.exe` → **High-performance
   NVIDIA processor**. Restart the editor.
2. **Shaders still compiling.** The first PIE after a pull compiles thousands of
   shaders in the background on 8 logical cores. Check the shader-compiler count
   in the bottom-right of the editor; wait for zero, then re-measure.
3. **Editor overhead.** PIE renders the editor UI as well. A standalone or
   cooked Development run is the only honest number.

Only after 1 and 2 are eliminated does the renderer configuration become
suspect. The standing policy below still holds: do not cut Nanite or Lumen on
the strength of an editor reading.
