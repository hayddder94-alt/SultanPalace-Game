# PERFORMANCE TARGETS (PHASE 0)

**Status:** Initial targets. **Do not optimize prematurely.**  
**First job:** establish baselines in Phase 1, then again on the grey wing, then on the VS.

---

## 1. What we are aiming at

| | Target |
|---|---|
| Resolution (reference) | 1080p |
| Frame (High, reference PC) | 60 fps, 16.6 ms GPU |
| Frame (Low, min PC) | Playable 30+ , readable, Lumen off |
| Game thread explore | < 8 ms typical |
| Hitch on authored transition | < 100 ms player-facing |
| VS critical spikes | None during hall cine, annex stealth, court fight |

“Reference PC” will be written from Phase 13 data. Until then assume: 8-core CPU, 16 GB RAM, RTX 3060-class or equal, SSD, DX12.

“Min PC” guess for planning: GTX 1060 / RX 580-class, 8 GB RAM is *not* a promise. The VS must still expose Low/Med/High/Epic.

---

## 2. Scalability (skeleton only)

| Preset | Lumen | Nanite | Shadows | RT |
|---|---|---|---|---|
| Low | Off (baked/SSGI) | On if free, else off | Low | Off |
| Medium | Software, tight | On | Med | Off |
| High | Software | On | Virtual SM | Off |
| Epic | Software or HWRT later | On | High | Optional later |

Hardware ray tracing stays **off** for the slice. Do not make the hall depend on it.

---

## 3. Content budgets (VS wing)

From architecture + wing spec:

- Visible NPCs ≤ 12 in the hall cine, ≤ 6 in morning play  
- Full AI brains ≤ 4  
- Shadow-moving lights: few  
- No world partition  
- No 4K unique wall textures on every module  

---

## 4. Baselines to capture (after first PIE on UE 5.8)

Not captured on the planning host (no editor). Fill this table on the first Development Editor session:

| Scene | GPU ms | GT ms | RT ms | Notes |
|---|---|---|---|---|
| Empty template + pawn |  |  |  | |
| Hall (sandbox) |  |  |  | |
| Annex |  |  |  | |
| Court |  |  |  | |
| HUD + flag list |  |  |  | |

## 4b. Original capture list

On `L_Dev_Sandbox`, then on grey `L_VS_Palace`:

1. Empty map, default pawn, 1080p High — GPU / GT / render thread  
2. Hall with 8 seated bodies (cine)  
3. Annex with 2 AI + detection  
4. Court fight 1v2  
5. Pause + pin-board  

Write the numbers into this file. Do not tune Nanite/Lumen for a week because a number looked high on an empty map.

---

## 5. Profiling tools

Unreal Insights, `stat unit`, `stat fps`, GPU profiler. One named person (Technical seat) owns a weekly capture after the wing exists. No optimization task without a capture attached.
