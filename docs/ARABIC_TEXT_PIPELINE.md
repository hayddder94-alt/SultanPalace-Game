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

## Editor-log noise that is NOT a game bug

On an Arabic Windows install (`ar-IQ`) the editor's own UI is localised, and the
editor draws its own UI with `Roboto` + `DroidSansFallback`, not with our font.
That produces, repeatedly, in the Output Log:

```
LogSlate: Warning: Could not find Glyph Index 0 with codepoint U+61c,
          getting last resort font data ../../../Engine/Content/Slate/Fonts/DroidSansFallback.ttf
```

`U+061C` is ARABIC LETTER MARK — an invisible bidi control character that the
Arabic editor localisation embeds in menu and tooltip strings. It has no glyph
in any font by design.

* It concerns **editor chrome**, not our HUD.
* Our subtitles/objectives go through `STextBlock` + `DejaVuSans.ttf`.
* Confirmed absent from our data: `grep -rlP '\x{061C}' Content/TBW/Data/` → none.

Ignore it. It will not appear in a packaged build. If you want the log quiet,
switch the editor language to English in
*Edit → Editor Preferences → Region & Language*.

## The DataTable import dialog

Double-clicking `Content/TBW/Data/Objectives.json` in the Content Browser — or
accepting the *"source content files changed, import them?"* toast — opens the
**DataTable import options** dialog. **Always press إلغاء / Cancel.**

Those `.json` files are runtime data read with `FFileHelper`
(`TBWObjectiveSubsystem.cpp:25`, `TBWDialogueSubsystem.cpp:21`). They are not
assets. Importing one would create a `UDataTable` uasset that the runtime never
reads, and which would silently drift away from the file that actually ships.

`Config/DefaultEditor.ini` now sets `bMonitorContentDirectories=False`, so the
editor stops offering. Requires an editor restart to take effect.

---

## ✅ PROVEN ON SCREEN — 2026-08-19

Screenshot from a live PIE session on the Windows machine (`ar-IQ` Windows,
UE 5.8.1, `DejaVuSans.ttf` loaded — the log line
`Font face 'C:/Dev/SultanPalace-Game/Content/TBW/UI/Fonts/DejaVuSans.ttf'
subface index 0 successfully added to font face map` appears immediately
before the frame was taken):

| Element | Rendered | Verdict |
|---|---|---|
| HUD title, top-left | `THE BETRAYED WILL  /  وصية الغدر` | letters **joined**, correct medial/final forms, word order right-to-left |
| Objective, top-right | `شاهد. اعرف خوف الأب.` | joined, RTL, and **right-aligned** — `HAlign_Right` + `ETextFlowDirection::Auto` behaved |

This closes the last open question in the Arabic pipeline. Previously only the
*string* was proven (`tbw.SelfTest`: "Arabic survives JSON to subtitle"); now
the *pixels* are proven too.

Consequence: converting `docs/VERTICAL_SLICE_SCRIPT.md` VS-02 … VS-18 into JSON
under `Content/TBW/Data/Dialogue/` is unblocked. It was deliberately held back
until one segment rendered correctly on a real screen.

Still unproven: Arabic inside the **subtitle** widget during an actual dialogue
scene (`tbw.Dialogue.Play VS01_OrinLastWords`) — same widget class and same
font as the objective, so expected to pass, but expected is not measured.

### 2026-08-19, second occurrence: the fix was in the wrong file

The DataTable import dialog came back, this time on
`Content/TBW/Data/Dialogue/VS02_TheWillReading.json`.

`bMonitorContentDirectories=False` had been written into `Config/DefaultEditor.ini`.
That setting belongs to `UEditorLoadingSavingSettings`, which is declared
`UCLASS(config=EditorPerProjectUserSettings)` — so the engine never read it
there. The line existed, looked right in a diff, and did nothing. The check in
`tools/validate_narrative.py` then read the same wrong file and reported green
over a live defect, which is worse than having no check.

Correct locations:

| File | In git? | Role |
|---|---|---|
| `Config/DefaultEditorPerProjectUserSettings.ini` | **yes** | project default for every clone |
| `Saved/Config/WindowsEditor/EditorPerProjectUserSettings.ini` | no | the per-machine copy the editor writes; **overrides the above** |

Because the editor has already written its own copy on this machine, the
project default will not win on its own. Untick it once:

**Edit → Editor Preferences → Loading & Saving → Auto Reimport →
Monitor Content Directories** (`مراقبة أدلة المحتوى`).

There is no correct row type to pick in that dialog. `FarmVisualDataRow`,
`GameplayTagTableRow` and the rest are unrelated engine structs. The answer is
Cancel, every time.
