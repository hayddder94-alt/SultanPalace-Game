# ENGINE VERSION LOCK

**Project:** The Betrayed Will / وصية الغدر  
**Status:** **LOCKED — Unreal Engine 5.8 only**  
**Locked on:** 2026-08-14  
**Owner:** Technical Director

---

## Active decision

| Field | Value |
|---|---|
| Engine | Unreal Engine 5 |
| Minor version | **5.8** |
| Hotfix policy | Latest **5.8.x** hotfix on the production machine, then freeze that exact build |
| Association string | `"5.8"` in `TheBetrayedWill.uproject` |
| Platforms | Windows 64-bit |
| Include order | `EngineIncludeOrderVersion.Unreal5_8` |
| Build settings | `BuildSettingsVersion.V7` |

Do **not** install or target 5.6 or 5.7. There is no dual-version compatibility.

---

## HISTORICAL / ARCHIVED — prior 5.7 lock (2026-08-13)

The following is **not** current setup. It records why 5.7 was chosen before the production machine already had 5.8.

At that time 5.8 was ~7 weeks old and was rejected as too young. That decision was overturned because 5.8 is installed on the production PC and a second engine install is wasteful. The 5.7 tree is frozen under `backups/UE57_preserved/` (non-authoritative).

---

## Exception process

A minor-version change requires a reproducible 5.8 blocker, a failed workaround, director sign-off, and a branch test. Default answer is no.
