# WINDOWS SETUP — من الصفر إلى بناء المرحلة 2

**المحرك:** Unreal Engine **5.8 فقط** · **الفرع:** `arena/019ffc4c-sultanpalace-game`
**المستودع:** https://github.com/hayddder94-alt/SultanPalace-Game (عام — لا يحتاج كلمة مرور)

---

## لماذا فشل أمرك السابق

```
PS C:\Users\dell> git pull origin arena/019ffc4c-sultanpalace-game
fatal: 'origin' does not appear to be a git repository
```

`C:\Users\dell` هو مجلد المستخدم، وليس مجلد المشروع. الأمر `git pull` يعمل فقط **داخل** نسخة
مستنسخة من المستودع، و`.\tools\...` يعمل فقط عندما تكون بداخلها. نسخة المشروع التي بنيت بها
المرحلة 1 وصلتك عبر ملف ZIP، لذلك هي على الأرجح ليست مستودع git ولا تعرف `origin`.

---

## الطريقة الأسرع — انسخ والصق كتلة واحدة

افتح **PowerShell** والصق هذا كما هو:

```powershell
git clone --branch arena/019ffc4c-sultanpalace-game https://github.com/hayddder94-alt/SultanPalace-Game.git C:\Dev\SultanPalace-Game
cd C:\Dev\SultanPalace-Game
.\tools\build_phase2.cmd
```

هذا كل شيء. حجم المستودع أقل من 5 ميغابايت.

> **لماذا `.cmd` وليس `.ps1`؟** ويندوز يرفض تشغيل سكربتات PowerShell غير الموقّعة رقميًا:
> `... is not digitally signed. You cannot run this script on the current system.`
> ملف `build_phase2.cmd` يشغّل نفس السكربت مع تجاوز السياسة **لهذه العملية فقط** — لا يغيّر
> أي إعداد دائم على جهازك، ولا يحتاج صلاحيات مدير. يمكنك أيضًا **النقر عليه مرتين** من مستكشف الملفات.

### إن أردت البقاء داخل PowerShell بدل `.cmd`

نفّذ هذا السطر مرة واحدة في نافذة PowerShell الحالية (يزول أثره بإغلاق النافذة):

```powershell
Set-ExecutionPolicy -Scope Process -ExecutionPolicy Bypass -Force
```

ثم:

```powershell
.\tools\phase2_build_and_check.ps1
```

### إعداد آلي كامل

انقر مرتين على `tools\setup_windows.cmd` — يستنسخ أو يحدّث المشروع في `C:\Dev\SultanPalace-Game`
ويضبط الفرع تلقائيًا. أضف `-Build` ليبني مباشرة بعد ذلك.

---

## مجلدك الحالي ليس نسخة git

الرسالة `fatal: 'origin' does not appear to be a git repository` في
`C:\Users\dell\Desktop\a\aa` تعني أن هذا المجلد ملفات مفكوكة من ZIP، وليس نسخة مستنسخة.
لذلك `git pull` لن يعمل فيه أبدًا، وستضطر لإعادة التنزيل يدويًا في كل تحديث.

للتحقق بنفسك:

```powershell
git -C C:\Users\dell\Desktop\a\aa remote -v
```

فراغ = ليست نسخة git.

**التوصية:** استنسخ إلى `C:\Dev\SultanPalace-Game` واترك مجلد سطح المكتب كما هو. سببان:

1. `git pull` سيجلب كل تحديث بأمر واحد.
2. سطح المكتب في ويندوز غالبًا مُزامن مع OneDrive. بناء Unreal داخل مجلد مُزامن يولّد
   مجلدات `Binaries` و `Intermediate` بمئات الميغابايت، فتحدث أخطاء قفل ملفات وبطء شديد.
   ابقِ المشروع خارج OneDrive دائمًا.


---

## في كل مرة لاحقًا

```powershell
cd C:\Dev\SultanPalace-Game
git pull origin arena/019ffc4c-sultanpalace-game
.\tools\build_phase2.cmd
```

- إن ظهر `git is not recognized` → ثبّت Git من https://git-scm.com/download/win ثم أعد فتح PowerShell.

---

## إذا قال السكربت `Unreal Engine 5.8 not found`

السكربت الآن يبحث عن المحرك في أربعة مصادر: متغير `UE58_ROOT`، وملف Epic Launcher
`LauncherInstalled.dat`، وسجل ويندوز (تثبيتات Launcher وبناءات المصدر)، ثم المجلدات
المعتادة على كل أقراصك. إن لم يجده، شغّل:

```powershell
.\tools\find_ue58.cmd
```

يطبع لك **كل** نسخ Unreal الموجودة على الجهاز مع رقم إصدار كل واحدة ومصدر اكتشافها، ثم:

- إن وجد 5.8 → احفظ مساره دائمًا: `.\tools\find_ue58.cmd -Remember` ثم أعد فتح PowerShell.
- إن كان المحرك مثبّتًا في مكان غريب → مرّر مساره يدويًا (المجلد الصحيح هو الذي يحتوي
  `Engine\Build\BatchFiles\Build.bat`):
  ```powershell
  .\tools\build_phase2.cmd -EngineRoot "D:\your\path\UE_5.8"
  ```
- إن لم يكن 5.8 مثبّتًا أصلًا → Epic Games Launcher ◂ Unreal Engine ◂ Library ◂ زر `+` ◂ اختر **5.8**.
  يحتاج نحو 60 غيغابايت مساحة حرة. لا تُثبّت رموز التنقيح (Editor symbols) إلا إن احتجتها.

**مهم:** المشروع مقفول على 5.8. إذا اكتشف السكربت إصدارًا آخر (5.5 أو 5.6 أو 5.7) فسيتوقف
عمدًا برمز خروج `2` بدل أن يغرقك بجدار أخطاء لا علاقة لها بالشيفرة.

---

## ماذا عن نسخة المشروع القديمة عندك؟

اتركها كما هي، ولا تبنِ منها. **النسخة المعتمدة الوحيدة** هي `C:\Dev\SultanPalace-Game`.
لا يوجد شيء مهم في النسخة القديمة: كل الشيفرة والإعدادات وخط `DejaVuSans.ttf` داخل المستودع.
مجلدات `Binaries` و `Intermediate` و `Saved` و `DerivedDataCache` يولّدها المحرك تلقائيًا.

إن أردت الاحتفاظ بلقطات أو إعدادات محرر من النسخة القديمة، انسخ فقط ما تحتاجه يدويًا.

---

## بعد نجاح البناء

1. افتح `C:\Dev\SultanPalace-Game\TheBetrayedWill.uproject` بنقرة مزدوجة.
   - إن سُئلت عن إعادة بناء الوحدات، اختر Yes.
   - إن سُئلت عن اختيار إصدار المحرك، اختر **5.8**.
2. اضغط **Play** (PIE).
3. امشِ على `docs/PHASE2_PIE_CHECKLIST.md` وسجّل النتائج.
4. ألصق لي: كتلة **SUMMARY** من السكربت + الصفوف الفاشلة + أرقام الأداء.

---

## ما تحتاج لصقه إن فشل شيء

- كتلة SUMMARY كاملة (فيها إصدار المحرك ورمز الخروج وعدد الأخطاء).
- أول 10 أسطر أخطاء كما طبعها السكربت.
- مسار ملف السجل الكامل موجود في نهاية المخرجات: `Saved\Logs\Phase2_Build_<تاريخ>.log`.


---

## حالة جهازك الفعلية (مسجّلة بعد الاكتشاف)

```
5.6.1   C:\Program Files\Epic Games\UE_5.6     ← تثبيت Epic Launcher (لا تستخدمه لهذا المشروع)
5.8.1   E:\UE_5.8                               ← بناء من المصدر (source build)  ✔ المعتمد
```

نقطتان مهمتان بسبب أن 5.8 عندك **بناء من المصدر** لا تثبيت Launcher:

1. **توليد ملفات المشروع:** الخيار `-rocket` صالح فقط للتثبيتات الجاهزة. السكربت صار يكتشف
   نوع المحرك عبر وجود `Engine\Build\InstalledBuild.txt` ويمرّر الخيارات الصحيحة تلقائيًا.
2. **معرّف المحرك:** بناءات المصدر تُسجَّل في
   `HKCU\SOFTWARE\Epic Games\Unreal Engine\Builds` باسم خاص (غالبًا GUID). ملف المشروع
   يطلب `EngineAssociation = "5.8"`. البناء من سطر الأوامر لا يتأثر إطلاقًا، لكن **النقر المزدوج**
   على `TheBetrayedWill.uproject` من مستكشف الملفات قد يفشل بعدم إيجاد محرك. الحل بأمر واحد:

   ```powershell
   .\tools\find_ue58.cmd -RegisterAs58
   ```

   يضيف قيمة واحدة في سجل المستخدم (بدون صلاحيات مدير) تربط الاسم `5.8` بمسار `E:\UE_5.8`.
   للتراجع: احذف القيمة `5.8` من المفتاح أعلاه. لن نعدّل `EngineAssociation` في المستودع
   لأن المشروع مقفول على `5.8` وهذا القفل مقصود.

3. **لا تبنِ بـ 5.6.** السكربت يرفض ذلك تلقائيًا برمز خروج `2`.


---

## خطأ `You must install or update .NET to run this application.`

سببه أن `UnrealBuildTool.exe` يحتاج .NET مثبّتًا على مستوى النظام. Unreal يشحن نسخته
الخاصة من .NET داخل المحرك في:

```
E:\UE_5.8\Engine\Binaries\ThirdParty\DotNet\...\dotnet.exe
```

السكربت الآن يستخدم هذه النسخة المرفقة (`dotnet.exe UnrealBuildTool.dll`)، ويسقط تلقائيًا
إلى `Build.bat` الذي يهيّئ البيئة بنفسه. **لا تحتاج تثبيت .NET يدويًا.**

كما أن توليد ملفات المشروع خطوة اختيارية: إن فشلت، يكمل السكربت إلى الترجمة بدل التوقف.

### عيب ثانٍ صُحّح في نفس الدفعة

كان `$ErrorActionPreference = "Stop"` مع `2>&1` يحوّل أي سطر يكتبه المترجم على مجرى الخطأ
إلى **خطأ قاتل يوقف السكربت** — وهذا ما أنهى التشغيل السابق قبل أن تبدأ الترجمة أصلًا.
الأدوات الأصلية (UBT، MSVC) تكتب تقدّمها وتحذيراتها على stderr بشكل طبيعي، فكان البناء
سيتوقف عند أول تحذير. صار تنفيذ الأدوات داخل دالة تُرخي هذا الإعداد أثناء الاستدعاء فقط.

---

## When `git pull` refuses

`.\tools\GO.cmd` pulls before it builds. If that pull fails, the build never
happens — and until 2026-08-19 the script announced *"check the internet
connection"* no matter what the real cause was, then filtered git's own words
out of the copied report. So the report said a pull had failed and gave no
reason. That is fixed: GO now asks git what is wrong and prints the specific
remedy.

But GO pulls *itself*, so a broken pull cannot deliver its own fix. Use this
instead — it diagnoses first and picks the safe action:

```
.\tools\PULL.cmd
```

| Flag | Effect |
|---|---|
| *(none)* | stash local edits, pull, put them back |
| `-Discard` | throw local edits away, then pull |
| `-DryRun` | report only, change nothing |

### The usual cause

Opening the project rewrites tracked files under `Config\`, and a level build
touches the map. Git then refuses to merge over them. Nothing is broken and
nothing is lost; the edits are almost always the editor's own churn.
`PULL.cmd` stashes rather than discards by default, because a stash costs
nothing and an overwrite cannot be undone.

### Reading the clipboard report

The report GO copies is now wrapped in `<# ... #>`. Pasted into the **chat** it
reads normally; pasted into **PowerShell** by mistake it is a comment and does
nothing. Before that wrapper, a misdirected paste produced a screen of
`The term '===' is not recognized` on top of whatever had actually gone wrong.

### What the editor actually writes into `Config/DefaultEngine.ini`

Diagnosed 2026-08-19. It is not corruption and it is not your doing — the
editor materialises the full default block for two settings pages the moment
it saves project settings:

| Section | Why it appears | Verdict |
|---|---|---|
| `[/Script/AndroidFileServerEditor.AndroidFileServerRuntimeSettings]` | An engine plugin enabled by default writes its whole default set, including a machine-generated `SecurityToken`. The plugin itself stays `bEnablePlugin=False`. | Harmless. Irrelevant to a Windows-only project. |
| `[/Script/WindowsTargetPlatform.WindowsTargetSettings]` | Audio device defaults, shader formats, `DefaultGraphicsRHI`. Adds `PCD3D_SM6` alongside `PCD3D_SM5`. | Wanted. SM6 is the UE5 default and the Quadro P3000 supports it. |

**The fix is to accept them into the repository once.** As long as the tracked
file lacks blocks the editor insists on writing, every single pull will fail on
the same file. Commit the editor's version and the churn stops.

One thing to watch: `RayTracingMode=Full` in that block coexists with
`r.RayTracing=False` in `[/Script/Engine.RendererSettings]`. The latter wins —
ray tracing stays off, which is correct for a Pascal card. If the two ever
disagree in a way that matters, `r.RayTracing` is the one we control
deliberately.

### Stop `git diff` opening a pager

`git diff` pipes through `less`, which is why a long diff came back as the same
screen repeated with `...skipping...`. Either:

```
git --no-pager diff Config/DefaultEngine.ini
```

or turn it off for this repository once:

```
git config core.pager cat
```
