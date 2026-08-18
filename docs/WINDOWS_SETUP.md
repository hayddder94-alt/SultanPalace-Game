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
