# WORLD DESIGN

**Project:** The Betrayed Will / وصية الغدر  
**Status:** Locked for planning  
**World type:** Compact, authored, interconnected. **Not** an open world.

---

## 1. Design law

The world is a set of **seven major locations**, plus short authored connectors. Every location must appear in at least two story states. We buy perceived depth with lighting, occupancy, damage, and who is allowed to speak — not with square kilometers.

If a space is used once, it is probably too expensive. Cut it or reuse it.

## 2. World diagram

```
                    [ Temple Area ]
                          |
                          | processional street (authored)
                          |
 [ Noble District ] — [ Palace ] — [ Market District ]
                          |
                          | canal / service streets
                          |
              [ Residential District ]
                          |
                          | reed boat transition (authored, not a lake)
                          |
                  [ Kurnah Hold / Prison ]
                          |
                          | (Ch 8 only, short)
                          |
              [ Final Hall = Palace Audience Hall
                 in a last story state ]
```

The final confrontation location is **not a new map**. It is the palace audience hall after war has been through it. That is a production decision and a story decision.

Travel between major locations is:

- A walkable authored street (60–180 seconds), or
- A short canal-boat or corridor transition with a load underneath (15–40 seconds of player-facing time).

No world map with fog of war. No mount. No fast-travel grid. A simple “return to palace gate / temple gate / dock” list unlocks after Ch 2 for already-visited gates, so the player is never forced to replay a street for padding.

## 3. Location briefs

### 3.1 Orin’s Palace — hero space

**Function:** Identity of the game. Family, ritual, lies, ending.  
**Used in:** Prologue, Ch 1, Ch 2, Ch 4 (exterior/storehouse), Ch 7, Ch 8.

**Wings (all interior, all reusable):**

| Wing | Gameplay | Story |
|---|---|---|
| Audience hall | Dialogue, finale | Will reading; last confrontation |
| Family residences | Explore, investigate | Empty beds, stolen rings, mourning lamps |
| Archive / steward rooms | Investigate | Malik’s paper war |
| Kitchen yard + servant run | Explore, eavesdrop | Yasmin, rumor |
| Training court | Combat tutorial / Darius fight | House muscle |
| Terrace and canal gate | Cinematic, stealth start | Leila; Raynor’s last prologue walk |
| Service undercroft | Stealth | How the house moves bodies and grain |

**States:**

| State | When | What changes |
|---|---|---|
| P_DyingNight | Prologue | Low lamps, few servants, Orin alive upstairs |
| P_NamedMorning | Ch 1–2 | Performed celebration, empty heir chair, extra guards on the canal |
| P_MourningEvan | Ch 4–5 (seen from outside / Ch 7 return) | Evan’s lamp, hushed staff, Nofan hosting |
| P_ReturnedNight | Ch 7 | Lockdown, patrols, training court hostile |
| P_LastHall | Ch 8 | Scored damage, daylight cut by shutters, the will physically present |

**Budget note:** This is the only location that deserves hero materials, hero lighting rigs, and the most set dressing.

### 3.2 Babylonian residential district

**Function:** Human scale. Where Evan hides after the drowning.  
**Used in:** Ch 2, Ch 5, Ch 7 (short).

Mudbrick lanes, courtyard houses, a shrine niche, Old Nabeh’s reed shed, a shared well. Play spaces are two streets, one courtyard cluster, one rooftop run. That is enough.

**States:** Day curiosity (Ch 2) vs. night refuge (Ch 5) vs. whispered recognition of Raynor (Ch 7).

### 3.3 Market district

**Function:** Information, one street confrontation, the grain-dock clue.  
**Used in:** Ch 3, Ch 7 (optional).

A spice row, a bronze stall, a grain dock office, a covered lane for a short chase or stealth miss. Iltani lives here. Hazar’s men can appear as “not city watch.”

Do not build a full bazaar simulation. Build one memorable market street that photographs well.

### 3.4 Temple area

**Function:** Leila, Bel-Iddin, the second tablet, moral pressure.  
**Used in:** Ch 2–3, Ch 5, Ch 8.

Processional approach, public court, a restricted archive, a vault (Ch 8). Lighting is cooler, more incense, more echo. Gameplay is mostly dialogue and investigation, with one stealth archive in Ch 3 and a vault infiltration or granted entry in Ch 8 depending on LeilaTrust.

### 3.5 Noble district

**Function:** The world outside the family. Hazar’s pressure.  
**Used in:** Ch 3 (edge), Ch 7–8.

One gated street, one townhouse courtyard, one reception room. If Hazar is cut as an on-screen character, this district shrinks to a single courtyard and a letter. It is the first *location* on the cut list after any one-off set.

### 3.6 Kurnah Hold (prison / fortress)

**Function:** Rescue, Evan’s death, the game’s darkest architecture.  
**Used in:** Ch 5 approach, Ch 6 full, Ch 7 optional return for a remaining prisoner/clue.

Marsh, reed wall, water gate, cisterns, inner cells, inner gate, a ruined upper walk. Vertical, wet, torch-lit. This is the stealth-combat showcase.

**States:** Occupied prison (Ch 6), emptied and bloody (optional Ch 7).

### 3.7 Final confrontation location

The palace audience hall in state `P_LastHall`. Possibly preceded by a short push through the undercroft and training court so the player *arrives*.

Do not build a volcano, a ziggurat summit arena, or a throne room of the King.

## 4. Connectors

| Connector | From–To | Player-facing | Production |
|---|---|---|---|
| Processional street | Palace–Temple | Walkable, 90s | Hero lighting, reused |
| Canal service path | Palace–Residential | Walkable / short boat | Reused night and day |
| Market gate street | Palace–Market | Walkable, 75s | Medium |
| Noble climb | Market–Noble | Walkable, 60s | Cheap, first cut |
| Reed boat | Residential–Kurnah | Authored transition | Cinematic + load |
| Undercroft spine | Palace wings | Walkable stealth | Reused often |

Loads happen behind boat rides, door rituals, and short follow-cams. The player should almost never see a black loading screen after the first boot.

## 5. Time, weather, light

There is **no systemic day/night cycle**. Time of day is a story state per location.

Allowed palettes:

- Oil-night (prologue, Ch 6, Ch 7)
- White morning (Ch 1)
- Heat-haze noon (Ch 3 market)
- Blue hour (Ch 5 boat, several terraces)
- Storm-threat over the marsh (Ch 6), rain only if the VFX budget is healthy after VS

Weather is authored, not simulated. Wind in reeds matters more than a cloud system.

## 6. Environmental storytelling toolkit

Every reused space should change at least three of:

1. Who is present
2. Who is allowed to speak
3. What light is doing
4. What is damaged or missing
5. What sound is doing (see Audio Plan)
6. What the player is allowed to touch

Examples already locked:

- Raynor’s unused bed (Ch 1) → stripped room (Ch 7)
- Audience hall celebration cloth (Ch 1) → mourning cloth (Ch 4/7) → bare table and the will (Ch 8)
- Canal water as path, grave, and road back
- Kitchen that will not feed Evan after he is “dead” (Ch 5, observed from a door)

Clutter is authored. No procedural debris.

## 7. Scale and density

| Metric | Target |
|---|---|
| Major locations | 7 (one is a state of another) |
| Unique playable interiors of note | ~18 rooms |
| Unique authored streets | 5 |
| Named NPCs on-screen in a typical hour | 4–8 |
| Background bodies in the densest space (market) | 12–20, mostly cards / simple AI |
| Simultaneous combat enemies | 1–4, never a dozen |
| Secrets per major location | 1–2, story-flavored, not collectible spam |

## 8. Navigation and readability

- Landmark rule: from any playable outdoor spot the player can see one of: the palace terrace, the temple mass, the market gate lion, or the canal.
- Interior: warm light = permitted, cold torch = restricted, green-oil = family-only.
- No minimap. A simple compass-free objective mark is allowed. A pause-map is a hand-authored plan of the current location, not a world map.
- Climbing is limited to authored mantle points (waist-high + a few story ladders). No parkour system.

## 9. Historical visual rules

Use Neo-Babylonian references:

- Glazed brick blues and golds on ceremonial faces (palace gate, temple court)
- Mudbrick and bitumen in working spaces
- Reed, water, dust, date palm, incense smoke
- Lions, mušḫuššu-adjacent motifs as architectural language — stylized, not copied from a specific copyrighted reconstruction
- Bronze, leather, linen, wool. No plate armor. No medieval crenellations.

Do not copy:

- Ishtar Gate as a 1:1 tourist replica
- Any single museum piece as a prop you could identify in a lawsuit
- Islamic geometric palaces, Ottoman interiors, or generic “Arabian Nights” pink sandstone from the wrong millennium

The README’s “Islamic palace simulation” is explicitly rejected here.

## 10. Asset architecture for replacement

All world assets are placeholder-safe:

```
/Content/Environments/
    /Palace/
    /Residential/
    /Market/
    /Temple/
    /Noble/
    /Kurnah/
    /Shared/          # modular mudbrick, trims, doors
    /Lighting/
    /Foliage/
    /Decals/
    /Probes/
```

Rules:

- Modular kit first, hero pieces second.
- Naming: `SM_Pal_Column_A`, `SM_Mkt_Stall_Cloth_B`.
- No Megascans / marketplace pack ships in the final build unless the license is cleared and the look is painted through our material layer.
- Every hero room has a dressing pass list, not “art until it looks good.”

## 11. Streaming and persistence

- One persistent “hub” level for the palace + a streamed sublevel per wing.
- Other districts are their own persistent levels with a transition.
- World state is a flag set (`WorldStateSubsystem`), not a physics snapshot of every pot.
- Doors, corpses that matter, and clue items are saved. Ambient pots are not.

## 12. Cuts, in order, if the world is over budget

1. Noble district as a space (keep Hazar in letters)
2. Optional Ch 7 return to Kurnah
3. Market side lane
4. Palace undercroft loop (keep a single corridor)
5. Any rooftop run that is only “variety”

Never cut the palace audience hall, Raynor’s chamber, the canal gate, the temple court, or Kurnah’s inner gate. Those are the story.
