# دليل بناء لعبة Sultan Palace لـ Steam - خطوة بخطوة

## المتطلبات على جهازك
- Windows 10/11
- Node.js 18+ (https://nodejs.org)
- Git

## خطوات البناء (5 دقائق)

### 1. حمل المشروع
```bash
git clone https://github.com/hayddder94-alt/SultanPalace-Game.git
cd SultanPalace-Game/Steam/Build
```

### 2. ثبت Electron
```bash
npm install
# إذا فشل بسبب TLS، جرب:
# npm config set registry https://registry.npmmirror.com
# npm install
```

### 3. اختبر اللعبة كتطبيق سطح مكتب
```bash
npm start
```
ستفتح نافذة بحجم 1280x720، اللعبة تعمل كتطبيق PC حقيقي مثل RDR2، بدون متصفح

### 4. ابنِ installer Windows
```bash
npm run build
```
الناتج:
- `dist/Sultan Palace Setup 1.0.0.exe` - مثبت (≈150MB)
- `dist/win-unpacked/` - مجلد اللعبة بدون تثبيت (يمكن رفعه مباشرة ل Steam)

### 5. اختبر المثبت
شغل `Sultan Palace Setup 1.0.0.exe` وثبّت، ثم شغل اللعبة من قائمة Start

### 6. ارفع ل Steam
- افتح Steamworks Partner
- اذهب لـ Your App -> SteamPipe -> Builds
- ارفع مجلد `win-unpacked` كـ Build جديد
- عينه كـ Default

## ماذا يوجد في Build؟

- `SultanPalace/index.html` - اللعبة الأساسية AAA (41KB)
- `SultanPalace/assets/` - 5 صور واقعية PNG/JPG (11MB):
  - courtyard_aaa.jpg - فناء 8K
  - player_face.png - أحمد باشا
  - dawud_face.png - داوود
  - leyla_face.png - ليلى
  - ishak_pasha.png - إسحاق
- `SultanPalace/data/` - حوارات وأحداث JSON
- `main.js` - كود Electron لفتح نافذة
- `package.json` - إعدادات

## حجم اللعبة
- WebGL version (الحالية): 11MB assets + 41KB code + 150MB Electron runtime = ~160MB installer
- UE5 version (المستقبل): 50GB

## نصائح Steam
- غير أيقونة اللعبة: استبدل `SultanPalace/assets/courtyard_aaa.jpg` بصورة 256x256 ICO
- أضف فيديو Trailer: سجل من اللعبة ب OBS على دقة 1920x1080
- اضغط Screenshots بـ F12 داخل اللعبة (ستحفظ في مجلد Steam)

## حل مشاكل شائعة

**npm install fails TLS:**
```bash
npm config set registry https://registry.npmmirror.com
npm install
```

**three.min.js not loading offline:**
اللعبة حالياً تستخدم CDN https://unpkg.com/three@0.160.0/build/three.min.js
للـ offline، حمل الملف يدوياً وضعه في `SultanPalace/three.min.js` وغير في `index.html`:
```html
<script src="three.min.js"></script>
<!-- بدل -->
<script src="https://unpkg.com/three@0.160.0/build/three.min.js"></script>
```

**اللعبة بطيئة:**
قلل PixelRatio في `index.html`: `renderer.setPixelRatio(1)`

**اللعبة لا تبدأ:**
افتح `SultanPalace/index.html` مباشرة في Chrome - إذا عملت، المشكلة في Electron. إذا لم تعمل، المشكلة في اللعبة نفسها.

## جاهز للنشر؟
إذا نجح `npm run build` وظهر المثبت، أنت جاهز لرفع ل Steam!

بالتوفيق يا كتخدا!
