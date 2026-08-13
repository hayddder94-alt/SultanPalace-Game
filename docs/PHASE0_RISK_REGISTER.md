# PHASE 0 / VERTICAL SLICE RISK REGISTER

> **HISTORICAL / ARCHIVED** risk notes from the 5.7 lock period.  
> **Current active engine is Unreal Engine 5.8.**

**Status:** Living. Review weekly once Phase 1 starts.  
**Parent:** `PROJECT_RISKS.md` (whole project)  
**This file:** risks that can kill the *slice*, not the 24-month dream.

Probability / impact: L M H. Priority: P0 / P1 / P2.

| ID | Risk | P | I | Pri | Mitigation |
|---|---|---|---|---|---|
| V01 | Host has no UE; first compile finds Target/API issues | M | M | P1 | Skeleton follows 5.7 include order. Phase 1 hour 1 is “generate + compile,” not design. |
| V02 | Temptation to install 5.8 because it is newest | M | H | **P0** | Version lock document. Refuse. |
| V03 | VS script plays over 45 minutes | H | H | **P0** | Cut list in the script. Rehearse with a timer week 1 of Phase 10. |
| V04 | Slice grows into Ch 2–3 (market, temple, breakfast) | H | H | **P0** | Scope gate Red list. Weekly stamp. |
| V05 | Raynor gameplay “just to test identity” ships in VS | M | H | **P0** | Identity data exists; apply is not exposed. Cine actor only. |
| V06 | MetaHuman look / clothing fails period test | M | M | P1 | Hybrid: custom clothes. Two-day import test. Fallback in pipeline doc. |
| V07 | Faces cannot carry the will reading | M | H | **P0** | Spend hero budget on Evan/Nofan/Orin. Shorten VS-02 rather than ship mannequins. |
| V08 | Combat feel is embarrassing | M | H | **P0** | Six verbs, 1–2 enemies, juice in Phase 3. Do not art Kurnah instead. |
| V09 | Stealth perception is noisy | H | M | P1 | Failure → combat. No mission-fail-on-seen. |
| V10 | Save works in PIE, dies packaged | M | H | **P0** | Phase 8 acceptance is packaged. Kill-process test. |
| V11 | Softlock on clasp / roster / study | H | H | **P0** | Fallbacks written in the script. QA cases. |
| V12 | Plugin tourism (Water, GAS, Steam, PCG) | M | H | **P0** | Build.cs comments. Scope gate Red. |
| V13 | Lumen-only hall, Low preset unreadable | M | M | P1 | Two lighting paths planned. RT off. |
| V14 | Legal temp music in a public VS trailer | L | H | P1 | Legal temp only. Replace before any public capture. |
| V15 | Arabic RTL unfinished, bilingual promise broken | M | M | P1 | Subtitle RTL is Green. Final font is Yellow. |
| V16 | One person holds every seat, Phase 0 never “ends” | M | H | P1 | This package *is* the end of Phase 0. Stop. |
| V17 | Building seven locations because the folders exist | M | H | **P0** | Content README. Only EastWing is scheduled. |
| V18 | Git LFS not enabled before first uasset | M | M | P1 | `.gitattributes` ready. Phase 1 task 1. |
| V19 | StateTree learning tax | M | M | P2 | BT fallback. Do not start in Phase 1. |
| V20 | Treating greybox VS as “good enough” | H | H | **P0** | Phase 10 gate language is explicit. |

P0 weekly list: **V02, V03, V04, V05, V07, V08, V10, V11, V12, V17, V20**.
