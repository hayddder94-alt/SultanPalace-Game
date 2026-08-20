# Bringing your own character into the game

**Status:** pipeline written, nothing imported yet.
**Trigger:** a model appeared in `C:\Users\dell\Downloads\Tripo3d_Blender_Bridge`.

---

## The one question that decides everything

**Is the mesh rigged?**

| | What it is | What it can be in this game |
|---|---|---|
| **Rigged** — has a skeleton and skin weights | a character | Evan, Orin, a brother. Can walk, sit, die |
| **Not rigged** — geometry only | a statue | a prop, a body on a bed, architecture, a boat |

Generators of the Tripo3D kind usually output the second. That is not a fault;
it is what they do. Importing one and hoping is how an evening disappears.

Answer it before anything else:

```
.\tools\INSPECT_MODEL.cmd "C:\Users\dell\Downloads\Tripo3d_Blender_Bridge"
```

It reads the files directly — no Unreal, no Blender — and reports skeletons,
skin weights, animation tracks, textures and any licence file. For `.glb` it
parses the glTF JSON and counts `skins`. For `.fbx` it looks for `LimbNode` and
skin `Cluster` records, which appear as literal strings in both the binary and
ASCII encodings. For `.obj`, `.stl` and `.ply` the answer is *no* by definition:
those formats cannot store a skeleton at all.

---

## Route A — it is rigged

1. Import the FBX/glTF into `Content/TBW/Characters/<Name>/`.
2. Its skeleton will **not** be Manny's, so our clips will not play directly.
   Build an **IK Retargeter**: `IK_Manny` → `IK_<Name>`, then retarget
   `MM_Idle`, `MM_Walk_Fwd`, `MF_Unarmed_Jog_Fwd`.
3. Point the loader at it. `TBWPlayerCharacter.cpp` already tries
   `/Game/TBW/Characters/Evan/SKM_Evan` **first**, before any mannequin —
   name the asset that and it wins with no code change.

## Route B — it is not rigged

Two honest options.

**B1. Use it as what it is.** A generated bust, jar, boat or throne is
genuinely useful: the palace is 189 grey boxes. A single good hero prop on the
dais does more for the look than a whole rigged cast. Import as a Static Mesh,
drop it in the layout, done today.

**B2. Rig it.** Mixamo auto-rigs a humanoid upload for free and returns an FBX
with a skeleton; Blender's Rigify does the same with more control and more
work. Then it becomes Route A. Budget an evening, not ten minutes.

## Route C — it is a `.blend`

Unreal cannot import `.blend`. Export FBX or glTF from Blender first
(*File → Export → FBX*, Armature + Mesh, **Apply Transform** on).

---

## Before any of that: the licence

This is a hard line and it has cost projects their release.

Whatever generated this model has terms. Some AI generators grant full
commercial rights on paid plans and **withhold them on the free tier**. Some
require attribution. Some forbid resale of the asset as an asset — which is
fine for a game — and some do not.

**Find the terms for the exact account and plan the model was made on, and read
the commercial-use clause.** If it cannot ship on Steam, it cannot go in, no
matter how good it looks. Same rule that applies to every asset here: an asset
acquired under terms that do not permit commercial release is a liability
disguised as progress.

`INSPECT_MODEL.cmd` prints any licence or readme file it finds in the folder.
If there is none, the terms live on the generator's website, not on your disk.

---

## What I need from you

Run the inspection and paste its report. It ends with one of two verdicts, and
which one it is decides whether the next hour is spent on a character or on a
prop.
