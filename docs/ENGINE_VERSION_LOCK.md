# ENGINE VERSION LOCK

**Project:** The Betrayed Will / وصية الغدر
**Status:** **LOCKED — Unreal Engine 5.8.1 only**
**Locked on:** 2026-08-14 · **Exact build confirmed:** 2026-08-18
**Owner:** Technical Director

---

## Active decision

| Field | Value |
|---|---|
| Engine | Unreal Engine 5 |
| Version | **5.8.1** |
| Verified install | `E:\UE_5.8` — installed build, `Engine\Build\InstalledBuild.txt` present |
| Registered identifiers | `{0B082D8B-4E00-1114-FDA5-5D980883089F}` and `5.8` (added per-user so Explorer resolves the project) |
| Association string | `"5.8"` in `TheBetrayedWill.uproject` |
| Platforms | Windows 64-bit |
| Include order | `EngineIncludeOrderVersion.Unreal5_8` |
| Build settings | `BuildSettingsVersion.V7` |
| Toolchain (verified) | MSVC 14.44.35228 · Windows SDK 10.0.22621 · bundled .NET 10.0 win-x64 · ISPC 1.24 |

**Verified compile:** `TheBetrayedWillEditor Win64 Development`, exit code 0, 0 errors — 2026-08-18.

---

## What must not exist

| Item | Rule |
|---|---|
| UE 5.6 / 5.7 | Do not install, target, open, or build with. There is no dual-version support. |
| A second `.uproject` | Only `TheBetrayedWill.uproject` at the repo root. |
| `backups/UE57_preserved/` | **Deleted 2026-08-18.** The 5.7 snapshot and the migration ZIP are gone from the repository. If it is ever needed it lives in git history at commit `502df34` and earlier. |
| `UE_5.6` on the build machine | Not used by any tool here. `phase2_build_and_check.ps1` refuses to build on a non-5.8 engine with exit code 2. |

`tools/validate_ue58.py` enforces the version lock structurally and fails the build gate if 5.7 reappears.

---

## Hotfix policy

Stay on **5.8.1** for the whole vertical slice. A hotfix upgrade (5.8.2+) is allowed only when it
fixes a reproducible blocker, and only as its own commit with a full rebuild and a PIE pass after it.
Never upgrade the engine in the same change as gameplay work.

## Exception process

A minor-version change requires a reproducible 5.8 blocker, a failed workaround, director sign-off,
and a branch test. Default answer is no.
