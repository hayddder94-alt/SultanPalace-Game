# Content / TBW

All shipped game content lives under this root.

| Folder | Phase that fills it | Notes |
|---|---|---|
| `Core/` | 1 | GameMode / GI blueprints if needed |
| `Characters/` | 2, 10 | Four primaries first. No crowd. |
| `Combat/` | 3 | Attack data, weapons. Three player weapons total. |
| `AI/` | 4 | Guard BP, StateTree. Not in Phase 0. |
| `Dialogue/` | 6 | Widgets + sample conversations |
| `Quests/` | 7 | VS segments only until Phase 11 |
| `Interaction/` | 5 | Doors, clues, lamps |
| `Save/` | 8 | None required as content |
| `UI/` | 1–2, 10 | HUD, pause, dialogue |
| `Audio/` | 10 | Beds and footsteps for VS |
| `Cinematics/` | 10 | Will reading + VS close |
| `Environment/Palace/EastWing/` | 9–10 | **The only environment built for the Vertical Slice** |
| `Data/` | 1+ | Identities, clues, flags, quests |
| `Maps/Dev/` | 1 | `L_Dev_Sandbox` — create in editor |
| `Maps/VS/` | 9–10 | `L_VS_Palace` — create in editor |
| `Materials/` | 9 | Master + instances, replaceable |
| `Development/` | always | Per-user sandboxes. Do not cook. |

Do not put marketplace packs at this root. Do not build Market, Temple, Noble, Residential, or Kurnah maps in the Vertical Slice.
