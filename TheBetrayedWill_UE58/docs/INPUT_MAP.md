# INPUT MAP — PHASE 1

**Status:** Keyboard/mouse first. Gamepad keys exist so the architecture is ready. Polish later.

All actions are Enhanced Input, created at runtime by `UTBWInputConfig` (`IMC_TBW_Default`).

| Action | Type | Keyboard / mouse | Gamepad | Phase 1 behavior |
|---|---|---|---|---|
| `IA_TBW_Move` | Axis2D | WASD | Left stick | Walk |
| `IA_TBW_Look` | Axis2D | Mouse | Right stick | Camera |
| `IA_TBW_Sprint` | Digital | Left Shift | L3 | Hold to run |
| `IA_TBW_Crouch` | Digital | Left Ctrl / C | B / Circle | Toggle (hold if `bCrouchToggle` is false) |
| `IA_TBW_Interact` | Digital | E | A / Cross | Examine focused object |
| `IA_TBW_Primary` | Digital | LMB | RT / R2 | **Reserved** — combat Phase 3 |
| `IA_TBW_Secondary` | Digital | RMB | LT / L2 | **Reserved** — combat Phase 3 |
| `IA_TBW_Pause` | Digital | Esc / P | Start | Toggle pause |

No jump. No bow. No radial menu.

Editor assets `IA_TBW_*` / `IMC_TBW_Default` may replace the runtime objects later by assigning `OverrideMappingContext` on `ATBWPlayerController`.
