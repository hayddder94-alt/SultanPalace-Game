# STATUS — سجل التحقق الصادق

**آخر تحديث:** 2026-08-19 (مساءً) · **المحرك:** UE 5.8.1 · **الإصدار:** `0.2.0-systems-verified-ue58`

> هذه الوثيقة تفصل ما رآه إنسان يعمل عمّا هو مكتوب فقط. بلا تجميل.

---

## ✅ ما أصبح مُثبتًا اليوم — 2026-08-19

**بناء ناجح:** `TheBetrayedWillEditor Win64 Development` · رمز الخروج **0** · صفر أخطاء.

**اختبار ذاتي داخل محرك Unreal حقيقي (`-nullrhi`): 30 نجحت، 0 فشلت.**

| النظام | الفحوص | الحالة |
|---|---|---|
| أعلام العالم | كتابة، قراءة، مسح، جدول 14 علمًا، تحليل الأسماء | ✅ 6/6 |
| الحوار | تحميل من القرص، بدء مشهد، امتلاء الترجمة، تقدّم، توقف | ✅ 7/7 |
| **العربية** | **نقاط ترميز عربية موجودة فعلًا في الترجمة المعروضة** | ✅ |
| الأهداف | 9 قواعد، هدف فعّال، **التقدّم عند ضبط علم قصة** | ✅ 4/4 |
| الحفظ | كتابة خانة، مسح، تحميل، بقاء العلم، ملخّص، حذف | ✅ 7/7 |
| اللاعب والكاميرا | نوع الـpawn، الهوية، المتفاعل، HUD، حدّا الميلان −55/+70 | ✅ 5/5 |
| الحفظ التلقائي | اشتغل فعليًا عند أول علم: `Autosave (ClueFound_01): ok` | ✅ |

**أهم سطر في التقرير كله:**
```
PASS  Arabic survives JSON to subtitle  (Arabic codepoints present)
```
مسار النص العربي مُثبت من ملف JSON إلى الترجمة على الشاشة داخل محرك حقيقي. هذا يفتح الباب
لكتابة الـ17 مقطعًا الباقية بلا خوف من تكرار عطل التشويش الذي ظهر في المرحلة 1.

**ودليل على أن الإصلاحات تعمل:**
```
Level AUTHORED. Greybox suppressed (authored level).
```
منطق منع صندوق المجسّمات في الخرائط المؤلَّفة يعمل كما صُمِّم.

---

## ✅ القصر صار موجودًا — 2026-08-19

بُني بلا فتح المحرر، عبر `tools/BUILD_LEVEL.cmd`:

```
[TBW] actors placed : 220
[TBW] story beats   : 9
[TBW] cast staged   : 16
[TBW] level saved   : /Game/TBW/Maps/L_VS_Palace_EastWing
```

والاختبار الآلي على تلك الخريطة تحديدًا:

```
INFO  map 'L_VS_Palace_EastWing', 9 interactable(s), 16 cast member(s)
PASS  authored level has its story beats  (9 found)
PASS  authored level has its cast  (16 found)
PASS  at least one character can speak
TBW SELFTEST RESULT: 33 passed, 0 failed
```

الأسطر الثلاثة التي كانت `INFO ... 0` صارت **فحوصًا ناجحة**. القصر لم يعد ادعاءً.

## 🔨 مُترجَم لكن لم يُلعب بعد

| العنصر | لماذا |
|---|---|
| الجسد الهيكلي للاعب | `body: placeholder cube` — المحرك مثبَّت بلا مكوّن القوالب، انظر `FREE_ART_PLAN.md` |
| **المشي داخل القصر بعين بشرية** | الاختبار يثبت وجود 220 ممثلًا، لا كيف يبدو المكان ولا كيف يشعر |
| إحساس الحركة والكاميرا | يحتاج إنسانًا يلعب، لا اختبارًا آليًا |
| الترجمة العربية **مرسومة على الشاشة** | الاختبار أثبت النص، لا البكسل. يحتاج PIE بعين بشرية |

---

## 📝 أعطال حقيقية أظهرها هذا التشغيل، وأُصلحت فورًا

| العطل | السبب | الإصلاح |
|---|---|---|
| 4 تحذيرات `A Key Mapping with a blank action` | `Primary`/`Secondary` مربوطتان بمفاتيح لكن لم تُنشأ الإجراءات أصلًا | أُنشئت الإجراءات؛ الحجز صار حقيقيًا لا اسميًا |
| صندوق المجسّمات لم يعد يظهر في الخريطة الافتراضية | `Template_Default` يحتوي PlayerStart فاعتُبر «مؤلَّفًا» | أمر جديد `tbw.Dev.Sandbox` يستدعيه عند الطلب |

---

## ❌ ما لا يوجد إطلاقًا

مواد Megascans · MetaHuman · أي حركة (animation) · صوت أو موسيقى أو أداء صوتي ·
سينمائيات Sequencer · قتال · ذكاء اصطناعي · قوائم · نسخة مطبوخة · تحسين أداء.

---

## الخطوة التالية — تحويل «القصر» من مكتوب إلى مُشغَّل

```powershell
cd C:\Dev\SultanPalace-Game
.\tools\GO.cmd
```
ثم داخل المحرر:
1. **Add ◂ Add Feature or Content Pack ◂ Third Person ◂ Add** ثم **Starter Content**
2. أعد تشغيل المحرر
3. **Tools ◂ Execute Python Script ◂** `tools/ue_python/build_east_wing.py`
4. احفظ، ثم من PowerShell:
   ```powershell
   .\tools\selftest.cmd -Map /Game/TBW/Maps/L_VS_Palace_EastWing
   ```
   عندها ستتحول أسطر `INFO map ... 0 interactable(s)` إلى فحوص `PASS` حقيقية.

---

## 2026-08-19 — عطلان اكتُشفا من لقطة شاشة المحرر

| العطل | السبب | الإصلاح | التحقق |
|---|---|---|---|
| المحرر يعرض حوار **DataTable import options** على `Content/TBW/Data/Objectives.json` | مراقبة مجلدات المحتوى مفعّلة افتراضيًا، فيرى المحرر ملفات `.json` كـ«محتوى مصدري» ويعرض استيرادها. الاستيراد كان سيُنشئ `UDataTable` لا يقرأه التشغيل أبدًا، ثم ينحرف عن الملف المشحون فعلًا | `Config/DefaultEditor.ini` → `bMonitorContentDirectories=False` + استثناء `TBW/Data/*` | يتطلب إعادة تشغيل المحرر. `validate_narrative.py` صار يحذّر إن رجع الإعداد |
| **الأخطر:** `+DirectoriesToAlwaysCook=(Path="/Game/TBW/Data")` | هذا السطر يشمل الـ uassets فقط. ملفاتنا `.json` خام، فالطابخ (cooker) كان سيتجاهلها بالكامل: **النسخة المحزومة كانت ستُشحن بلا حوار وبلا أهداف**، ولا شيء قبل الإطلاق كان سيقول ذلك | استُبدل بـ `+DirectoriesToAlwaysStageAsUFS=(Path="TBW/Data")` — نفس آلية شحن خط DejaVu | فحص جديد داخل `tools/validate_narrative.py` يُفشل البناء إن اختفى السطر |
| `+MapsToCook` يشير إلى `L_Dev_Sandbox` و`L_VS_Palace` | خريطتان غير موجودتين؛ الخريطة الحقيقية `L_VS_Palace_EastWing` | صُحّح | — |

تحذير `Could not find Glyph Index 0 with codepoint U+61c` **ليس عطلًا**: هو محرف
ARABIC LETTER MARK داخل ترجمة واجهة المحرر نفسها على ويندوز عربي، ولا يوجد في
بياناتنا (`grep -rlP '\x{061C}' Content/TBW/Data/` → لا شيء). التفاصيل في
`docs/ARABIC_TEXT_PIPELINE.md`.
