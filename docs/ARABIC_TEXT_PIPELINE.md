# Arabic text / font pipeline (Phase 1 HUD)

**Status:** Source fix applied. Runtime PIE not executed on this host.

## Affected files (inspected before edit)

| File | Finding | Action |
|---|---|---|
| All `Source/**/*.h,*.cpp` | Valid UTF-8. Arabic only in copyright comments except HUD. | No recode. |
| `Source/TBW/Private/UI/TBWHUD.cpp` | Only **runtime** Arabic string. Drawn with `AHUD::DrawText(..., nullptr)` → TinyFont / DroidSansFallback. | Title now Slate + DejaVu. Literal stored as `\uXXXX` so MSVC cannot CP1252-mojibake it. |
| `Config/DefaultGame.ini` `CopyrightNotice` | Valid UTF-8 Arabic. Not drawn in PIE HUD. | Unchanged. |
| Docs / README | Valid UTF-8 Arabic. | Unchanged. |

No file on disk contained stored Mojibake (`ÙˆØµÙŠØ©`). The garbled PIE text came from **font fallback** (and/or MSVC source charset on `TEXT("وصية...")`).

## Fix

1. `Content/TBW/UI/Fonts/DejaVuSans.ttf` — licensed face with Arabic glyphs.
2. Title drawn by `STextBlock` with `ETextShapingMethod::Auto` and `ETextFlowDirection::Auto` (Unreal ICU/HarfBuzz, no custom RTL).
3. Canvas debug lines stay English-only so DroidSansFallback is not asked for Arabic.

## Not run here

Unreal Editor is not installed on this machine. Compile / PIE / visual confirmation of Arabic must be done on the Windows UE 5.8 PC.
