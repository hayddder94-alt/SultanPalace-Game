# Free and open-source 3D character generation — what actually fits this project

**Researched 2026-08-20.** Judged against three hard constraints, not against
demo reels.

| Constraint | Value | Why it eliminates most options |
|---|---|---|
| GPU | Quadro P3000, **6 GB VRAM** | the good image-to-3D models want 16–24 GB |
| Need | **17 characters that walk** | an unrigged mesh is a statue; rigging 17 by hand is the real cost |
| Ship | Steam, commercial | a licence that forbids that makes everything else irrelevant |

---

## The distinction that decides everything

There are two completely different families of tool, and confusing them is why
"generate a 3D character" sounds easier than it is.

| | AI image-to-3D | Parametric human generators |
|---|---|---|
| Input | a picture | sliders |
| Output | **unrigged** mesh | **rigged** human, skeleton and weights included |
| Good at | props, statues, one-off hero objects | bodies that need to move |
| Hardware | a large NVIDIA GPU | a CPU |

**We need bodies that move.** That points away from the famous repos.

---

## Family A — AI image-to-3D (open weights)

| Repo | Licence | VRAM | Rigged? | Verdict here |
|---|---|---|---|---|
| [microsoft/TRELLIS.2](https://github.com/microsoft/TRELLIS.2) | **MIT** — cleanest of all | 16–24 GB | no | Best quality open model, PBR output [1](https://trellis2.app/blog/best-image-to-3d-models-huggingface). **Cannot run on 6 GB.** |
| [stabilityai/TripoSR](https://github.com/VAST-AI-Research/TripoSR) | **MIT** | **6–8 GB** | no | The only one that fits the GPU [6](https://triposr.org/blog/hunyuan3d-vs-trellis). Vertex colours only, no PBR, ~256³ geometry — fine for a jar, not for a face |
| [Tencent-Hunyuan/Hunyuan3D-2.1](https://github.com/Tencent-Hunyuan/Hunyuan3D-2.1) | Tencent Community — **excludes EU, UK, South Korea** | 10–29 GB | no | Best open textures, but a territory exclusion is a shipping problem, not a footnote [3](https://app.cinevva.com/guides/ai-3d-model-generators) |
| [VAST-AI-Research/TripoSG](https://github.com/VAST-AI-Research/TripoSG) | MIT | high | no | Same family, same rigging gap |
| SF3D / SPAR3D | Stability Community — free under $1M revenue | ~6 GB | no | Fine licence for us today, still unrigged |

**Summary:** every one of these hands you a statue. To use one for a character
you then rig it yourself — which is the expensive half of the job, seventeen
times.

They *are* the right tool for the palace's props: a lion-head clasp, a water
bowl, a will tablet, a covered boat. That work is real and TripoSR can do it on
this GPU.

---

## Family B — parametric humans, rigged from the start

### MakeHuman — the clearest licence in this whole search

* Code: AGPL. **Exported models: CC0 by explicit exception.**
* Their own FAQ spells out the exact case we need: *"Use a character model in a
  closed source game without need for attribution … Sell a character model"*
  [4](https://static.makehumancommunity.org/makehuman/faq/are_makehuman_files_free.html).
* Runs on the CPU. The 6 GB card is irrelevant.
* Outputs a rigged humanoid, exportable as FBX, with a choice of skeletons —
  including game-oriented ones.

Nothing else we looked at states the commercial position that plainly. After a
week of licence traps, that is worth more than a nicer demo.

### CharMorph — the modern successor, with one trap

* [Blender add-on](https://www.blendernation.com/2025/03/19/charmorph-v0-4-0-character-creator-released/),
  GPLv3 code, spiritual successor to MB-Lab, Rigify support, real-time clothing
  fitting [2](https://www.cgchannel.com/2025/03/check-out-new-open-source-3d-character-generator-charmorph/).
* **The trap:** its base meshes carry *different* licences. The MB-Lab-derived
  base is **AGPL, and that propagates into the models you export** — which
  would make our closed-source game's characters AGPL. The project says so
  itself.
* **The rule if we use it:** the **Vitruvian base is CC0**, and there are CC-BY
  bases. Use those and only those. Never the AGPL base.

### Mixamo — not a repo, but it closes the gap

Free, Adobe account, upload a humanoid mesh and get it auto-rigged and
animated. It is the bridge that turns any Family A statue into a Family B
character. Not open source, but free and commercially usable.

---

## The recommendation

**For the cast: MakeHuman.**

Seventeen bodies, rigged, CC0, generated on a laptop CPU while the GPU does
nothing. Period costume is then modelling work — simple tunics and robes are
some of the easiest garments there are, and we already have the concept sheets
in `docs/art/concept/` to model against.

**For props: TripoSR**, locally, MIT, 6 GB. Feed it a photo or one of our
concept images and get a clasp, a bowl, a tablet. Props do not need skeletons,
which is exactly why this family suits them.

**Not for now: TRELLIS.2.** It is the best open model and its MIT licence is
the cleanest, but 16–24 GB of VRAM is not a preference we can argue with. Note
it for the day the hardware changes — it is where I would go first.

**Avoid: Hunyuan3D** while it excludes the EU and UK, and any free-tier hosted
generator whose output is CC BY 4.0 non-commercial (see
`docs/CUSTOM_CHARACTER_IMPORT.md`).

---

## The executable version

The ordered, step-by-step plan with download links, export settings and a
checkpoint after every stage is in **`docs/PLAN_CHARACTERS.md`**.

Note on tooling: standalone MakeHuman still works, but the same community now
ships **MPFB2** as a Blender add-on installable in one click from
<https://extensions.blender.org/add-ons/mpfb/>. It generates the humanoid,
does the parametric modelling and **rigs it automatically** without leaving
Blender, which removes an entire export/import round trip.

## What this changes about the plan

Nothing about the code. `TBWPlayerCharacter.cpp` already tries
`/Game/TBW/Characters/Evan/SKM_Evan` **before** any mannequin, and
`ATBWStoryCharacter` uses the same ladder. A MakeHuman export named that wins
with no code change at all.

The retarget step is the same either way: build an IK Retargeter from the
mannequin rig onto the new skeleton so the clips we already resolve —
`MM_Idle`, `MM_Walk_Fwd`, `MF_Unarmed_Jog_Fwd` — play on our own people.

---

## Honest caveat about all of it

A generated body is a starting point, not a character. The bible's priority
order is Raynor → Leila → Evan → Nofan, and the reason is that *faces* carry
this story. A parametric generator gets you seventeen plausible bodies in an
afternoon; it does not get you Nofan's warmth or Darius's stillness. Those are
sculpting and animation, and no repository on GitHub will hand them over.
