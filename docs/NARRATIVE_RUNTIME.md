# NARRATIVE RUNTIME — الحوار والأهداف

**التاريخ:** 2026-08-18 · **المحرك:** UE 5.8.1 · **الحالة:** مكتوب، لم يُترجم على جهازك بعد

---

## القرار المعماري الأهم: العربية في البيانات لا في الشيفرة

كل نص عربي في اللعبة يعيش في ملفات JSON تحت `Content/TBW/Data/`، ولا يمرّ **أبدًا** عبر
مترجم C++.

السبب ليس أسلوبيًا. في المرحلة 1 ظهر عنوان اللعبة على الشاشة هكذا:

```
ÙˆØµÙŠØ© Ø§Ù„ØºØ¯Ø±
```

لأن MSVC يقرأ ملفات المصدر كـ CP1252 ما لم يُجبَر على غير ذلك. الحل وقتها كان ترميز
العنوان بهروب Unicode `\u0648\u0635...` — وهو حلّ لا يصلح لآلاف أسطر الحوار.

النص المقروء عبر `FFileHelper::LoadFileToString` من ملف UTF-8 لا يمرّ بهذا الالتباس إطلاقًا.
لذلك: **الشيفرة إنجليزية بالكامل، والنصوص كلها بيانات.**

---

## ما بُني

| المكوّن | الملف | الوظيفة |
|---|---|---|
| أنواع الحوار | `Narrative/TBWDialogueTypes.h` | سطر ومشهد: متحدث، إنجليزي، عربي، مدة، سطر وصفي |
| مشغّل الحوار | `Narrative/TBWDialogueSubsystem` | يقرأ كل ملفات المشاهد، يشغّل، يتقدّم تلقائيًا بمؤقّت، يضبط علم الإنجاز |
| الأهداف | `Narrative/TBWObjectiveSubsystem` | يشتقّ الهدف الحالي من أعلام العالم |
| العرض | `UI/TBWHUD` | ترجمة سفلية وهدف علوي، **بـ Slate** لا Canvas |
| الربط | `TBWInteractableActor::PlaysDialogueScene` | لمس غرض يشغّل مشهدًا |
| المحتوى | `Content/TBW/Data/` | VS-01 كاملًا (13 سطرًا) + 9 قواعد أهداف |

### لماذا Slate وليس Canvas مجددًا

`AHUD::DrawText` يمرّ عبر `DroidSansFallback` ولا يستطيع **تشكيل** العربية — الحروف تظهر
منفصلة ومعكوسة. Slate يستخدم HarfBuzz ويشكّلها صحيحة. الترجمة السفلية والهدف كلاهما
`STextBlock` مع `ETextFlowDirection::Auto`، تمامًا كما فُعل مع العنوان في المرحلة 1.

### ما **لم** يُبنَ عمدًا

لا شجرة حوار، ولا اختيارات، ولا حالة علاقات مع الشخصيات، ولا نظام مهام برسم بياني.
«وصية الغدر» قصة خطّية مروية؛ محرك تفرّعات لا نحتاجه هو محرك تفرّعات سنضطر لتصحيحه.
الأهداف مجرد قاعدة: «إن كانت هذه الأعلام مضبوطة وهذه ليست، فالهدف هو كذا».

---

## أوامر التحكم

```
tbw.Dialogue.Play                     يسرد كل المشاهد المتاحة
tbw.Dialogue.Play VS01_OrinLastWords  يشغّل مشهد موت أورين
tbw.Dialogue.Skip                     يتخطى السطر، أو المشهد إن كان قابلًا للتخطي
tbw.Dialogue.Reload                   يعيد قراءة كل ملفات السرد من القرص أثناء اللعب
tbw.Objective                         الهدف الحالي ونسبة التقدّم
tbw.Lang ar | en                      لغة الترجمة
```

`tbw.Dialogue.Reload` مقصود: تعدّل نصًا في JSON وتضغط الأمر فترى النتيجة فورًا بلا إعادة
ترجمة ولا إعادة تشغيل. هذا يختصر دورة كتابة الحوار من دقائق إلى ثوانٍ.

**VS-01 غير قابل للتخطي في المرة الأولى** — منصوص عليه في السيناريو، ومطبّق في `CanSkip()`.

---

## المدقّق الجديد

```bash
python3 tools/validate_narrative.py
```

يفحص ما لا يستطيع المترجم فحصه لأن النص بيانات:

1. كل ملف JSON صالح ومُرمَّز UTF-8 فعلًا.
2. كل `completion_flag` وكل علم في الأهداف **له حالة في** `FTBWWorldFlags::ToName`.
3. حقل `ar` يحتوي حروفًا عربية فعلًا (يمسك النسخ الخاطئ من الإنجليزية).
4. كل سطر له نص إنجليزي — لغة الإنتاج لا تسقط أبدًا.
5. كل متحدث موجود في `CHARACTER_BIBLE.md`.
6. **سرعة القراءة**: أكثر من 22 حرفًا في الثانية = تحذير، لأن المشاهد لن يلحق.
7. هدف يطلب علمًا ويمنعه في آن واحد = خطأ (محتوى ميت).
8. هدف يطلب علمًا لا يضبطه شيء في المستوى ولا في الحوار = تحذير.

النتيجة الحالية: **PASS** — مشهد واحد، 13 سطرًا، 0.9 دقيقة محتوى منطوق، 9 أهداف.

---

## الاختبار على جهازك

```powershell
git pull origin arena/019ffc4c-sultanpalace-game
.\tools\build_phase2.cmd
.\tools\run_editor.cmd
```

ثم داخل PIE:

1. `tbw.Objective` → يجب أن يطبع هدف VS-01.
2. `tbw.Dialogue.Play VS01_OrinLastWords` → تظهر الترجمة العربية أسفل الشاشة سطرًا بسطر.
3. عند انتهاء المشهد يُضبط `OrinLastWords`، وتحقق بـ `tbw.Flags.List`.
4. `tbw.Objective` مرة أخرى → يجب أن يتغيّر الهدف إلى VS-02 تلقائيًا.
5. `tbw.Lang en` ثم أعد التشغيل → الترجمة إنجليزية.

**الفحص الحاسم:** إن ظهرت العربية متصلة ومقروءة من اليمين لليسار، فمسار النص العربي كله
مُثبت من الشيفرة إلى الشاشة، ويمكن كتابة بقية الـ18 مقطعًا بلا قلق.

---

## الخطوة التالية في السرد

VS-01 مكتوب. الباقي — VS-02 حتى VS-18 — نصوصه **مقفولة أصلًا** في
`docs/VERTICAL_SLICE_SCRIPT.md`. تحويلها إلى ملفات JSON عمل نسخ منظّم لا تصميم،
ويمكن أن يتم دفعةً واحدة بعد أن تؤكد أن VS-01 يظهر صحيحًا على الشاشة.

لن أحوّل ثمانية عشر مقطعًا قبل أن نرى مقطعًا واحدًا يعمل. هذا ليس تردّدًا، بل ترتيب:
خطأ واحد في المسار مضروبًا في 18 مقطعًا هو 18 خطأً.

---

## نظام الحفظ (أُضيف 2026-08-18)

لا توجد لعبة بلا حفظ. الملفات: `Save/TBWSaveGame.h` و `Save/TBWSaveSubsystem`.

### ما يُحفظ
أعلام العالم كاملة، وموضع اللاعب ودورانه، والهوية الحالية، والمشاهد التي شوهدت، ووقت اللعب،
وسطر ملخّص جاهز لقائمة التحميل (المقطع + الهدف + التاريخ + المدة).

### ثلاث قواعد تمنع تعفّن الحفظات

1. **الحفظة تحمل رقم نسختها.** إن اختلف الرقم عن نسخة البناء تُرفض بوضوح ولا تُحمَّل جزئيًا.
   التحميل الجزئي الصامت هو ما يوقع لعبة قصة في حالة مستحيلة لا يستطيع اللاعب الخروج منها.
2. **الحفظ التلقائي في خانة منفصلة** (`TBW_Auto`) عن اليدوي (`TBW_Quick`). حفظة تلقائية سيئة
   لا يجوز أن تدمّر حفظة يدوية يعتمد عليها اللاعب.
3. **لا نحفظ لاعبين ولا ممثلين (actors) بالتسلسل.** كل ما تحتاجه القصة موجود في الأعلام.
   صيغة حفظ تسلسل الممثلين تنكسر مع كل تغيير في صنف C++.

### الحفظ التلقائي عند تقدّم القصة

`ATBWGameMode` يستمع إلى `OnFlagChanged`، وعند ضبط علم قصة **معرّف في التعداد** (لا علم
تصحيح عابر) يحفظ تلقائيًا. حمايتان ضروريتان:

- **حارس أثناء التحميل**: تحميل حفظة يكتب عشرات الأعلام؛ بلا الحارس كان كل واحد منها
  سيُطلق حفظًا تلقائيًا فنحفظ فوق ما نحمّله للتو.
- **خنق زمني ثلاث ثوانٍ**: المشهد الواحد قد يضبط عدة أعلام؛ نقطة تفتيش واحدة تكفي.

### الأوامر

```
tbw.Save [slot]          حفظ (الافتراضي TBW_Quick)
tbw.Load [slot]          تحميل
tbw.Save.List            ما في الخانات القياسية
tbw.Save.Delete <slot>   حذف خانة
```

---

## Vertical slice dialogue — written 2026-08-19

The Arabic pipeline was proven on screen in PIE that day (see
`docs/ARABIC_TEXT_PIPELINE.md`), which was the condition I had set for
converting the rest of the locked script. VS-02 … VS-14 are now JSON.

| Scene file | Segment | completion_flag | Lines | Attached to |
|---|---|---|---|---|
| `VS01_OrinLastWords.json` | VS-01 | `OrinLastWords` | 13 | `OrinBedside`, `Raynor_VS01` |
| `VS02_TheWillReading.json` | VS-02 | `WillWasRead` | 13 | `WillTablet` |
| `VS03_EyesInTheHall.json` | VS-03 | `RaynorDisappeared` | 2 | `Leila` |
| `VS04_MorningYouAreEvan.json` | VS-04 | — | 1 | (first-move trigger, not wired) |
| `VS05_LearnTheHouse.json` | VS-05 | — | 2 | `EmptyChair`, `ServantA` |
| `VS06_TheUnusedRoom.json` | VS-06 | `EvanSuspicious` | 5 | `ScarOil` |
| `VS07_SorenWillNotLook.json` | VS-07 | `SorenSpoken` | 4 | `Soren` |
| `VS07a_KeepThis.json` | VS-07 | `SorenRespected` | 4 | choice branch 1 |
| `VS07b_GoBack.json` | VS-07 | — | 4 | choice branch 2 |
| `VS08_YasminPerformedJoy.json` | VS-08 | `YasminSpoken` | 6 | `Yasmin` |
| `VS08b_MalikAtTheDoor.json` | VS-08b | `MalikSpoken` | 4 | `Malik` |
| `VS09_TheCanalClasp.json` | VS-09 | `BoatmanSpoken` | 6 | `Boatman` |
| `VS10_TheNightRoster.json` | VS-10 | — | 2 | `Roster` |
| `VS11_TheManInTheCourt.json` | VS-11 | `AnnexChallenged` | 1 | `GuardCourt` |
| `VS12_WrongWax.json` | VS-12 | — | 4 | `FalseLetter` |
| `VS13_TheBoardConnects.json` | VS-13 | `FamilyConnected` | 3 | (auto-connect, not wired) |
| `VS14_NofansKindness.json` | VS-14 | `SliceComplete` | 9 | `Nofan` |

**17 scenes, 83 lines, 5.7 minutes of authored subtitle time.** Line durations
are computed at 12 characters per second of the longer language, floor 1.8 s,
so no hand-typed number can breach the 22 chars/s readability ceiling that
`tools/validate_narrative.py` enforces.

### What is deliberately NOT in these files

* **The VS-07 choice has no chooser.** The runtime plays a linear list of
  lines; it has no branch selection and no input for one. Rather than invent a
  schema the runtime cannot read, `VS07` stops before the choice and the two
  answers live in `VS07a` / `VS07b`, playable directly via
  `tbw.Dialogue.Play VS07a_KeepThis`. `VS07`'s `_choice_pending` field names
  them. When a chooser is built, the wiring is two lines.
* **VS-11 has no fight.** The script calls for a short melee. Combat, health,
  stamina, enemy AI and perception are outside the authorised scope. The scene
  is the guard's one line; `AnnexChallenged` records that the line was heard.
* **VS-10 has no stealth.** Same reason: guard patrols and detection are not
  implemented. The scene is the roster examine narration.
* **VS-04 and VS-13 have no trigger yet.** VS-04 wants a first-movement
  trigger; VS-13 wants the investigation board's auto-connect. Neither exists,
  so both scenes are authored and reachable by console only.

### Console

```
tbw.Dialogue.Reload
tbw.Dialogue.Play VS02_TheWillReading
tbw.Objective
```

---

## 2026-08-20 — two scenes that nothing could ever play

`VS04_MorningYouAreEvan` and `VS13_TheBoardConnects` were authored, validated
and completely unreachable. Every check passed and no path through the game
could reach either of them. **A scene only the console can play is a document,
not a game.**

| Scene | Trigger now | Where |
|---|---|---|
| VS-04 | the player's first movement input | `TBWPlayerCharacter.cpp` `PlayWakeLineOnce()` |
| VS-13 | `ClaspFound` + `RosterAltered` + `LetterIsFalse` all set | `TBWGameMode.cpp` `CheckStoryTriggers()` |

VS-04 fires on the first movement rather than on `BeginPlay` because the script
says what the moment is: *"Move. Look. Understand you are not the man from the
night."* The line lands on the player's own first decision, not on a loading
screen.

VS-13 is checked **before** the autosave throttle, not after. The throttle
stops one scene setting three flags from writing three saves; a story trigger
must not be swallowed by that same rule.

### `PlaySceneOnce`

"First time" means two things and both matter:

* not already played in this session, and
* its completion flag not already set.

Without the second, loading a save and taking one step replays Orin's death.

### This is not a quest graph

`CheckStoryTriggers()` is a short, explicit list of *when these are all true,
this happens once*. It has no nodes, no state machine and no data asset.
Anything needing more than that belongs in a system nobody has authorised.

### The check that stops it recurring

`tools/validate_narrative.py` now cross-references every scene id against the
`scene` field of every interactable and character in the layout, plus a table
of code-triggered scenes that names the function playing each one. A scene
reachable by nothing is reported. The code-trigger table is verified against
the actual sources, so deleting a trigger fails the check instead of silently
orphaning the scene.

Known exception: `VS07a_KeepThis` and `VS07b_GoBack` are branches of a choice
that has no UI yet. Documented, not forgotten.
