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

---

## Answering "which GPU" without asking you to know anything

Asked ten times, never made easy. That was my failure, not yours: I kept
requesting a section of a log file and left the reading of it to you.

```
.\tools\GPU.cmd
```

One command, three answers:

1. **What is in the laptop** — from `Win32_VideoController`. Both cards.
2. **What Unreal actually chose** — grepped out of `Saved\Logs\*.log`, matching
   the several forms the engine uses (`Adapter Name`, `Chosen D3D12 Adapter`,
   `RHI Adapter`, `VendorId`) because the wording moves between versions and
   between the D3D11 and D3D12 RHIs.
3. **Whether that is the fast one**, in words, with the fix if it is not.

### Why the question will not go away

This is an Optimus laptop: an Intel HD 530 and a Quadro P3000, and *Windows*
decides which one an application gets. An earlier `-nullrhi` run logged
`GPU: Intel HD 530`.

If Unreal is on the integrated chip then every number in this document was
measured on the wrong hardware, and the plan to add seventeen skinned
characters is budgeted against a fiction.

### One caveat the tool prints for itself

`tools/selftest.ps1` runs the engine with `-nullrhi` deliberately — it renders
nothing, on purpose, so the checks are fast and headless. Those logs will never
name an adapter. Only a real run does, which is what `PLAY.cmd` gives.
