# ENGINE VERSION LOCK

**Project:** The Betrayed Will / وصية الغدر  
**Status:** **LOCKED**  
**Locked on:** 2026-08-13  
**Owner:** Technical Director

---

## Decision

| Field | Value |
|---|---|
| Engine | Unreal Engine 5 |
| Minor version | **5.7** |
| Hotfix policy | Latest **5.7.x** hotfix at the moment the production machine is imaged. Then freeze that exact hotfix. |
| Association string | `"5.7"` in `TheBetrayedWill.uproject` |
| Platforms | Windows 64-bit only for the Vertical Slice |
| Change during VS? | **No**, unless a critical blocking engine defect has no workaround |

This is not the newest Unreal. That is deliberate.

---

## Versions considered (as of 2026-08-13)

| Version | Released | Age at lock | Verdict |
|---|---|---|---|
| 5.4 | 2024-04-23 | ~28 months | Too old. Misses in-editor MetaHuman Creator, 5.6 60 Hz renderer work, 5.7 StateTree / Substrate maturity. |
| 5.5 | 2024-11-12 | ~21 months | Historically the “safest” 5.x. Rejected because MetaHuman Creator is still a sidecar and we would start the VS already planning a forced upgrade. |
| 5.6 | 2025-06-03 | ~14 months | Strong. In-editor MetaHuman, Lumen HWRT gains, 60 Hz device profiles. Runner-up. |
| **5.7** | **2025-11-12** | **~9 months** | **Selected.** |
| 5.8 | 2026-06-17 / 23 | ~7 weeks | Newest. Last planned major UE5. Too young to lock a Vertical Slice against. |

---

## Evaluation against the required axes

| Axis | 5.7 finding | Weight |
|---|---|---|
| **Stability** | Nine months of hotfixes (5.7.1 / 5.7.2 and later). Newer than the “5.5 is the stable one” folklore, old enough that first-wave defects are documented. | High |
| **Documentation** | Full 5.7 docs for Nanite, Lumen, Sequencer, StateTree, MetaHuman plugin, packaging. 5.8 docs are still settling. | High |
| **Plugin compatibility** | Marketplace and first-party plugins have had a full season on 5.7. 5.8 plugin lag is expected. We enable almost none of them anyway. | Medium |
| **Nanite** | Production, including 5.7 Nanite foliage (we will barely use foliage in the palace). | High |
| **Lumen** | Production. Software Lumen is the VS default. Hardware RT stays **off** until a High preset needs it. | High |
| **MetaHuman** | Creator + Animator integrated since 5.6; 5.7 extends the plugin to Linux/macOS and keeps the DNA / Control Rig path we need for four hero faces. | High |
| **Packaging reliability** | Windows IoStore + pak is boring on 5.7. That is what we want. | High |
| **AI tooling** | StateTree is first-class. We still do not enable the plugin until Phase 4. | Medium |
| **Sequencer** | Mature. Take Recorder / subsequence workflow is enough for the will reading. | High |
| **Control Rig** | Production. Used for MH face and simple IK, not as a gameplay animation engine. | Medium |

---

## Why not 5.8

The brief says: do not automatically select the newest version.

5.8 is the last major UE5 and is seven weeks old at this lock. It adds Mesh Terrain, MetaHuman Crowds, further Lumen work. None of that is a Vertical Slice blocker. Crowds are a scope violation. Terrain is irrelevant to an authored palace wing. Migrating onto a fresh major during the slice is how small teams lose a month.

Revisit **after** the Vertical Slice gate, not during it. A post-VS move to 5.8 is allowed only with a written risk note and a duplicate project test.

## Why not 5.6

5.6 is the runner-up and would have been a correct lock six months ago. By August 2026, 5.7 gives us the same MetaHuman-in-editor path plus nine months of animation / StateTree / Substrate production hardening, without taking the 5.8 new-major bet.

## Why not 5.5

Stability reputation is real. Cost is a character-pipeline tax (web MetaHuman, weaker body tools) and a near-certain mid-production upgrade. We refuse to start the VS on a version we already intend to leave.

---

## Locked companion settings

| Setting | Lock |
|---|---|
| RHI | DirectX 12 |
| Default GI | Lumen (software) |
| Default reflections | Lumen |
| Hardware ray tracing | Off by default |
| Nanite | On for static environment |
| Target hardware class | Desktop |
| C++ include order | `EngineIncludeOrderVersion.Unreal5_7` |
| Build settings | `BuildSettingsVersion.V5` |
| IDE | Rider or VS 2022, MSVC toolset required by 5.7 |

---

## How a critical exception works

A minor-version change during the Vertical Slice requires **all** of:

1. A reproducible blocker that Epic will not hotfix on 5.7.
2. A written workaround attempt that failed.
3. Director + Technical sign-off.
4. A branch that proves 5.7→new version conversion on a copy, not on `main`.

Otherwise the answer is no.

---

## Environment note (this repository host)

This planning host does **not** have Unreal Editor, the Epic launcher, or MSVC installed. The lock is documentary and encoded in `TheBetrayedWill.uproject` + Target files. First live compile happens on a Windows (or official Linux) machine with UE **5.7.x** installed. That event is the first hour of Phase 1, not a reason to keep Phase 0 open.
