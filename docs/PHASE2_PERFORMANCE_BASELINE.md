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
