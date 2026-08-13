# PROJECT RISKS

**Project:** The Betrayed Will / وصية الغدر  
**Status:** Living register — review at every phase gate  
**Last updated:** 2026-08-13

Probability and impact are **L / M / H**.  
Priority is **P0** (can kill the project or the product promise), **P1** (can kill a phase or the date), **P2** (watch).

---

## 1. Risk matrix

| ID | Risk | Prob | Impact | Priority | Mitigation |
|---|---|---|---|---|---|
| R01 | Scope creep into open-world / RPG systems | H | H | **P0** | Binding Scope Control; five-question rule; weekly cut review; delete unofficial prototypes the same week |
| R02 | Vertical Slice is greybox and “promises quality later” | H | H | **P0** | Phase 10 gate is hard; VS must photograph as the shipping game; no Phase 11 if failed twice |
| R03 | Small team tries to build too many pillars equally in every hour | H | H | **P0** | Evan = investigate; Raynor = confront. Pillar mix is in the GDD and is not optional |
| R04 | Story contradictions / Leila-as-sister / secret-heir twists get “improved” in | M | H | **P0** | Assumptions file is binding; no blood relation for Leila; no secret parentage |
| R05 | Player rejects the identity swap after attaching to Evan | M | H | **P0** | Short playable Raynor in prologue; Evan’s death is playable and earned; Raynor chapters are climax, not a new campaign; same controls |
| R06 | Save system that works in PIE and dies in Shipping, or wipes slots | M | H | **P0** | Phase 8 acceptance on packaged builds; backup rotation; version refuse-don’t-wipe; QA kill-process tests |
| R07 | Softlocks on an investigation spine | H | H | **P0** | Fallback pointer on every investigate-to-progress beat; SkipTo for QA; softlock register in QA plan |
| R08 | Combat feels like a bad prototype and poisons reviews | M | H | **P0** | Tiny verb set; juice early (Phase 3); VS must include a fight that is fun; no sponges; no GAS tax |
| R09 | Stealth perception is unfair or noisy; players hate it | H | M | **P1** | Simple meter; failure converts to combat; no mission-fail-on-seen except telegraphed; ten AI tests |
| R10 | Unreal version / plugin / marketplace dependency trap | M | H | **P1** | Lock version in Phase 0; no plugin tourism; marketplace art must be licensed and painted through |
| R11 | Custom systems rebuilt where Unreal already works | M | M | **P1** | Architecture principles; review any new subsystem against “does UE have this?” |
| R12 | Cinematic ambition exceeds capture/face budget | H | M | **P1** | Four hero spends only; hybrid over film; demote CIN-05/07/09/16/17 first |
| R13 | MetaHuman / hero-face decision wrong | M | M | **P1** | Decide in Phase 0; pre-authorize abandon to traditional heads |
| R14 | Second half (Raynor) is under-built and feels like DLC | M | H | **P1** | Same city reused; Ch 7–8 get more calendar than their count; identity work starts as soon as Evan feels good, after systems 1–7 |
| R15 | Playtime slips under 5h or over 9h | M | M | **P1** | Chapter budgets; do not add districts to lengthen; do not pad walks |
| R16 | Localization / Arabic RTL broken, Arabic audience feels second-class | M | H | **P1** | UI RTL from Phase 6; human AR store text; VO plan communicated; test Windows AR locale |
| R17 | Audio is silence and temp copyright | M | M | **P1** | Legal temp only; VS audio acceptance; footsteps and detection are not optional |
| R18 | AI StateTree learning tax blocks VS | M | M | **P1** | Official fallback to Behavior Trees with the same state names |
| R19 | Performance: Lumen/Nanite-only lighting, unplayable Low | M | H | **P1** | Scalability from Phase 1; Low without Lumen still readable; budgets after VS data |
| R20 | Single-person bus factor (one human holds every seat) | H | H | **P1** | Documents good enough that a second person can enter; AI for boilerplate, not for taste; do not skip the 48-hour gold wait |
| R21 | Wishlist/trailer oversells a city we do not have | M | H | **P1** | Only shoot real spaces; Steam copy states 6–8 hours and “compact authored” |
| R22 | Steamworks early, contaminates VS | L | M | **P2** | Phase 16 implementation gate |
| R23 | Hazar / noble district / poison thread become a second plot | M | M | **P2** | Pre-authorized cuts 2–4; they are pressure, not a new game |
| R24 | VO quality embarrasses hero close-ups | M | M | **P2** | Subtitle-only is better than bad machine Orin; spend paid VO on four principals |
| R25 | Review-bomb on length or “not open world” | M | M | **P2** | Honest page; tags; trailer matches build |
| R26 | Historical-accuracy backlash | L | L | **P2** | Fictionalized Neo-Babylonian; we do not claim documentary truth; do not copy museum pieces |
| R27 | Name collision / repo “Islamic palace sim” confusion | L | M | **P2** | README rewritten; brief is authoritative; shipping title is The Betrayed Will |
| R28 | Legal: uncleared music, scans, or faces | L | H | **P1** | License pass before any public footage; no copyrighted assets |
| R29 | Certification / rating surprise | L | M | **P2** | Honest questionnaire; violence is visible and finite |
| R30 | Early Access used as a crutch for an unfinished story | M | H | **P1** | EA is contingency only; if used, store must name missing chapters; prefer slipping the date |
| R31 | Quest/dialogue tools become products | M | M | **P2** | Editor utility widgets only; no standalone apps |
| R32 | Identity swap corrupts saves or AI | M | H | **P1** | One-pawn preference; dedicated QA cases; swap before Ch 6 content art is finished so the bug is cheap |
| R33 | Team adds a bow / horse / romance meter “as optional” | M | H | **P0** | Locked out list; optional is still cost; delete |
| R34 | Depression of scope after VS: rewriting the story instead of building Ch 2 | M | H | **P1** | Story is locked; post-VS changes need a named cut; build chapters in order |
| R35 | Crash without a dump in Shipping | L | H | **P1** | Crash folder; never silent |

---

## 2. Heat map (summary)

|  | Impact L | Impact M | Impact H |
|---|---|---|---|
| **Prob H** | — | R09, R12 | **R01, R02, R03, R07, R20** |
| **Prob M** | — | R11, R13, R15, R17, R18, R23, R24, R25, R30, R31, R34 | **R04, R05, R06, R08, R10, R14, R16, R19, R21, R28, R32, R33** |
| **Prob L** | R26 | R22, R27, R29 | R35 |

P0 cluster to manage weekly: **R01, R02, R03, R04, R05, R06, R07, R08, R33**.

---

## 3. Triggers (what “this risk just happened” looks like)

| Risk | Trigger | Immediate action |
|---|---|---|
| R01 / R33 | A PR adds an inventory rarity field or a world-map fog | Revert; write a note or drop it |
| R02 | VS review uses the phrase “you have to imagine the lighting” | Fail the gate |
| R05 | Playtesters say “I wanted to quit when I became Raynor” | Shorten the gap, strengthen prologue Raynor, check Ch 7 opening |
| R06 | One report of a wiped slot | Stop feature work; treat as S |
| R07 | A tester cannot leave the palace without a hint | Add fallback the same day |
| R08 | VS fight is called “floaty” by two people | Stay in Phase 3/10; do not art Kurnah |
| R16 | Arabic choices overflow or LTR | Block UI sign-off |
| R21 | Trailer shows a skyline that is not in-game | Recut |

---

## 4. Residual risk we accept

- The game will not look like a 500-person studio’s Babylon.
- Some secondary VO will be imperfect at 1.0.
- Noble district and Hazar may be paper, not a place.
- Steam Deck may be unofficial.
- Historical consultants will find errors; we are making a tragedy, not a lecture.

Accepting these is how the P0 risks stay manageable.

---

## 5. Review cadence

- Weekly: P0 list, 15 minutes
- Every phase gate: this whole file
- After every external playtest: R05, R07, R08, R09, R15
- After first public trailer: R21, R25, R27, R28

Changes to this file should say what got worse, what got better, and what was cut because of it.
