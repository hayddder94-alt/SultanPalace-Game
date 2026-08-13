# STEAM RELEASE PLAN

**Project:** The Betrayed Will / وصية الغدر  
**Platform:** Windows PC  
**Distribution:** Steam  
**Status:** Planning lock  
**Law:** Do not implement Steamworks until a Shipping Windows build is in sight (Phase 15–16). Plan it now.

---

## 1. Product promise (the page must not lie)

**The Betrayed Will** is a 6–8 hour cinematic third-person story adventure set in a compact, authored Babylon. Family conspiracy, investigation, focused melee, simple stealth.

It is **not** an open-world ancient sandbox, not a live service, not a palace simulator, not a 40-hour RPG.

Store copy in English and Arabic must say the playtime and the genre honestly. Wishlist conversion dies if the trailer looks like Assassin’s Creed and the build is a corridor tragedy — so the trailer should look like a corridor tragedy that is beautiful.

## 2. When Steam work happens

| Work | When |
|---|---|
| Concept page, legal entity, name check | Phase 0–1 |
| App ID reserved, coming soon (no trailer yet) | Optional, after Phase 0 |
| Trailer + screenshots from VS | Phase 10 accepted |
| Wishlist push | Phase 10–11 |
| Steamworks in the build | Phase 16 (hooks designed now) |
| Review build / recap / depot | Phase 15–16 |
| Launch | After Phase 16 acceptance |
| Post-launch patches | 30–60 day watch |

Early Access is a **contingency** if Phase 11 slips past honesty. It is not the plan. If EA is ever used, the store must say “story incomplete” and name the missing chapters. Prefer to slip the date.

## 3. Technical integration (Phase 16)

### 3.1 SDK

- `OnlineSubsystemSteam` + Steamworks SDK version current at implementation
- App ID in a single config, not scattered
- `UTBWAchievementBridge` listens to world flags
- Steam Input recommended, with our IMC actions published
- Overlay must work (cursor, pause)

### 3.2 Achievements (target 20–30, draft)

Keep them story and play, not grind.

**Story (hidden where spoilery):**

1. The Named Son — finish prologue
2. Wrong Wax — prove the letter false
3. Three Names — pin the conspiracy
4. The Canal — survive Ch 4
5. Spoken in the Past — Soren’s confession
6. Unbroken — free Raynor
7. The Seal — finish MQ-06c
8. The Returned — enter the city as Raynor
9. The Second Writing — tablet in hand
10. The Audience Hall — finish the game (any ending)

**Play:**

11. Quiet Annex — barracks without an alert
12. Clean Hands — a fight won with a parry finisher
13. Reed and Iron — hide 5 bodies in Kurnah (not a collectable plague; cap it)
14. Brother’s Keeper — treat Soren with respect
15. Paper Knife — expose Malik publicly
16. The Captain’s Debt — spare Radd
17. Temple Token — enter Kurnah by the water gate (story, still)
18. Night Walker — Ch 7 palace without killing civilians (there are almost none)

**Endings (hidden):**

19. The House Stands
20. The House is a Name
21. The House is Ash

**Optional extras if we still have room:**

22. Reader — collect all clues (20–30, fine)
23. Witness — find OQ-04 edict
24. Another Man’s Cell — OQ-06
25. Standard or Hard complete

No “deal 10,000 damage.” No multiplayer. No timed daily.

### 3.3 Steam Cloud

- Sync the three slots + settings (or settings local-only; prefer cloud slots)
- Conflict: newest timestamp wins, warn if possible
- Tested on two machines in Phase 16

### 3.4 Other Steam features

| Feature | 1.0 |
|---|---|
| Achievements | Yes |
| Cloud | Yes |
| Steam Input | Yes |
| Overlay | Yes |
| Trading cards / badges | Optional, late, not a dependency |
| Workshop / mods | No |
| Rich presence | Nice if cheap (“In the palace”, “In Kurnah”) |
| Leaderboards | No |
| Multiplayer | No |
| DLC | Not planned |
| Demo | Optional: the VS after Phase 10, if it does not spoil Ch 4+ |
| Deck / Proton | Best-effort verification, not a launch promise unless it is free |

### 3.5 Languages

Steam language: English + Arabic store text and in-game UI/subtitles.  
VO language: as decided in Phase 0. If English VO only, say so.

### 3.6 Controller

Full gamepad support is a pillar, not a bonus checkbox. Steam Input + in-game remap.

## 4. Store page

### 4.1 Capsule / title

Title: **The Betrayed Will**  
Arabic: **وصية الغدر** visible on the Arabic storefront.

Key art: Raynor’s scar in oil-light, or the six sons and one empty space in the hall. Not a skyline of a huge city we do not have.

### 4.2 Short description (draft)

English:  
*A cinematic third-person tragedy in ancient Babylon. A will names the wrong son. You uncover the conspiracy as the brother who will not survive it — then finish the story as the heir they buried alive.*

Arabic store text is written by a human, not a raw machine pass, in Phase 16.

### 4.3 Features list (honest)

- 6–8 hour authored campaign
- Two playable brothers, one story
- Investigation, stealth, and focused melee
- A compact, reusable Babylon — palace, temple, market, marsh fortress
- Cinematic presentation, English and Arabic subtitles
- Keyboard/mouse and controller
- Built-in graphics settings, multiple save slots, Steam Cloud

Do not list: open world, RPG builds, hundreds of weapons, multiplayer, photo mode (unless it actually ships).

### 4.4 Trailer plan

1. **VS teaser (Phase 10):** 60–90 s, will reading + Evan in the palace + one stealth beat + one hit. No Ch 6 spoilers.
2. **Story trailer (Phase 12):** 90–120 s, can hint at disappearance and betrayal, not the death.
3. **Launch trailer:** ending-safe, faces, one combat, one line of Orin.

No trailer uses footage that is prettier than the build.

### 4.5 Screenshots

Minimum 5: hall, terrace night, market street, temple court, Kurnah cistern, a conversation face, a combat read. Mixed EN/AR UI in at least one.

### 4.6 Tags (draft)

Story Rich, Cinematic, Third Person, Action-Adventure, Stealth, Investigation, Singleplayer, Atmospheric, Dark, Mature, Historical, Drama.

Not: Open World, Co-op, Survival, Crafting, Looter.

### 4.7 Rating

Expect ESRB M / PEGI 18. Violence, dark themes. Prepare the questionnaire honestly. No sexual content requirement.

## 5. Pricing and commercial

- Premium single-purchase.
- Price band: comparable to other 6–8 hour authored indies / AA shorts at the time of launch (set in Phase 16 with current comps). Do not price as a 60-hour open world.
- Launch discount: modest if any.
- Regional pricing: yes, including MENA, with Arabic as a first-class storefront.
- Game Pass / Epic: out of scope for planning. Steam first.

## 6. Wishlist and community

- Wishlist CTA after VS, when footage is honest.
- A simple Steam forum + one external channel (whatever the team can actually staff).
- No live-ops calendar.
- Spoiler policy pinned: Ch 4+ is spoiler territory.

## 7. Branching and depots

- `app` Windows 64
- Playtest branch for QA
- Shipping default
- Optional demo app if the VS becomes a demo

Version string in-game matches the build uploaded.

## 8. Launch checklist

- [ ] Shipping build, no cheats, no dev maps
- [ ] Cloud verified
- [ ] Achievements unlock once, no mass-unlock on boot
- [ ] Overlay + Steam Input
- [ ] Store EN + AR
- [ ] Capsules in all required sizes
- [ ] Trailer accepted
- [ ] Recap / build uploaded
- [ ] Supported languages filled
- [ ] Min/rec specs from Phase 13 data
- [ ] Save path documented
- [ ] Crash folder documented for support
- [ ] Credits and third-party licenses
- [ ] Age rating submitted
- [ ] Launch date with a slip buffer
- [ ] Day-0 branch ready for a hotfix

## 9. Post-launch (30–60 days)

- Crash and progress-blocker patches only
- Arabic VO if it was deferred
- No new pillar, no new map, no season pass announcement

A “making of” or photo mode can wait.

## 10. Risks specific to Steam

| Risk | Mitigation |
|---|---|
| Trailer oversells world size | Only shoot spaces that exist |
| Review-bomb on playtime | State 6–8 hours on the page |
| Arabic community feels second-class | Human AR store text, RTL that works, VO plan communicated |
| Steamworks integrated too early | Phase 16 gate |
| Cloud conflicts wipe a slot | Backup rotation + test |
| Deck unplayable and users assume it | Do not check Deck Verified unless tested |

## 11. What we will not do on Steam

- Battle pass
- Marketplace
- Loot boxes
- Always-online
- Forced launcher besides Steam
- A second store as a launch requirement
