# The Betrayed Will — Assumptions, Gaps, and Resolutions

**Document status:** Binding for planning  
**Last updated:** 2026-08-13  
**Owner:** Lead Game Director / Narrative Lead

This document records every material gap in the original brief, every contradiction found during analysis, and the smallest reasonable assumption used to close it. Production documents treat these assumptions as locked unless a later design review explicitly overturns them.

No assumption in this file expands playtime, adds multiplayer, adds an open world, or adds RPG systems.

---

## 1. Source conflict: repository vs. brief

| Source | Claim |
|---|---|
| Repository `README.md` | “أول لعبة عربية واقعية لمحاكاة قصر إسلامي تاريخي” — a realistic Arabic historical Islamic-palace simulation |
| Production brief | Cinematic third-person story-driven action adventure set in **Ancient Babylon / Mesopotamia**, titled **The Betrayed Will / وصية الغدر** |

**Resolution:** The production brief is authoritative. This is not a palace simulator and not an Islamic-period game. The setting is a fictionalized Neo-Babylonian city. The existing README will be rewritten to match the brief. The repository name `SultanPalace-Game` is historical only and is not used in shipping branding.

---

## 2. Narrative gaps and locked assumptions

### 2.1 The six sons — only three were named

**Gap:** The brief names Raynor, Evan, and Nofan. Three brothers are unnamed.

**Assumption (smallest):** The remaining brothers are **Darius**, **Malik**, and **Soren**. They exist to make the conspiracy structurally believable. They are not additional protagonists.

| Brother | Role in the house | Conspiracy stance |
|---|---|---|
| Nofan | Eldest. Court politician. Passed over by the will. | Architect |
| Raynor | Chosen heir. Warrior-statesman. | Victim, then protagonist |
| Darius | Commander of the house guard | Muscle of the abduction |
| Evan | Overlooked middle son. Investigator. | First playable character |
| Malik | Steward of ledgers, servants, stores | Paper trail and false evidence |
| Soren | Youngest. Temple-educated. | Not a founder of the plot; contested loyalty |

Birth order is locked as: Nofan, Raynor, Darius, Evan, Malik, Soren.

### 2.2 Why Orin passed over the eldest

**Gap:** The brief does not say why Raynor, not Nofan, inherits.

**Assumption:** Orin judged Nofan hungry for the house’s power and Raynor capable of protecting its people and irrigation rights. The will is a merit choice that breaks eldest-son expectation. That wound is Nofan’s motive. It is not a secret bloodline twist.

### 2.3 Leila’s relationship to the family

**Gap:** Leila is a primary character with no defined blood or legal tie. A sister-romance would create an incest problem the brief does not ask for.

**Assumption:** Leila is **not** a blood relative. She is the daughter of High Priest Bel-Iddin of the Temple of Ishtar, betrothed to Raynor by Orin’s last political act, to bind the house to the temple. She is intelligent, charismatic, and initially unreadable. Her loyalty is the story’s secondary mystery. She is never a cartoon femme fatale and never a helpless prize.

### 2.4 Cause of Orin’s death

**Gap:** “After the death of the powerful patriarch” does not say murder.

**Assumption:** Orin dies of a wasting illness. Mid-game, Evan can find **optional** evidence that the illness was hastened by poison. This is a secondary revelation, not a second main plot. If it threatens scope, the poison thread is the first narrative cut.

### 2.5 Why Raynor is imprisoned instead of killed

**Gap:** A disappeared heir is a louder problem than a dead one, unless killing him is dangerous.

**Assumption:** Orin left a sealed second tablet with the temple: if Raynor dies within one year of succession, the royal trade charter reverts to the crown and the irrigation concession is frozen. The conspirators need Raynor alive and hidden until they can destroy or replace that tablet. Official story: Raynor left to inspect the northern estates.

### 2.6 Who betrays Evan, and how he “dies”

**Gap:** “Evan is betrayed and believed to be dead” is unspecified.

**Assumption:** Malik lures Evan to the old river storehouse with a forged note in Soren’s hand. Darius’s men ambush him. He is stabbed and thrown into the canal. A reed cutter pulls him out downriver. The house announces he drowned while drunk. He is believed dead for the rest of Chapter 4–5.

### 2.7 How Evan dies for real

**Gap:** He frees Raynor, then dies. No cause given.

**Assumption:** During the fortress escape, Evan holds the inner gate against Darius’s incoming guard. He is mortally wounded by a spear. He lives long enough to put the house seal into Raynor’s hand and say the line that titles the second half: the will was not the tablet. The will is what you do with the truth. This is a playable last stand, not only a cutscene.

### 2.8 What the house actually inherits

**Gap:** “Successor” is undefined. A palace alone is a weak stake.

**Assumption:** House Orin holds three things: the riverside palace, a royal trade charter for the eastern caravan gate, and the irrigation concession for two agricultural districts. Inheritance is political power over grain, water, and court access. The King is a background pressure, not a playable or fully produced character.

### 2.9 Historical period

**Gap:** “Ancient Babylon / Mesopotamia” spans millennia.

**Assumption:** Fictionalized Neo-Babylonian period. Architecture, clothing, weapons, and religion use Neo-Babylonian references (glazed brick, Ishtar iconography, sickle-swords, spears, maces, reed boats). Named historical kings are not required. We do not claim documentary accuracy. Tone is “believable ancient city,” not a textbook.

### 2.10 Character names vs. setting

**Gap:** Raynor, Evan, Nofan, Orin, Leila are not Akkadian names.

**Assumption:** Keep the brief’s names as player-facing names. Each major character also has a short Akkadian-style epithet used in ceremony and documents (see Character Bible). We do not rename the cast. Localization can later offer an optional “historical names” subtitle mode; it is not a production requirement.

### 2.11 Language of the game

**Gap:** Arabic title exists; spoken language is unspecified.

**Assumption:** Shipping UI and subtitles support **English and Arabic**. Voice production may start as English placeholder / generated voices, with Arabic and English professional VO as a post-vertical-slice pass. In-world writing on tablets can use stylized cuneiform decals plus readable translation in the examine UI.

### 2.12 The King and the city government

**Gap:** A public will crisis in Babylon should attract the palace.

**Assumption:** Nofan has a court patron, **Lord Hazar**, a royal provisioner who wants House Orin’s charter absorbed. Hazar keeps the King “unconcerned.” Hazar is a late-game reveal and a conversation / confrontation character, not a second final boss. If scope slips, Hazar remains off-screen and exists only in documents and dialogue.

### 2.13 Player character switch

**Gap:** The player spends most of the game as Evan, then loses him, while marketing priority is Raynor.

**Assumption:** This is intentional. Prologue gives a short playable Raynor scene so the player meets him as a person. Chapters 1–6 are Evan (~4.5 hours). Chapters 7–8 are Raynor (~2–2.5 hours) and are the climax, not a new campaign. Systems, controls, and camera stay identical. Raynor plays heavier and more capable; Evan plays more investigative. No third playable character.

### 2.14 Leila’s moral alignment

**Gap:** Brief says “manipulative or mysterious depending on final story design.”

**Assumption:** Mysterious first, morally grey, not a simple villain. She withholds the second tablet from both sides until she knows who will protect the temple’s independence. She is capable of a betrayal and capable of a rescue. The player can reach one of two locked readings of her, based on three specific interactions, not a relationship meter.

### 2.15 Nofan’s humanity

**Gap:** Must not be a simplistic villain.

**Assumption:** Nofan loved Orin and spent twenty years doing the political work Raynor refused. He believes Raynor will get the house destroyed by honesty. He wants Raynor broken and discredited, not butchered. He does not order Evan’s death; Darius exceeds the plan. Nofan’s tragedy is that he cannot stop the machine he started.

### 2.16 Endings

**Gap:** No ending specified.

**Assumption:** One core ending path with three final states, driven by two binary decisions (Nofan’s life; Leila’s trust). No branching campaign. Total extra production for endings is limited to the final 8–12 minutes and a handful of epilogue cards.

---

## 3. Design contradictions and how they are closed

| Contradiction | Close |
|---|---|
| Brief wants cinematic AAA presentation and also independent/AA scope | Presentation budget goes into lighting, camera, hero characters, and a few authored spaces. Not into world size or NPC count. |
| 9 story blocks (prologue + 8 chapters) inside 6–8 hours | Average chapter is 40–50 minutes. Prologue is 15–20. Chapters 7–8 are longer. Optional content is capped at 30–40 minutes. |
| Four gameplay pillars on a small team | Evan chapters are Exploration + Investigation, with light stealth and rare combat. Raynor chapters are Combat + Stealth, with lighter investigation. Not all pillars are equal in every hour. |
| “High-quality modern cinematic games” vs. “do not replicate RDR2” | Reference the *discipline* of those games (composition, acting, sound) not their production volume. |
| Character priority is Raynor, but Evan is the first long playable | Marketing, key art, and the final act center Raynor. Evan is the audience’s way into the crime. |
| Investigation games often become UI-heavy detective sims | Evidence is a small pin-board of 20–30 clues, not a case-management RPG. |
| Combat listed with light/heavy/block/dodge/parry/finisher | That list is the **entire** combat verb set. No stances, no magic, no weapon classes beyond 3. |
| Save “manual + autosave + multiple slots + world state” | Supported, but world state is a compact flag set, not a simulation snapshot. |

---

## 4. Explicit non-goals (locked)

The following will not be designed, prototyped, or scheduled:

- Open world, streaming wilderness, mount/vehicle travel between cities
- Multiplayer, co-op, PvP, live service, seasons, gacha, battle pass
- Crafting, cooking, hunting, fishing, romance meters, dating
- Skill trees larger than 8 unlocks
- Loot rarity, randomized gear, durability
- Hundreds of NPCs, crowd simulation, full day/night city simulation
- Horse combat, chariot combat, naval combat
- New Game+ as a launch requirement
- Photo mode as a launch requirement
- Mod tools as a launch requirement
- A second city
- A playable Leila or Nofan
- A prequel campaign about Orin

---

## 5. Open questions that may stay open until Vertical Slice

These do not block planning. They must be answered before full production (Phase 11).

1. ~~Exact Unreal Engine minor version at kickoff~~ **LOCKED: UE 5.8 only** (`docs/ENGINE_VERSION_LOCK.md`). Hotfix frozen on the day the production machine is imaged. Do not use 5.6 or 5.7.
2. Whether Arabic VO ships on day one or as a first patch.
3. Whether Lord Hazar appears on screen.
4. Whether the poison-Orin thread survives the first content cut.
5. Final Steam achievement count (target 20–30, designed in Phase 16 planning, implemented late).

---

## 6. How to overturn an assumption

1. Write the change in a one-page design note.
2. State what it costs in weeks and what it improves (story, gameplay, commercial value).
3. Name what will be cut to pay for it.
4. Get director sign-off.
5. Update this file, the Story Bible, and the Scope Control document in the same change.

If a proposed change does not name a cut, it is rejected.
