# Plan: from grey mannequin to our own cast

**Written 2026-08-20.** Ordered so that the first hour produces something
visible in the game, and every step after it is verifiable by a tool we
already have.

Nothing here is a download-and-hope. Each stage ends with a check that either
passes or names what is wrong.

---

## Stage 0 — what is already true

Do not redo these.

| | Status |
|---|---|
| Palace, 242 actors, 21 lights, coloured | built and verified |
| 17 staged characters, tinted per role | in the level |
| Mannequin walking with real clips | `MM_Idle`, `MM_Walk_Fwd`, `MF_Unarmed_Jog_Fwd` |
| 17 dialogue scenes, Arabic proven on screen | 83 lines |
| Concept sheets for Evan, Raynor, Leila | `docs/art/concept/` |

**The loader already prefers our own art.** `TBWPlayerCharacter.cpp` tries
`/Game/TBW/Characters/Evan/SKM_Evan` *before* any mannequin, and
`ATBWStoryCharacter` uses the same ladder. Name the asset correctly and it wins
with **no code change at all**.

---

## Stage 1 — install the two tools · ~40 min, mostly downloading

### Blender 4.2 LTS or newer

<https://www.blender.org/download/>

Free, GPLv3. Art you make with it is yours — the Blender Foundation states this
outright, and it is why a GPL tool is not a licence problem the way a GPL
*asset* is.

### MPFB2 — the human generator, inside Blender

<https://extensions.blender.org/add-ons/mpfb/>

In Blender: **Edit → Preferences → Get Extensions → search "MPFB" → Install.**
One click, no zip files, no manual paths.

* Source: <https://github.com/makehumancommunity/mpfb2>
* Docs / quick start: <https://static.makehumancommunity.org/mpfb.html>
* Requires Blender **4.2+**

This is the modern successor to standalone MakeHuman, by the same community. It
one-click-creates a humanoid mesh, does parametric body modelling, and — the
part that matters — **rigs automatically**, with a choice of skeletons and
Rigify support.

### Optional: standalone MakeHuman

<http://www.makehumancommunity.org/content/downloads.html>

Only needed if you want the older asset downloader. MPFB2 covers the workflow
on its own.

### Licence position, stated plainly

MakeHuman's own FAQ names our exact case:

> *"Use a character model in a closed source game without need for
> attribution … Sell a character model"*
> — <https://static.makehumancommunity.org/makehuman/faq/are_makehuman_files_free.html>

Core assets are **CC0**. The add-on code is GPLv3, which governs the code, not
the mesh you export — the same relationship Blender itself has with your art.

**Before the first export, read that FAQ page yourself.** I am reporting it,
not warranting it, and this is the one class of mistake that cannot be undone
after release.

---

## Stage 2 — build Evan · ~1 hour

Evan first, not Raynor. He is the character the player *is* for the whole
slice, so he is the one whose absence is felt every second.

Model him against `docs/art/concept/Evan_concept_front.png` and the locks in
`docs/CHARACTER_BIBLE.md` §2.2:

| Lock | Setting in MPFB |
|---|---|
| Age 27 | age slider |
| Lean, quick, **shorter than Raynor** | height and mass sliders — keep him visibly smaller |
| Mesopotamian features | face sliders |
| Open collar, no armour | clothing: simple tunic |

Rig: choose a **game engine rig**, not Rigify, for the first pass. Rigify is
for animation authoring; a plain deform skeleton retargets more predictably.

Export: **File → Export → FBX**

| Setting | Value | Why |
|---|---|---|
| Path Mode | Copy, with the embed-textures button on | otherwise textures do not travel |
| Apply Transform | **on** | Blender is Z-up, Unreal is Z-up, but scales differ |
| Armature → Add Leaf Bones | **off** | leaf bones confuse the retargeter |
| Bake Animation | off | we are exporting a body, not a performance |

Save it somewhere outside the repo — a 30 MB FBX does not belong in git.

### Checkpoint

```
.\tools\inspect_model.cmd "C:\path\to\the\export\folder"
```

It must say **RIGGED**, with a bone count and skin weights. If it says
`NO SKELETON`, the export dropped the armature — redo the export with the
Armature section enabled.

---

## Stage 3 — into Unreal · ~30 min

1. Content Browser → `Content/TBW/Characters/Evan/` → **Import** the FBX.
2. Import options: **Skeletal Mesh** on, **Import Mesh** on, **Create Physics
   Asset** on, **Import Animations** off.
3. Rename the skeletal mesh asset to exactly **`SKM_Evan`**.

That name is not cosmetic. It is the first entry in the loader's candidate
list, ahead of every mannequin path.

### Checkpoint

```
.\tools\play.cmd
```

Expect in the self test:

```
INFO  body: skeletal mesh, single-node blockout locomotion
INFO  clips: idle=...  walk=...  run=...
```

Evan will appear — and will probably stand in his bind pose, because our clips
belong to the mannequin skeleton, not his. **That is expected and is Stage 4.**

---

## Stage 4 — make our animations play on him · ~45 min

This is the only genuinely fiddly step.

1. Right-click `SKM_Evan` → **Create → IK Rig**. Add a Retarget Chain for
   spine, both arms, both legs, and set the Retarget Root to the pelvis.
2. Do the same for the mannequin: an IK Rig on `SKM_Manny_Simple`.
3. Create an **IK Retargeter**: source = mannequin rig, target = Evan's rig.
4. Select `MM_Idle`, `MM_Walk_Fwd`, `MF_Unarmed_Jog_Fwd`,
   `MM_Unarmed_Crouch_Walk_Fwd` → **Export Selected Animations** into
   `Content/TBW/Characters/Evan/Animations/`.

Epic's own documentation for this:
<https://dev.epicgames.com/documentation/en-us/unreal-engine/ik-rig-animation-retargeting-in-unreal-engine>

Our scanner searches `/Game/TBW/Characters` **first**, so the retargeted clips
win over the mannequin's automatically — again, no code change.

### Checkpoint

`clips:` in the self test should now show names ending `@Game`, resolved from
Evan's own skeleton, and he should walk.

---

## Stage 5 — the rest of the cast · ~2 hours

Now it is repetition, and it gets much faster.

Order from the bible's production priority, adjusted for who is on screen
longest in the slice:

1. **Nofan** — speaks VS-14, the last scene
2. **Raynor** — VS-01 and VS-02, the reason for everything
3. **Leila** — stands apart in the hall, unsmiling
4. **Soren**, **Malik**, **Darius**
5. **Orin** — needs a lying pose, so he wants the death clip working
6. Servants, guards, boatman — **one shared body**, three tints

Same skeleton for all of them. That is the whole trick: build one body, vary
the sliders, and every character inherits the retargeted animation set with no
extra work.

Costume is the honest cost here. A tunic and a robe are among the simplest
garments to model, and MPFB fits clothing to the body automatically. The
per-role colours are already chosen and already in the layout — see the tint
table in `tools/layout/east_wing.py`.

---

## Stage 6 — props, a separate track · anytime

Different tool, different family, no skeleton needed.

**TripoSR** — MIT licence, runs in 6–8 GB, which is what your card has:

* <https://github.com/VAST-AI-Research/TripoSR>
* Try it in the browser first, no install:
  <https://huggingface.co/spaces/stabilityai/TripoSR>

Worth generating, in this order of impact:

| Prop | Where it appears |
|---|---|
| The will tablet | VS-02, in the player's hands |
| The lion-head cloak clasp | VS-09, the clue in the mud |
| A water bowl and an oil jar | Orin's bedside, Raynor's chamber |
| A covered boat | the canal gate |

One good hero prop on the dais does more for how the palace reads than three
more grey boxes.

---

## What I do while you do this

Not blocked on any of it:

* **VS-07's choice UI** — the two branches are written and unreachable
* **The blue light** — the sky is drowning the warm lamps; needs your eye once
* **`FSlateFontInfo` → `FCompositeFont`** — 3 deprecation warnings that the
  engine says will stop compiling next release
* **Performance** — still blocked on one thing, below

---

## The one thing I keep asking for

```
.\tools\collect_pie_log.cmd
```

The **GPU / RHI adapter** section, nothing else.

Stage 5 adds seventeen skinned meshes to a scene currently running at 16 FPS.
If the engine is rendering on the Intel HD 530 instead of the Quadro — a real
possibility on an Optimus laptop, and an earlier log did print
`GPU: Intel HD 530` — then every budget in this plan is built on a false
number, and I would rather find that out before you model seventeen people
than after.
