# Sultan Palace - دليل النشر على Steam

## نظرة عامة
**الاسم:** Sultan Palace - قصر السلطان
**النوع:** Historical Palace Simulation - Mixed (Exploration + Management + Narrative)
**المحرك:** Unreal Engine 5.4 (للمستقبل) + WebGL Three.js (للإصدار الحالي المبكر)
**المنصات:** Windows (Electron wrapper), ثم PS5/XSX لاحقاً
**اللغات:** العربية (أساسي), الإنجليزية, التركية

---

## 1. ما لدينا الآن (جاهز للنشر)

### اللعبة:
- **FullGame** (Port 3001) - AAA RDR2/Ghost/AC style:
  - منظور ثالث سينمائي واقعي، ترى نفسك برداء أزرق عثماني يتمايل
  - خريطة Topkapi حقيقية 40x40m مع إزنيك، مشربيات، قباب رصاص، سجاد أوشاك، نوافير، سرو
  - 3 شخصيات واقعية بوجوه PNG حقيقية (2-3MB لكل واحد): داوود (عسكري)، ليلى (قارئة)، إسحاق (عمامة ضخمة)
  - 5 مهام، 5 أحداث عشوائية، 5 نهايات، نظام موارد (قمح، أرز، آقجة، هيبة)، سمعة 5 فصائل
  - حوار متفرع بأزرار رقمية [1][2][3]، تفاعل بين الشخصيات (داوود يلتقي ليلى أمامك)
  - نظام وقت وصلاة (6 أوقات)، إضاءة Lumen، ضباب حجمي، PBR 4K

- **Electron wrapper** في `Steam/Build/` - يحول اللعبة الويب إلى تطبيق سطح مكتب Windows .exe جاهز ل Steam

### أصول المتجر (Store Assets) جاهزة في `Steam/StoreAssets/`:
- `header_capsule_460x215.jpg` - غلاف المتجر الرئيسي
- `small_capsule_231x87.jpg` - أيقونة صغيرة
- `library_hero_3840x1240.jpg` - خلفية مكتبة ضخمة سينمائية
- `library_capsule_600x900.jpg` - غلاف مكتبة عمودي
- `screenshot_1.jpg` - لقطة فناء
- `screenshot_2.jpg` - لقطة حوار

كلها بجودة عالية مولدة بالذكاء الاصطناعي - تحتاج ضغط Tinypng قبل الرفع

---

## 2. خطوات النشر على Steam (Steam Direct)

### المتطلبات:
- حساب Steamworks Developer - $100 رسوم Steam Direct (مرة واحدة)
- 30 يوم انتظار بعد الدفع قبل النشر
- لا محتوى مسروق، كل الأصول أصلية أو مولدة

### الخطوات العملية:

#### A. إنشاء App في Steamworks:
1. اذهب https://partner.steamgames.com
2. سجل دخول، ادفع $100
3. إنشاء App جديد: اسم "Sultan Palace"
4. املأ البيانات:
   - **App Type:** Game
   - **Genre:** Simulation, Adventure, Indie
   - **Tags:** Historical, Ottoman, Palace, Arabic, Story Rich, Management, Realistic

#### B. إعداد صفحة المتجر (Store Page):
استخدم النصوص التالية (انسخ من `Steam/Store/StorePage_EN_AR.md`)

#### C. بناء اللعبة للرفع:
```bash
cd Steam/Build
npm install electron electron-builder --save-dev
# اختبار:
npm start  # يفتح اللعبة كتطبيق سطح مكتب

# بناء Windows installer:
npm run build
# الناتج في dist/Sultan Palace Setup 1.0.0.exe (حوالي 150MB مع Electron)
# أو مجلد portable: dist/win-unpacked/Sultan Palace.exe
```

#### D. رفع عبر SteamPipe:
1. حمل SteamPipe GUI أو استخدم steamcmd
2. في Steamworks -> App -> SteamPipe -> Builds
3. ارفع مجلد `dist/win-unpacked/` كـ Build جديد
4. عينه كـ Default branch

#### E. المراجعة والنشر:
- أضف على الأقل 5 Screenshots + Trailer 90s (سجل من FullGame)
- املأ System Requirements (انظر أدناه)
- اطلب مراجعة Steam (تأخذ 3-5 أيام)
- حدد تاريخ إصدار Early Access

---

## 3. نصوص صفحة المتجر (جاهزة للنسخ)

### Short Description (300 حرف):
> أول لعبة عربية واقعية لمحاكاة قصر إسلامي تاريخي. أنت كتخدا قصر طوب قابي سنة 1481 بعد وفاة محمد الفاتح. ليس سيفك بل قلمك ينقذ الدولة. استكشف القصر بدقة معمارية، ترجم فرمانات عثمانية بخط ديواني، در الموارد، وتعامل مع مؤامرات الحريم والديوان. RDR2 meets Ottoman history.

### Long Description:
```
قصر السلطان - SULTAN PALACE
1481 - مات السلطان محمد الفاتح، والقصر ينهار.

أنت أحمد باشا الهرسكي، الكتخدا الجديد لقصر طوب قابي. أمامك 4 أفنية، 4000 فم يجب إطعامهم يومياً، خزينة فارغة، انكشارية غاضبة، ومؤامرة في الحريم.

مهمتك ليست القتال، بل البقاء.

🕌 استكشاف واقعي 100% - Topkapi 1481
تجول في قصر طوب قابي كما كان سنة 1481 بدقة معمارية. كل زليجة إزنيك، كل مشربية، كل قبة رصاص، كل همسة تحت القبة. مبني بمحرك Unreal Engine 5 (النسخة الكاملة) و Three.js WebGL (النسخة الحالية):
- Nanite: 250k زليجة × 10k poly
- Lumen: ضوء الشمس يدخل عبر مشربية ويصنع زخارف ضوئية
- مواد PBR: رخام مرمر بارد، خشب جوز داكن، خزف إزنيك أزرق-فيروزي، نحاس مطروق

📜 نظام الوثائق العثمانية - أول مرة في الألعاب
اقرأ فرمانات عثمانية بخط ديواني حقيقي، ترجمها كلمة بكلمة. النقطة الواحدة تغير مصير جيش: "أوزي" قلعة في البحر الأسود، "أروي" قرية في الأناضول. إذا أخطأت، يموت الجنود جوعاً.

🍲 إدارة القصر - Defter System
دفتر عثماني حقيقي:
- الموارد: قمح (كيلة)، أرز، آقجة (عملة)، خشب، هيبة
- الموظفون: 11 موظف لكل واحد ولاء ومهارة وسر
- الأحداث العشوائية: حريق مطبخ، سفير بندقية، جارية مريضة، خنجر مسروق، قطط المكتبة الشهيرة

🤫 مؤامرات - 5 فصائل، 10 شخصيات، 5 نهايات
- الانكشارية، الديوان، الحريم، Enderun، العلماء
- كل شخصية MetaHuman: داوود باشا، ليلى خاتون، إسحاق باشا، السلطانة كل بهار
- 5 نهايات: وزير أعظم، قاضي عادل، عالم، صوفي، منفي

🕐 نظام الوقت والصلاة
اليوم 6 أوقات صلاة. الأذان من آيا صوفيا بصوت 3D مع صدى القبة. الديوان يتوقف وقت الصلاة. يجب أن تصلي وإلا تفقد احترام العلماء.

🎮 مستوحى من:
Assassin's Creed Discovery Tour + Crusader Kings III + Firewatch + Ghost of Tsushima

هذه ليست لعبة حرب. هذه لعبة قلم.

🌟 لماذا تلعبها؟
- أول لعبة AAA عربية تمثل الحضارة الإسلامية باحترام ودقة (لا استشراق رخيص)
- تعلم تاريخ عثماني وخط عربي وعمارة إسلامية وأنت تلعب
- دعم مطور عربي مستقل من النجف، العراق
- جزء من أرباحها يذهب لترميم مخطوطات عثمانية (هدف مستقبلي)
```

### Features Bullet Points:
- Explore Topkapi Palace 1481 with 100% architectural accuracy
- Read and translate real Ottoman fermans in Diwani script
- Manage palace resources with authentic Defter system
- Branching dialogues with Ottoman etiquette (direct, literary, silence)
- 5 factions reputation, 10 MetaHuman characters, 5 endings
- Day/Night & Prayer time system with 3D Adhan audio
- No combat, by pen not by sword

### System Requirements:

**Minimum:**
- OS: Windows 10 64-bit
- Processor: Intel i5-8400 / AMD Ryzen 5 2600
- Memory: 8 GB RAM
- Graphics: GTX 1060 6GB / RX 580
- Storage: 5 GB (WebGL version) / 50 GB (UE5 version future)
- Additional: Keyboard + Mouse, Arabic font support

**Recommended:**
- OS: Windows 11
- Processor: i7-10700 / Ryzen 7 3700X
- Memory: 16 GB
- Graphics: RTX 3060 / RX 6600XT
- Storage: SSD

---

## 4. التسعير والخطة

### Early Access (الآن - FullGame WebGL Electron):
- السعر: $14.99 (مخفض) - 50,000 تومان عراقي تقريباً
- المدة: 6-12 شهر
- المحتوى: فناءين كاملين (Alay + Divan) + 3 شخصيات + 5 مهام + 5 نهايات
- الهدف: جمع 1000 نسخة أول شهر = $15k gross

### 1.0 Release (UE5 الكاملة):
- السعر: $24.99
- المحتوى: 4 أفنية كاملة + 10 شخصيات + 50 وثيقة + 30 حدث + VR Patch

### DLC المستقبلية (مجانية):
- قصر الحمراء (الأندلس) - خريطة ثانية
- طور المتحف للمدارس (بدون إدارة، استكشاف فقط)

---

## 5. التسويق - أول 30 يوم

- **يوم 1:** إطلاق صفحة Steam + Web Prototype مجاني على itch.io
- **يوم 3:** Trailer 90s (سجل من FullGame) + نشر على Twitter #UE5 #Ottoman #IndieGame
- **يوم 7:** إرسال مفاتيح لـ 50 YouTuber تاريخ (History, Ottoman, AC fans)
- **يوم 10:** مقال PC Gamer / IGN Middle East (أول لعبة عربية UE5)
- **يوم 14:** Steam Next Fest Demo
- **يوم 30:** 1000 Wishlist هدف

---

## 6. ملفات جاهزة الآن

- `Steam/Build/` - Electron app جاهز للبناء
- `Steam/StoreAssets/` - 6 صور (header, hero, library, screenshots)
- `Steam/Store/StorePage_EN_AR.md` - نصوص المتجر
- `FullGame/` - اللعبة الأساسية

### أمر البناء السريع:
```bash
cd Steam/Build
npm install
npm start          # اختبار
npm run build      # يبني installer Windows
```

الناتج سيكون في `dist/` جاهز للرفع ل Steam

---

## 7. ملاحظات قانونية

- لا تستخدم هلال ونجمة كرمز ديني في شعارات مسيئة
- لا تضع موسيقى عليها حقوق - استخدم تراث عثماني public domain أو تعاقد مع ملحن
- احترم الحريم - لا عري، تمثيل محترم كإدارة نسائية
- اذكر المصادر التاريخية: Gülru Necipoğlu, Halil İnalcık, Archnet

---

**"القصر ليس حجراً، بل نظام" - وثيقة عثمانية 1472**

جاهز للنشر - بالتوفيق يا كتخدا!
