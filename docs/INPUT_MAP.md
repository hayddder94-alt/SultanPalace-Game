# INPUT MAP — PHASE 2

**Status:** Keyboard/mouse first. Gamepad keys exist so the architecture is ready.

All actions are Enhanced Input, created at runtime by `UTBWInputConfig` (`IMC_TBW_Default`).

| Action | Type | Keyboard / mouse | Gamepad | Phase 2 behavior |
|---|---|---|---|---|
| `IA_TBW_Move` | Axis2D | WASD | Left stick | Camera-yaw relative. Deadzone 0.12. |
| `IA_TBW_Look` | Axis2D | Mouse | Right stick | Yaw 0.85 / pitch 0.70. Pitch clamped −55°…70°. |
| `IA_TBW_Sprint` | Digital | Left Shift | L3 | Hold. Ignored while crouched or airborne for speed. |
| `IA_TBW_Crouch` | Digital | Left Ctrl / C | B / Circle | Toggle (hold if `bCrouchToggle` is false). |
| `IA_TBW_Interact` | Digital | E | A / Cross | Focused interactable. 0.18 s cooldown. |
| `IA_TBW_Pause` | Digital | Esc / P | Start | Toggle pause. |
| `IA_TBW_Primary` | Digital | LMB | RT / R2 | **Reserved** — combat Phase 3. No gameplay. |
| `IA_TBW_Secondary` | Digital | RMB | LT / L2 | **Reserved** — combat Phase 3. No gameplay. |

No jump. No bow. No radial menu.

Editor assets `IA_TBW_*` / `IMC_TBW_Default` may replace the runtime objects later by assigning `OverrideMappingContext` on `ATBWPlayerController`.
