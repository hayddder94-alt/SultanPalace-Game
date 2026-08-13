# CHARACTER PRESENTATION — FOUR PRIMARIES

**Project:** The Betrayed Will  
**Status:** Locked for Vertical Slice presentation  
**Pipeline:** Hybrid MetaHuman face/body + custom period clothing (`CHARACTER_PIPELINE.md`)

This file is what art and cine must hit. It is not a wardrobe system and not a relationship system.

---

## Shared rules

- Same skeleton. Same pawn for the two playables.
- Attractiveness is carriage and attention. The camera does not leer.
- Period: linen, wool, bronze, glazed-bead, kohl as character (Leila ceremonial only).
- No plate. No hoodies. No “fantasy Arab” turbans from a marketplace pack.
- Hero eyes before hero pores.
- Two people in a shot must be readable in silhouette.

---

## 1. Evan — playable in the slice

| | Requirement |
|---|---|
| Age / build | 27, lean, shorter than Raynor |
| Face | Warm, tired curiosity. Not naive. Ink-shadow at the nails if the camera gets that close. |
| Signature | River-stone bead on the left wrist. Open collar. Satchel. |
| Motion | Head turns before the feet. Lighter footfall. |
| VS costumes | One: day house clothes. |
| Cine | Must carry examine close-ups. Must be distinct from Raynor in one silent shot. |
| VO | Warm, questioning. Scratch allowed. |
| Failure | Making him a second handsome hero. Making him comic. |

Identity data target (`DA_Identity_Evan`):

- Walk 420, crouch 160, sprint 620  
- Noise 0.8, visibility 0.85  
- Light 0.8, heavy 0.85, poise 0.8, reach 130  
- `bCanChargeHeavy = false`

---

## 2. Raynor — seen, not played, in the slice

| | Requirement |
|---|---|
| Age / build | 34, tall, broad, still |
| Face | Scar, left brow to cheekbone, pale, clean, readable at medium shot. |
| Signature | Scar. Shoulder cloak in the prologue. Seal in his hand in VS-02. |
| Motion | Shoulders first. Even seated he looks like he could stand into a fight. |
| VS costumes | One: formal night / dawn. Prisoner and returned cloaks are Phase 11. |
| Cine | VS-01 hands, VS-02 seal, VS-03 back walking to the canal. We must miss him when Evan wakes. |
| VO | Low, precise. Few lines. |
| Failure | Hiding the scar. Playing him in the slice. Identical body to Evan. |

Identity data (`DA_Identity_Raynor`) is **authored** to prove the architecture and is **not** applied in the slice:

- Walk 380, crouch 140, sprint 560  
- Noise 1.15, visibility 1.2  
- Light 1.0, heavy 1.25, poise 1.3, reach 165  
- `bCanChargeHeavy = true`

---

## 3. Leila — not decoration

| | Requirement |
|---|---|
| Age / build | 29, tall, unhurried |
| Face | Intelligence first. Ceremonial gold temple-line in VS-02. |
| Signature | Stillness. She does not smile at the naming. |
| Motion | She arrives; she is not presented. |
| VS costumes | One: temple ceremonial. City clay-dark is later. |
| Cine | VS-02 reaction. VS-03 half-heard line. That is enough for the slice to make her necessary. |
| VO | Dry, never breathless, never “seductive default.” |
| Failure | Orbiting camera. A romance prompt. Explaining her. |

Full motivation and trust flags: `LEILA_SPEC.md`. The slice **does not** implement trust. It only introduces her unreadability.

---

## 4. Nofan — the kindness that is a threat

| | Requirement |
|---|---|
| Age / build | 37, as tall as Raynor, narrower |
| Face | Host, not villain. Warm when most dangerous. |
| Signature | Clean hands. Aligns a cup / bowl before he speaks. |
| Motion | Unhurried. He is always dressed for witnesses. |
| VS costumes | One: ceremonial morning / night that still looks like court. |
| Cine | VS-02 “clean sentence.” VS-14 hand on Evan’s shoulder. These two shots are the antagonist. |
| VO | Warm. No cackle. |
| Failure | A sneer. A dark hood. A scar to “match” Raynor. |

---

## 5. Orin (required supporting hero)

Dying, 71, still fills a room with a voice. Talks about water and sons. VS-01 only. If we can afford three finished faces, he is the third after Evan and Nofan. He must not be a mannequin on the deathbed.

---

## 6. VS production order for faces

1. Evan (player)  
2. Nofan (close)  
3. Orin (close)  
4. Raynor (scar + seal)  
5. Leila (stillness)  
6. Soren  
7. Everyone else as shared

---

## 7. What we will not do in the slice

- Makeup / wardrobe UI  
- Body morph in gameplay  
- A second Evan costume  
- Playable Raynor  
- Crowd of unique nobles  
- Beauty-pass that delays the walk/sneak/hit feel
