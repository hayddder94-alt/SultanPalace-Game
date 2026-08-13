# VERTICAL SLICE SCRIPT

**Project:** The Betrayed Will / وصية الغدر  
**Status:** Locked for Phase 0  
**Playable identity:** **Evan only**  
**Location:** East Ceremonial Wing of Orin’s Palace + canal gate  
**Designed duration:** **34 minutes**  
**First-time player estimate:** **40–44 minutes**  
**Hard cap:** 45 minutes. If a rehearsal exceeds it, cut from the list in §8.  
**Maps:** `L_VS_Palace` (single persistent). No second district. No Kurnah.

Raynor is **seen**, not played. The full-game playable prologue is a Phase 11 item. The slice must not include Raynor gameplay.

---

## 0. Player-facing promise

You witness a dying father’s last will. You wake as the overlooked son. By the time the slice ends you know Raynor did not leave, and someone inside the house helped him vanish.

---

## 1. Timing card

| ID | Scene | Location | Designed | First-time |
|---|---|---|---|---|
| VS-00 | Title / content note | Menu | 0:20 | 0:25 |
| VS-01 | Orin’s last words | Upper chamber | 2:30 | 2:30 |
| VS-02 | The will reading | Audience hall | 3:20 | 3:20 |
| VS-03 | Eyes in the hall / terrace | Hall + terrace | 1:00 | 1:00 |
| VS-04 | Morning. You are Evan | Evan’s chamber | 0:25 | 0:40 |
| VS-05 | Learn the house with your feet | Family corridor + hall | 2:30 | 3:30 |
| VS-06 | The unused room | Raynor’s chamber | 3:30 | 4:40 |
| VS-07 | Soren will not look at you | Corridor niche | 2:20 | 3:00 |
| VS-08 | Yasmin and the performed joy | Kitchen yard edge | 1:40 | 2:10 |
| VS-09 | The canal clasp | Canal gate | 3:00 | 4:20 |
| VS-10 | The night roster | Barracks annex | 4:00 | 5:30 |
| VS-11 | A man who was not supposed to see you | Storage court | 2:00 | 3:00 |
| VS-12 | Wrong wax | Steward study | 1:40 | 2:10 |
| VS-13 | The board connects | Pause / in-world | 1:10 | 1:40 |
| VS-14 | Nofan’s kindness / slice end | Audience hall | 1:40 | 1:40 |
| | **Total** | | **33:45** | **41:25** |

First-time column already includes looking at the wrong door and failing stealth once. It stays under 45.

Optional Malik two-liner (VS-08b) is **not** on the clock if skipped. Cap +1:00.

---

## 2. Scene scripts

### VS-00 — Title

| Field | Content |
|---|---|
| Location | Main menu / fade |
| Duration | 0:20 |
| Objective | Start a new game, Standard |
| Gameplay | Menu only |
| Dialogue | None. Content note: *Family cruelty. Violence.* |
| NPCs | — |
| Cinematic | Title card: **The Betrayed Will** / **وصية الغدر** over the empty hall at oil-night |
| Audio | `mus_palace_night` swell, then cut |
| Assets | Title texture, EN/AR font, content-note widget |
| Anims | None |
| Tech | GameInstance, settings language, difficulty write |

---

### VS-01 — Orin’s last words

| Field | Content |
|---|---|
| Location | Upper chamber (East Wing, one floor up). Player is **not** in control. |
| Duration | 2:30 |
| Objective | Watch. Learn the father’s fear. |
| Gameplay | None. Look-around disabled or very slight. |
| NPCs | Orin (hero face), Raynor (hero face, seated), one nurse-servant (card, silent) |

**Dialogue (locked, short):**

ORIN  
Water first. Always water. A house that forgets the ditches can keep its gold for a season.

RAYNOR  
The southern cuts are holding.

ORIN  
I am not talking about mud.  
*(breath)*  
Nofan has spent his life in rooms I sent him to. I used him. Then I looked away.  
If I name him, he will eat the house to prove he deserved it.

RAYNOR  
Then name me and be done.

ORIN  
I will. And you will hate me for the peace it steals.  
Protect Evan. He hears what you walk past.  
Protect Soren. He still thinks blood is a kindness.  
*(takes Raynor’s hand)*  
Do not be merciful with the wrong son.

*Orin’s eyes close. The nurse covers the mouth. Raynor does not stand yet.*

| Cinematic | CIN-VS-01. Close on hands, then the scar, then the river audible through the lattice. |
| Audio | Almost no score. Oil. River. Orin’s breath. |
| Assets | Chamber hero dress, Orin LOD0, Raynor formal, bed, water bowl, seal on the table (not taken yet) |
| Anims | Orin sit/lie, Raynor seated idle, hand clasp, death breath |
| Tech | Sequencer, skip locked until seen once, subtitle EN/AR |

---

### VS-02 — The will reading

| Field | Content |
|---|---|
| Location | Audience hall |
| Duration | 3:20 |
| Objective | Understand the public wound. |
| Gameplay | None. Player is seated among the sons as a camera, not as Raynor. |
| NPCs | Six sons, Leila, Temple Witness, 4 staff |

**Dialogue (abridged legal text — do not read a scroll):**

WITNESS  
In the presence of the house and the temple:  
The riverside palace, the eastern charter, and the southern water pass to **Raynor**.  
The other sons keep a room and a stipend, if they name him.  
The betrothal of **Leila**, daughter of Bel-Iddin, is confirmed.  
**Nofan**, first-born, is named advisor to the heir.

*Silence. A servant begins to clap and stops.*

NOFAN  
*(warm)*  
Father always did prefer a clean sentence.

*He aligns the cup in front of him. Does not drink.*

DARIUS  
*(to a guard, not to the room)*  
Doors.

MALIK  
Already writing.

SOREN  
*(to Evan, almost nothing)*  
He said your name last night. I heard it through the door.

LEILA  
*(does not smile)*

RAYNOR  
I accept the seal.

| Cinematic | CIN-VS-02. Faces, not architecture porn. Must land: Nofan’s warmth, Darius’s stillness, Malik’s ink, Soren’s look at Evan, Leila’s unsmiling, Raynor taking the seal. |
| Audio | Hall tone. Wax. One drum from the courtyard that is a fraction late. |
| Assets | Hall hero lighting, six brother positions, will tablet, seal prop, Leila temple dress |
| Anims | Seated ids, seal accept, Nofan cup-align (signature) |
| Tech | Sequencer. Crowd is 6+1+1+4. No extra city nobles. |

---

### VS-03 — Eyes in the hall

| Field | Content |
|---|---|
| Location | Hall → terrace door (camera) |
| Duration | 1:00 |
| Objective | Feel that the naming was a death sentence someone already understood. |
| Gameplay | None |

**Dialogue:**

LEILA  
*(to Raynor, terrace, half-heard)*  
If they loved him, they would have been kinder in their eyes.

*Raynor walks toward the canal gate at dusk. Fade. We do not follow.*

| Cinematic | CIN-VS-03. End on the dark canal, not on Raynor’s face. |
| Audio | Night insects. One oar. |
| Assets | Terrace, canal glimpse (skybox + water plane, not a district) |
| Anims | Walk away, Leila still |
| Tech | Crossfade into VS-04. **Identity is now Evan.** |

---

### VS-04 — Morning. You are Evan.

| Field | Content |
|---|---|
| Location | Evan’s chamber |
| Duration | 0:25 designed / 0:40 first-time |
| Objective | Move. Look. Understand you are not the man from the night. |
| Gameplay | Walk, look. First input. Prompt: move / look. |
| Dialogue | EVAN (examine, optional): *They clapped for him. Then they went to bed as if the house had not changed shape.* |
| NPCs | None |
| Cinematic | None. Gameplay camera. Daylight. |
| Audio | `mus_palace_morning` — slightly wrong joy |
| Assets | Evan mesh, day clothes, wrist bead, satchel, unmade bed |
| Anims | Locomotion start |
| Tech | `DA_Identity_Evan` applied. Raynor identity is **not** selectable. |

---

### VS-05 — Learn the house with your feet

| Field | Content |
|---|---|
| Location | Family corridor → audience hall (morning state) |
| Duration | 2:30 / 3:30 |
| Objective | Reach Raynor’s door. Read the room. |
| Gameplay | Walk, run, look. Performed celebration: cloth on the empty heir chair, two servants setting bowls. Soft wall at west arch (the rest of the palace). Prompt on the chair. |
| Dialogue | SERVANT (if spoken): *The heir left before dawn. Northern estates. Malik has the letter.* |
| NPCs | 2 servants (shared civilian faces) |
| Cinematic | None |
| Audio | Distant bronze, courtyard |
| Assets | Hall morning dress, empty chair hero prop |
| Anims | Servant work loops |
| Tech | Location volume `EastWing`. Soft-wall volume `RestOfPalace`. |

**Examine — empty chair:**  
*Still warm cloth. No one has sat. They laid the celebration anyway.*

---

### VS-06 — The unused room

| Field | Content |
|---|---|
| Location | Raynor’s chamber |
| Duration | 3:30 / 4:40 |
| Objective | Examine two clues. Unlock Focus and the pin-board. |
| Gameplay | Explore, examine, Focus hold. No enemies. |
| NPCs | None inside. Yasmin may pass the door and not enter. |

**Clue A — unused bed** (`Clue_UnusedBed`)  
Examine: *The sheet is tight. He did not lie down. Raynor sleeps like a soldier. He would have left a ruin if he had meant to sleep an hour.*

**Clue B — scar-oil** (`Clue_ScarOil`)  
Examine: *He oils the scar when the wind is dry. The stopper is loose. The oil is full. He did not pack this.*

Evan, after both: *He did not leave. He was taken, or he was lied about.*  
Flag: `EvanSuspicious = 1`

| Cinematic | None. Examine camera only. |
| Audio | Quiet room. Oil stopper. |
| Assets | Bed, oil bottle hero prop, cloak peg empty, window on canal light |
| Anims | Examine reach |
| Tech | `UTBWClue` ×2, Focus component, pin-board unlock, autosave |

---

### VS-07 — Soren will not look at you

| Field | Content |
|---|---|
| Location | Corridor niche / prayer recess |
| Duration | 2:20 / 3:00 |
| Objective | Talk. One real choice. |
| Gameplay | Dialogue. Choice does **not** gate the spine. Sets `SorenRespected` true/false for later game; in the slice it only changes his last line. |
| NPCs | Soren |

**Dialogue:**

SOREN  
They said he asked for a boat. Raynor does not ask for boats. He takes the path along the wall.

EVAN  
You heard father say my name.

SOREN  
I hear a lot of things I am not invited to keep.

**Choice (one important, marked with a seal):**

1. *Then keep this: I do not believe the letter.* → `SorenRespected = 1`  
   SOREN: *Neither do I. Do not say that in the yard.*
2. *Go back to your books. This is not yours.* → `SorenRespected = 0`  
   SOREN: *You sound like Darius when you do that.*

SOREN  
Malik has been in the study since before the sun. He only works that early when he is inventing a day.

| Cinematic | Conversation camera, shot / reverse. |
| Audio | Corridor hush |
| Assets | Soren mesh (secondary hero), niche |
| Anims | Talk stands, Soren hands in sleeves |
| Tech | Dialogue runner, one flag, subtitle |

---

### VS-08 — Yasmin and the performed joy

| Field | Content |
|---|---|
| Location | Kitchen yard edge (open to the storage court) |
| Duration | 1:40 / 2:10 |
| Objective | Talk. Get pointed at the canal and the barracks without a quest arrow lecture. |
| Gameplay | Talk. Interact. |
| NPCs | Yasmin |

**Dialogue:**

YASMIN  
I turned his bed down. No one slept in it. Malik told me to strip it anyway. I did not.

EVAN  
Who took the night watch off the canal?

YASMIN  
Ask the roster. Darius keeps it in the annex like it is a blade.  
And if you go to the water, go before Nofan comes down. He has been kind all morning. I do not like him kind.

| Cinematic | None |
| Audio | Kitchen, flies, a pot |
| Assets | Yasmin civilian-hero, kitchen dressing visible from the edge. We do **not** build a full kitchen. |
| Anims | Talk, look-away |
| Tech | Objective update: *Search the canal gate. Read the night roster.* |

**VS-08b optional (Malik, steward study door):**  
If Evan knocks:

MALIK  
The heir is inspecting what he has been given. That is what responsible men do. You will make the staff theatrical.  
*(already writing)*  
Come back when you have decided to be glad.

No new flag. Cut this conversation first if the slice runs long.

---

### VS-09 — The canal clasp

| Field | Content |
|---|---|
| Location | Canal gate / service dock (still East Wing exterior) |
| Duration | 3:00 / 4:20 |
| Objective | Recover Raynor’s cloak clasp. |
| Gameplay | Explore the dock. Talk to the boatman. Examine mud. Focus helps. Softlock fallback: after one full dock loop the boatman points. |
| NPCs | Boatman (civilian) |

**Dialogue:**

BOATMAN  
Covered boat. Before the birds. Two men I did not know, and one I did. House cloak. He did not speak.

EVAN  
Raynor always speaks to the dock.

BOATMAN  
Then it was a very quiet Raynor, or it was a cloak.

**Clue C — bronze clasp** (`Clue_CanalClasp`)  
Examine: *The lion-hook from his shoulder. Torn, not unpinned. He does not drop this. He would hear it hit the stone.*  
Flag: `ClaspFound = 1`

| Cinematic | None. A short camera settle when the clasp is picked. |
| Audio | Water against stone. Oar far away. |
| Assets | Dock modular, water plane, clasp hero pickup, boat (static) |
| Anims | Pickup, boatman sit |
| Tech | Clue, autosave, objective: *The annex roster* |

---

### VS-10 — The night roster

| Field | Content |
|---|---|
| Location | Barracks annex (restricted) |
| Duration | 4:00 / 5:30 |
| Objective | Enter unseen or accept a fight. Read the roster. |
| Gameplay | Crouch, detection, one distraction stone, optional takedown. Two guards on a spline. Restricted volume. Hide-body volume behind crates. |
| NPCs | Guard A (patrol), Guard B (stand at inner door) |

**On first crouch:** prompt. Detection eye. No mission-fail-on-seen — detection converts toward VS-11 early.

**Clue D — altered roster** (`Clue_AlteredRoster`)  
Examine: *Canal posts struck through. Darius’s mark over a name I know. The night Raynor “left,” the water had no house eyes.*  
Flag: `RosterAltered = 1`

| Cinematic | None |
| Audio | Detection tick, stone, cloth, distant kitchen |
| Assets | Annex modular, spline, roster tablet, crates, one extinguishable lamp |
| Anims | Guard patrol, takedown (simple), crouch loco |
| Tech | Stealth component, perception, alert subsystem lite, stone toss |

---

### VS-11 — A man who was not supposed to see you

| Field | Content |
|---|---|
| Location | Storage court (between annex and kitchen yard) |
| Duration | 2:00 / 3:00 |
| Objective | Survive a short melee. This encounter is **not missable**. |
| Gameplay | Light, heavy, block, dodge. Parry window exists; not required. 1 sword guard if stealth succeeded (he comes from the court door). 2 if the annex is already Alert. Standard is winnable and losable. Death → checkpoint at court threshold. Non-lethal finishers available; lethal allowed. |
| Dialogue | GUARD: *The annex is not for younger sons.* |
| NPCs | 1–2 house guards |
| Cinematic | None. Optional 2 s finisher. |
| Audio | `mus_combat` short. Blade, block, body. |
| Assets | Dagger (Evan), combat dummy-quality guard, court space with clear nav |
| Anims | Light string 2, heavy 1, block, dodge, hit react, death |
| Tech | Combat component subset, health, stamina, checkpoint |

This is the entire VS combat. No second wave. No captain.

---

### VS-12 — Wrong wax

| Field | Content |
|---|---|
| Location | Steward study (Malik gone, or standing aside) |
| Duration | 1:40 / 2:10 |
| Objective | Examine the official letter. |
| Gameplay | Door now opens (flag `RosterAltered` or `ClaspFound` — either is enough so the study is not a softlock). Examine the letter on the desk. |

**Clue E — forged letter** (`Clue_FalseLetter`)  
Examine: *Raynor’s words in Malik’s patience. The seal is our house. The pressure is wrong. He presses deep, like a man who means it. This is a careful man pretending to be him.*  
Flag: `LetterIsFalse = 1`

MALIK *(if present)*  
You are holding a courtesy. Put it back.

| Cinematic | Examine camera |
| Audio | Paper, wax |
| Assets | Study, letter hero prop, seal impression |
| Anims | Examine |
| Tech | Clue. If Malik is present he does not start combat. |

Five clues is the VS maximum. If the slice is fat, drop Clue A or B, not C/D/E.

---

### VS-13 — The board connects

| Field | Content |
|---|---|
| Location | Pause board, or a short in-world camera on Evan |
| Duration | 1:10 / 1:40 |
| Objective | See the first family connection. |
| Gameplay | Open pause if the player has not. Auto-connect fires once all of: clasp, roster, false letter. |

**Conclusion line (Evan’s voice):**  
*He did not ride north. Someone in this house moved the watch, wrote his hand, and put him on a boat that did not want to be seen. This is not a rival. This is us.*

Flag: `FamilyConnected = 1`  
This is the **first major evidence** the brief requires. We do **not** name Nofan as the architect yet. Three Names is Chapter 3.

| Cinematic | UI-cinema, 8 s thread draw |
| Audio | Wax-stamp sting |
| Assets | Pin-board art, three scraps + one thread |
| Tech | Investigation subsystem auto-connect |

---

### VS-14 — Nofan’s kindness / end

| Field | Content |
|---|---|
| Location | Audience hall, morning emptying |
| Duration | 1:40 |
| Objective | Hear the threat dressed as care. Slice complete. |
| Gameplay | Walk into a trigger. Then cinematic. Manual save still available after. |

**Dialogue:**

NOFAN  
You have been making the servants nervous, Evan.  
That is not a crime. It is a weather.  
Raynor will send for us when the north has been counted. Until then, eat. Sit. Let the house be a house.

EVAN  
The canal had no watch.

NOFAN  
*(aligns a bowl, smiles with his eyes only)*  
Then I will speak to Darius about sloppiness. Thank you.  
Go inside. The sun on that water makes men invent stories.

*He puts a hand on Evan’s shoulder. It is almost kind.*

*Camera leaves them for the canal. The covered boat is gone. The clasp’s absence in the mud is a dark comma.*

**On-screen:**  
MISSION COMPLETE  
*The house has started to lie.*

| Cinematic | CIN-VS-14. Hold-to-skip after first play. |
| Audio | Morning bed under a dry sting. No choir. |
| Assets | Nofan hero face, hall, shoulder touch |
| Anims | Nofan stand, cup/bowl align, shoulder |
| Tech | Quest complete, autosave, return-to-title allowed, Continue must restore this point |

---

## 3. Objectives the player sees

1. *Wake. Find Raynor.*  
2. *Search his chamber.*  
3. *Ask the people who still talk.*  
4. *Search the canal gate.*  
5. *Read the night roster in the annex.*  
6. *The letter on Malik’s desk.*  
7. *(auto) Face what the house has written.*

No side jobs. No collectible counter.

---

## 4. Required systems (and only these)

| System | VS need | Phase that builds it |
|---|---|---|
| Player move / look / crouch / sprint | Yes | 2 |
| Identity (Evan data) | Yes | 2 |
| Interact + examine | Yes | 5 |
| Focus + pin-board (5 clues) | Yes | 5–7 |
| Dialogue + 1 choice | Yes | 6 |
| Quest toasts / 4 segments | Yes | 7 |
| Stealth perception + takedown + stone | Yes | 4–5 |
| Melee verbs vs 1–2 sword guards | Yes | 3 |
| Save / load / continue | Yes | 8 |
| Sequencer cinematic + skip | Yes | 1 / 10 |
| EN/AR subtitles | Yes | 6 |
| Raynor playable | **No** | 11 |
| Steamworks | **No** | 16 |

---

## 5. Cast actually on screen

| Person | Quality bar | Notes |
|---|---|---|
| Evan | Hero, playable | Always |
| Orin | Hero face | VS-01 only |
| Raynor | Hero face | VS-01–03 only, not playable |
| Leila | Hero face | VS-02–03, few shots |
| Nofan | Hero face | VS-02 and VS-14 |
| Soren | Secondary hero | VS-02, VS-07 |
| Darius | Readable silhouette | VS-02 sit, no fight |
| Malik | Secondary face | VS-02, optional VS-12 |
| Yasmin | Recurring civilian | VS-08 |
| Temple Witness | Ceremonial | VS-02 VO |
| Guards ×2 | Shared combat | VS-10, VS-11 |
| Servants ×2, boatman | Shared civilian | |

---

## 6. Audio must-haves

Night palace bed, morning palace bed, combat loop (short), detection tick, footsteps stone/rug, water, wax sting, will-reading VO or strong scratch, Evan examine lines, Nofan close. Missing VO never blocks.

---

## 7. Technical dependencies

- `L_VS_Palace` baked lighting, two scenarios (oil-night cine, white morning play)
- Navmesh on court + annex + hall
- Checkpoints: after VS-04, after VS-06, dock, annex threshold, court threshold, after VS-13
- No-save volumes during VS-01–03 and VS-14
- Soft-walls: only East Wing is walkable

---

## 8. Cut list if a rehearsal exceeds 45 minutes

In this order:

1. VS-08b Malik door  
2. Clue A (keep scar-oil)  
3. Terrace line in VS-03 (cut to fade from the hall)  
4. Takedown (keep crouch + detect)  
5. Shorten VS-02 to naming + three faces (Nofan, Leila, Raynor)  
6. Never cut: clasp, roster, false letter, the fight, Nofan’s kindness, save/load

---

## 9. What this slice is not

Not Chapters 1–3. Not a market. Not a temple visit. Not Kurnah. Not a Raynor tutorial. Not Three Names. Not a romance beat. Not Steam.

If a rehearsal is *short* (under 28 minutes), do **not** add a district. Lengthen examines and the will faces.

---

## 10. Acceptance of the script (Phase 0)

- [x] Timed on paper to 30–45 minutes  
- [x] Opening will + Orin  
- [x] Control transfers to Evan  
- [x] Walk, run, interact, talk, investigate, stealth, guard, combat, evidence, cinematic end  
- [x] First family-connection evidence  
- [x] No forbidden VS features  
- [ ] Table-read with a timer in Phase 10 week 1 (live)
