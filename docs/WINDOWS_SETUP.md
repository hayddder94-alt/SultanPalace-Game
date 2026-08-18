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
powershell -ExecutionPolicy Bypass -File .\tools\phase2_build_and_check.ps1
```

هذا كل شيء. حجم المستودع أقل من 5 ميغابايت.

- إن ظهر `git is not recognized` → ثبّت Git من https://git-scm.com/download/win ثم أعد فتح PowerShell.
- إن ظهر `running scripts is disabled on this system` → استخدم صيغة `powershell -ExecutionPolicy Bypass -File` كما في الأعلى (هي مكتوبة هكذا لهذا السبب).
- إن لم يجد السكربت المحرك → مرّر مساره:
  ```powershell
  powershell -ExecutionPolicy Bypass -File .\tools\phase2_build_and_check.ps1 -EngineRoot "C:\Program Files\Epic Games\UE_5.8"
  ```

### أو بسكربت الإقلاع (يستنسخ أو يحدّث، ثم يبني)

```powershell
curl.exe -L -o $env:TEMP\windows_bootstrap.ps1 https://raw.githubusercontent.com/hayddder94-alt/SultanPalace-Game/arena/019ffc4c-sultanpalace-game/tools/windows_bootstrap.ps1
powershell -ExecutionPolicy Bypass -File $env:TEMP\windows_bootstrap.ps1 -Build
```

---

## في كل مرة لاحقًا

```powershell
cd C:\Dev\SultanPalace-Game
git pull origin arena/019ffc4c-sultanpalace-game
powershell -ExecutionPolicy Bypass -File .\tools\phase2_build_and_check.ps1
```

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
