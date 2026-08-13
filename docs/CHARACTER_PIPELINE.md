# CHARACTER PIPELINE COMPARISON

**Project:** The Betrayed Will  
**Status:** Comparison complete. Recommendation locked for Vertical Slice.  
**Date:** 2026-08-13  
**Four primaries:** Evan, Raynor, Leila, Nofan  
**Also needs a face:** Orin (prologue close-ups)

No live MetaHuman import was possible on this host (Unreal Editor is not installed). The comparison is production research against UE 5.8 capabilities (MetaHuman Creator in-editor since 5.6). A one-day import test on the production machine is the first character task in Phase 2, not a reason to delay this lock.

---

## 1. What “good” means for this game

We need four recurring humans who can hold a close-up in Sequencer, share one animation contract with the player pawn, wear **Neo-Babylonian** clothes (not a hoodie), and not destroy a 1080p High frame in a hall with six seated brothers.

We do not need: character creator at runtime, hundreds of faces, crowds, fashion, body morphs in gameplay.

---

## 2. Option A — MetaHuman-based heroes

**What it is (UE 5.7):** In-editor MetaHuman Creator (since 5.6), DNA facial rig, Control Rig body, MetaHuman Animator for performance, automatic LOD, groom system, Outfit assets that refit clothing to body.

**License (post-5.6):** Covered by the Unreal Engine EULA. Commercial shipment inside a UE game is allowed. Under $1M revenue the tools are free; above that, standard UE seat terms. Do not train foundation AI models on the DNA. Fine for this project.

### Scores

| Axis | Assessment |
|---|---|
| Visual quality | Best free path to cinematic skin, eyes, micro-expression. Risk: “MetaHuman face” if we ship presets. |
| Performance | LOD0 + strand groom is expensive. Four LOD0 heroes in one hall will hurt. Must use card/groom LOD for seated brothers not speaking. VRAM is the real cost. |
| Facial animation | **Winning axis.** Sequencer + Animator + ARKit-style curves. This is why the option exists. |
| Body animation | Shared UE skeleton / Control Rig. Retarget from a common library. Fine. |
| Clothing | **Losing axis.** Preset wardrobe is modern. Period costume must be custom meshes fitted as Outfit assets. This is real work either way. |
| Customization | High within human range. Raynor’s scar is a texture + local sculpt / Mesh-to-MetaHuman, not a slider. |
| Production time | Faces: days, not months. Clothing + unique identity: weeks. Net win on faces, wash on clothes. |
| Memory | High at LOD0. Acceptable if only the speaking face is LOD0. |
| Licensing | Clear for a Steam UE game. |
| Sequencer | Native. Best in class for our cine list. |
| Fit for 4 recurring leads | This is the use case MetaHuman is good at. It is bad at 40 unique leads. We have 4. |

### VS implication

Enable MetaHuman / groom plugins **when the first hero is imported**, not in Phase 0 “just because.”

---

## 3. Option B — Traditional custom characters

**What it is:** Sculpt in Blender/ZBrush, wrap a custom or Epic skeleton, build a face rig (or use a marketplace face), hand-paint skins, cloth as standard skeletal meshes, facial via morph targets / pose assets.

### Scores

| Axis | Assessment |
|---|---|
| Visual quality | Ceiling is higher for *stylized period* silhouettes. Floor is “indie mannequin” if the team is small. Cinematic close-up quality on four faces is a specialist job. |
| Performance | We control every LOD and we can be cheap. Better for guards and crowds (we have almost no crowds). |
| Facial animation | We must build or buy a rig. Visemes + 20 poses will look like a 2014 RPG in a hero close-up. Full FACS is months. |
| Body animation | Same as A if we use the UE5 mannequin / MH-compatible skeleton. If we invent a skeleton we pay forever. |
| Clothing | **Winning axis.** Draped linen, temple gold-line, cloak weight — authored exactly. |
| Customization | Unlimited. Scar, age, ink-stained fingers are just mesh and texture. |
| Production time | One hero face at cinematic quality: many weeks. Four: a phase by itself. |
| Memory | As low as we author. |
| Licensing | We own it, if we do not scrape photogrammetry we do not have rights to. |
| Sequencer | Works. Acting quality depends on the rig we could afford. |
| Fit for 4 leads | Expensive. Excellent for *secondary* and *guards*. |

---

## 4. Direct comparison

| Axis | Winner |
|---|---|
| Visual quality (faces, cine) | A |
| Visual quality (period silhouette) | B, unless A wears custom clothes |
| Performance (4 heroes + 8 extras) | B, unless A is strictly LOD-disciplined |
| Facial animation / Sequencer | **A, by a lot** |
| Body animation | Tie if both use the same skeleton |
| Clothing | **B, by a lot** — unless we do custom outfits on A |
| Production time to first close-up | **A** |
| Production time to a unique Babylonian wardrobe | Tie (must be custom either way) |
| Memory | B |
| Licensing | Tie for our case |
| Risk of “everyone looks like a demo” | A |
| Risk of “faces cannot carry the will reading” | **B, fatal** |

The game’s commercial and critical bet is **faces in a hall**, not a cloth sim.

---

## 5. Recommendation (locked)

**Hybrid.**

1. **Four primaries + Orin:** MetaHuman DNA faces and bodies as the rig/skin base.  
2. **All clothes:** custom period outfits (linen, wool, bronze, temple white-gold). No MH hoodies, no leather jackets. Outfit assets fitted to each body.  
3. **Raynor’s scar:** albedo + normal + a small face overlay. Must read at medium shot.  
4. **Grooms:** cards or simplified grooms for VS. Strand hair only on Leila and only in close-ups if the frame time allows.  
5. **Guards and civilians:** traditional modular (2 guard bodies, 2 civilian bodies, unique heads as cheap as possible). Not MetaHuman.  
6. **Darius / Malik / Soren:** MetaHuman *or* traditional secondary. Prefer MH presets with custom clothes if time is cheap after the four; otherwise traditional heads on the guard/civilian body.  
7. **Skeleton contract:** all humans retarget to the MetaHuman / UE5 compatible skeleton so Evan and Raynor stay one pawn.  
8. **Do not** enable MetaHuman Crowd (5.8, out of version lock, out of scope).

This is not “all-in MetaHuman” and not “all custom.” It spends the tool where the story spends the camera.

### Overturn rule

If the Phase 2 import test shows we cannot get a non-preset face (scar, age, period hair) in two days, fall back to traditional hero heads for Evan and Raynor first, keep MH for Leila/Nofan cine, and write the change in this file.

---

## 6. Presentation requirements for the four primaries

See also `CHARACTER_PRESENTATION.md`. Minimum bar for the slice:

| Character | On-screen in VS | Must read |
|---|---|---|
| Evan | Always | Lighter body, wrist bead, ink, curious head |
| Raynor | Cine only | Scar, weight, seal, not a second Evan |
| Leila | Cine, few shots | Stillness, temple line, not decorative |
| Nofan | Cine open + close | Clean hands, warmth, cup-align |

If only two hero faces can be finished, they are **Evan + Nofan** (the player, and the kindness that is a threat). Raynor and Leila can hold on strong silhouettes for a week. Orin cannot be a mannequin in VS-01 — he is the third face if we can afford three.

---

## 7. Plugins (when, not now)

| Plugin | Phase 0 | When |
|---|---|---|
| Enhanced Input | On | Now |
| MetaHuman, HairStrands | Off | First hero import (Phase 2/10) |
| Control Rig | Editor default | As needed |
| StateTree | Off | Phase 4 |
| Steam | Off | Phase 16 |
