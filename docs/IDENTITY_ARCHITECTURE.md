# IDENTITY ARCHITECTURE — EVAN AND RAYNOR

**Status:** Locked. Stub exists in C++. Raynor is not playable in the Vertical Slice.

---

## 1. Rule

One pawn: `ATBWPlayerCharacter`.  
One component: `UTBWPlayerIdentityComponent`.  
One data type: `UTBWIdentityData`.

Differences are data. Duplicated gameplay classes are a defect.

```
ATBWPlayerCharacter
 ├── movement (shared)
 ├── camera (shared, combat tighten later)
 ├── input (shared map)
 ├── UTBWPlayerIdentityComponent
 │     └── UTBWIdentityData   (mesh, anim, speeds, noise, damage, reach, charge)
 ├── (Phase 3) UTBWCombatComponent   // reads identity multipliers
 └── (Phase 5) UTBWStealthComponent  // reads identity noise / visibility
```

---

## 2. Data

Shipped as `DA_Identity_Evan` and `DA_Identity_Raynor` under `Content/TBW/Data/Identities/`.

| Field | Evan | Raynor |
|---|---|---|
| MaxWalkSpeed | 420 | 380 |
| MaxCrouchSpeed | 160 | 140 |
| MaxSprintSpeed | 620 | 560 |
| NoiseMultiplier | 0.8 | 1.15 |
| VisibilityMultiplier | 0.85 | 1.2 |
| LightDamageMultiplier | 0.8 | 1.0 |
| HeavyDamageMultiplier | 0.85 | 1.25 |
| Poise | 0.8 | 1.3 |
| MeleeReach | 130 | 165 |
| bCanChargeHeavy | false | true |

Feel target: ten seconds of walking tells them apart. The slice only needs Evan’s ten seconds.

---

## 3. Vertical Slice

- Default identity: Evan.  
- ApplyIdentity(Raynor) is compiled and can be cheated in Development **after** Phase 2, but the slice build does not expose it.  
- No pause-menu character swap.  
- No second mesh in the playable pawn during VS (Raynor is a cine actor).

---

## 4. Full-game swap (not now)

MQ-06c may apply Raynor in place on the same pawn, or possess if that proves safer. Prefer in-place. Save stores `IdentityId`.

---

## 5. What we will not do

- `AEvanCharacter` and `ARaynorCharacter` as separate hierarchies  
- Different input maps  
- Different cameras as different code paths  
- A third playable
