# LEILA SPECIFICATION

**Project:** The Betrayed Will  
**Status:** Defined. **Not implemented** as a system.  
**Law:** No romance meter. No dating. No playable Leila. Flags only.

This file exists so the Vertical Slice can introduce her correctly, and so Phase 11 does not invent a relationship RPG.

---

## 1. Who she is

Daughter of High Priest Bel-Iddin. Raised in the Temple of Ishtar, not in House Orin. Betrothed to Raynor by Orin’s last political act, to bind the house to the temple. Not a blood relative.

She is attractive, intelligent, charismatic, and initially unreadable. Attractiveness is not the mechanic.

**Want:** the temple’s independence from House Orin *and* from the court.  
**Need:** to decide whether love is a political error.  
**Fear:** being used as a seal on a man’s document.  
**Performance key:** “I will not set fire to my temple to keep your house warm.”

---

## 2. Why she is important

1. She is the non-blood stake. Without her the story is only men fighting over a building.  
2. She has access — not ownership — to the path toward the second tablet.  
3. She can keep Evan alive in Chapter 5.  
4. She can leave Raynor the house and take the meaning with her.  
5. Nofan believes she might join him. She has not.

Information she possesses (not all at once):

| When she knows | What |
|---|---|
| Prologue | The official will. That the brothers’ eyes were unkind. |
| Ch 2–3 | That Raynor did not simply ride north. The old name *Kurnah* stills her. |
| Ch 5 | Enough to give Evan a survey mark and a token that opens the water gate once. |
| Ch 7–8 | How to ask Bel-Iddin for the second tablet — or how to refuse Raynor that path. |

She is not the secret author of the conspiracy.

---

## 3. Relationships

### Raynor

Betrothed. Real feeling and real calculation, both true. She does not belong to him. In the slice they share one terrace line. Later, the betrothal remains legally real while trust does not.

### Evan

She uses him because he can walk into rooms she cannot be seen wanting. She also keeps him alive. The player may resent her and thank her. There is no Evan/Leila romance twist.

### The family

Guest of the will, not a daughter of the house. Nofan has approached her. Darius ignores her. Malik fears her paper. Soren treats her as temple first. Orin used her as a seal and also, perhaps, as a hope.

### The temple

Her real house. If Raynor treats the tablet as a weapon against the temple, she withdraws.

---

## 4. Trust — design only

Three player-readable states. **Not a meter. Not a percentage. Not gifts.**

| State | How you get there | What it changes |
|---|---|---|
| `Closed` | Default, Prologue–Ch 3, and the entire Vertical Slice | She gives nothing free. |
| `Allied` | Ch 5 she chooses this; Ch 7 it holds if Raynor did not treat her as property and did not butcher Malik in rage | Temple path to the tablet. She can stand in the hall. |
| `Withdrawn` | Raynor kills Malik in rage, or uses the tablet against the temple, or humiliates her in the Ch 7 conversation | Soren / public path to the tablet. She leaves him the house. |

Three specific interactions (full game, not VS):

1. Evan, Ch 3: he brings her the grain line without demanding she choose a brother.  
2. Raynor, Ch 7: he asks for the tablet without claiming her.  
3. Raynor, Ch 8: he does not turn the tablet on the temple.

`LeilaTrust` is a flag with three values. That is the entire “relationship system.”

---

## 5. Vertical Slice appearance

- VS-02: does not smile when named in the will.  
- VS-03: “If they loved him, they would have been kinder in their eyes.”  
- No conversation menu. No gift. No follow.  
- The player must leave the slice knowing she is a person with a stake, not a reward.

---

## 6. Implementation ban until Phase 11

Do not build: affection points, flowers, companion AI, jealousy scenes, a kiss QTE, a journal tab called Relationship.

Do build, later: the three flags, the two tablet paths, her last scene in the hall.
