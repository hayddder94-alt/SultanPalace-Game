// The Betrayed Will / وصية الغدر — East Wing layout previewer.
//
// Reads data/east_wing.json, the same file tools/ue_python/build_east_wing.py
// feeds to Unreal. This is an architectural walkthrough, not the game: no Lumen,
// no materials, no characters. Its job is to let anyone check scale, sightlines
// and circulation from a browser on any device.
//
// Unreal is Z-up and centimetre based. Three.js is Y-up and we work in metres,
// so every position goes through ueToThree().

import * as THREE from "three";

const CM = 0.01;                    // Unreal cm -> metres
const EYE = 1.7;                    // standing eye height, m
const EYE_CROUCH = 1.05;
const WALK = 4.2;                   // m/s, matches the 420 uu/s pawn
const RUN = 6.2;
const GRAVITY = 18.0;
const JUMP = 5.2;
const RADIUS = 0.38;                // capsule radius, matches the pawn

// Third-person rig, same numbers as ATBWPlayerCharacter's spring arm:
// TargetArmLength 280, SocketOffset (0, 55, 62), ProbeSize 20, collision on.
const ARM_DEFAULT = 2.80;
const ARM_SPRINT = 3.40;
const ARM_CROUCH = 2.30;
const SOCKET_RIGHT = 0.55;
const SOCKET_UP = 0.62;
const PROBE = 0.20;
const CHAR_HEIGHT = 1.76;

// Unreal (x east, y north, z up) -> Three (x east, y up, z south)
function ueToThree(x, y, z) {
  return new THREE.Vector3(x * CM, z * CM, -y * CM);
}

const canvas = document.getElementById("view");
const renderer = new THREE.WebGLRenderer({ canvas, antialias: true });
renderer.setPixelRatio(Math.min(devicePixelRatio, 2));
renderer.shadowMap.enabled = true;
renderer.shadowMap.type = THREE.PCFSoftShadowMap;
renderer.toneMapping = THREE.ACESFilmicToneMapping;
renderer.toneMappingExposure = 1.05;

const scene = new THREE.Scene();
scene.background = new THREE.Color(0x0e0d0b);
scene.fog = new THREE.FogExp2(0x1b1710, 0.006);

const camera = new THREE.PerspectiveCamera(72, innerWidth / innerHeight, 0.1, 2000);
const topCamera = new THREE.OrthographicCamera(-1, 1, 1, -1, 0.1, 3000);
let usingTop = false;

// ---------------------------------------------------------------- materials
// Colours stand in for the material families the Unreal build resolves to real
// Megascans surfaces. Same names, so a mental map carries over.
const MATERIALS = {
  wall:   new THREE.MeshStandardMaterial({ color: 0x8a7458, roughness: 0.95, metalness: 0.0 }),
  floor:  new THREE.MeshStandardMaterial({ color: 0x6f6252, roughness: 0.88 }),
  hero:   new THREE.MeshStandardMaterial({ color: 0xb99a63, roughness: 0.62, metalness: 0.12 }),
  ground: new THREE.MeshStandardMaterial({ color: 0x7a6a4e, roughness: 1.0 }),
  water:  new THREE.MeshStandardMaterial({ color: 0x2c4a52, roughness: 0.15, metalness: 0.5,
                                           transparent: true, opacity: 0.85 }),
};

// ---------------------------------------------------------------- lighting
// A blockout viewer has one duty: never show the user a black screen. Interiors
// here are sealed rooms with ceilings, so sunlight alone leaves them pitch dark.
// The rig is therefore deliberately over-lit compared to the Unreal scene:
// strong sky ambient, a fill light, and a camera headlamp that is on by default.

let headlamp = null;
let sunLight = null;

function buildLighting(layout) {
  const sunCfg = layout.sun || { pitch: -14, yaw: 125, intensity: 6, color: [1, 0.85, 0.7] };
  sunLight = new THREE.DirectionalLight(
    new THREE.Color(sunCfg.color[0], sunCfg.color[1], sunCfg.color[2]), 3.4);

  const pitch = THREE.MathUtils.degToRad(sunCfg.pitch);
  const yaw = THREE.MathUtils.degToRad(sunCfg.yaw);
  const dist = 120;
  sunLight.position.set(
    Math.cos(pitch) * Math.cos(yaw) * -dist,
    Math.sin(-pitch) * dist,
    Math.cos(pitch) * Math.sin(yaw) * dist
  );
  sunLight.castShadow = true;
  sunLight.shadow.mapSize.set(2048, 2048);
  const s = 45;
  sunLight.shadow.camera.left = -s; sunLight.shadow.camera.right = s;
  sunLight.shadow.camera.top = s; sunLight.shadow.camera.bottom = -s;
  sunLight.shadow.camera.far = 400;
  sunLight.shadow.bias = -0.0006;
  sunLight.target.position.set(20, 0, -27);
  scene.add(sunLight, sunLight.target);

  // sky/ground ambient - the main reason interiors stay readable
  scene.add(new THREE.HemisphereLight(0xbcd3f0, 0x6b5a42, 2.4));
  scene.add(new THREE.AmbientLight(0xffffff, 0.55));

  // The practicals from the layout, so the preview shows the same lit rooms the
  // engine does. Without these the preview relied on a hemisphere light and a
  // headlamp - hacks that made it look fine while the real level rendered
  // black, which is exactly the disagreement this preview exists to prevent.
  // No shadows here: 21 shadow-casting point lights would crawl in WebGL and
  // the preview is for layout and light PLACEMENT, not shadow quality.
  for (const spec of layout.lights || []) {
    const [x, y, z] = spec.location;
    const c = spec.color || [255, 200, 140];
    const lamp = new THREE.PointLight(
      new THREE.Color(c[0] / 255, c[1] / 255, c[2] / 255),
      (spec.lumens || 2600) / 90,
      (spec.attenuation_radius || 900) / 100,
      1.6);
    // layout is cm with Y north / Z up; the preview is metres with Y up.
    lamp.position.set(x / 100, z / 100, -y / 100);
    scene.add(lamp);
  }

  // warm fill over the hall so the hero volume reads first
  const fill = new THREE.PointLight(0xffd9a0, 60, 90, 1.6);
  fill.position.set(20, 6, -38);
  scene.add(fill);

  // headlamp: rides the camera, guarantees a lit surface wherever you look
  headlamp = new THREE.PointLight(0xfff2dd, 26, 34, 1.4);
  headlamp.position.set(0, 0, 0);
  camera.add(headlamp);
  scene.add(camera);
}

// ---------------------------------------------------------------- geometry
const colliders = [];      // axis-aligned boxes in Three space, for walking
const ceilingMeshes = [];  // toggled with H
let boxCount = 0;

function buildGeometry(layout) {
  const unit = new THREE.BoxGeometry(1, 1, 1);
  const byKind = new Map();

  for (const b of layout.boxes) {
    const [cx, cy, cz] = b.center;
    const [sx, sy, sz] = b.size;
    // Ceilings get their own batch so they can be hidden - standard practice in
    // architectural review, and the only way sunlight reaches a sealed room.
    const key = b.name.endsWith("_Ceiling") ? "ceiling:" + b.kind : b.kind;
    if (!byKind.has(key)) byKind.set(key, []);
    byKind.get(key).push({ cx, cy, cz, sx, sy, sz });
  }

  for (const [key, items] of byKind) {
    const isCeiling = key.startsWith("ceiling:");
    const kind = isCeiling ? key.slice(8) : key;
    const mat = MATERIALS[kind] || MATERIALS.wall;
    const mesh = new THREE.InstancedMesh(unit, mat, items.length);
    if (isCeiling) {
      mesh.visible = false;          // hidden by default: light in, layout readable
      ceilingMeshes.push(mesh);
    }
    mesh.castShadow = true;
    mesh.receiveShadow = true;
    const m = new THREE.Matrix4();

    items.forEach((it, i) => {
      const p = ueToThree(it.cx, it.cy, it.cz);
      m.compose(
        p,
        new THREE.Quaternion(),
        new THREE.Vector3(it.sx * CM, it.sz * CM, it.sy * CM)   // note: UE Y -> Three Z
      );
      mesh.setMatrixAt(i, m);

      // collider, half extents in Three space
      if (kind !== "water") {
        colliders.push({
          min: new THREE.Vector3(p.x - it.sx * CM / 2, p.y - it.sz * CM / 2, p.z - it.sy * CM / 2),
          max: new THREE.Vector3(p.x + it.sx * CM / 2, p.y + it.sz * CM / 2, p.z + it.sy * CM / 2),
        });
      }
      boxCount++;
    });

    mesh.instanceMatrix.needsUpdate = true;
    scene.add(mesh);
  }

  // ground plane well under the wing so the eye has a horizon
  const ground = new THREE.Mesh(
    new THREE.PlaneGeometry(600, 600),
    new THREE.MeshStandardMaterial({ color: 0x241d15, roughness: 1 })
  );
  ground.rotation.x = -Math.PI / 2;
  ground.position.y = -0.25;
  ground.receiveShadow = true;
  scene.add(ground);
}

// ---------------------------------------------------------------- story beats
// The same interactables the Unreal build spawns, shown as glowing markers so a
// layout review also answers "can the player find this?".

const beats = [];

function buildInteractables(layout) {
  const items = layout.interactables || [];
  const geo = new THREE.BoxGeometry(1, 1, 1);
  const mat = new THREE.MeshStandardMaterial({
    color: 0xffcf7a, emissive: 0xd8a24a, emissiveIntensity: 0.9, roughness: 0.4,
  });

  for (const it of items) {
    const p = ueToThree(it.location[0], it.location[1], it.location[2]);
    const mesh = new THREE.Mesh(geo, mat);
    mesh.position.copy(p);
    mesh.scale.set(it.size[0] * CM, it.size[2] * CM, it.size[1] * CM);
    mesh.castShadow = true;
    scene.add(mesh);

    // a soft light so the beat is findable in a dark room, exactly the question
    // this preview exists to answer
    const glow = new THREE.PointLight(0xffc978, 6, 9, 1.6);
    glow.position.copy(p).add(new THREE.Vector3(0, 0.6, 0));
    scene.add(glow);

    beats.push({ ...it, pos: p });
  }
  return items.length;
}

function nearestBeat() {
  let best = null;
  let bestD = 6.0;
  for (const b of beats) {
    const d = Math.hypot(b.pos.x - player.pos.x, b.pos.z - player.pos.z);
    if (d < bestD) { bestD = d; best = b; }
  }
  return best;
}

// ---------------------------------------------------------------- cast
// The staged characters, drawn as proxies with the same silhouette rules the
// Unreal ATBWStoryCharacter uses: body block plus head, poses staged not animated.

const cast = [];

function makeFigure(colour) {
  const g = new THREE.Group();
  const skin = new THREE.MeshStandardMaterial({ color: colour, roughness: 0.75 });
  const body = new THREE.Mesh(new THREE.BoxGeometry(0.40, 1.45, 0.32), skin);
  body.castShadow = true;
  const head = new THREE.Mesh(new THREE.SphereGeometry(0.115, 16, 12), skin);
  head.castShadow = true;
  g.add(body, head);
  g.userData.body = body;
  g.userData.head = head;
  return g;
}

const CAST_COLOUR = {
  Orin: 0xbfae86, Raynor: 0x9c6f4a, Evan: 0x7f93ae, Nofan: 0x8d6b8f,
  Leila: 0xb08fa2, Darius: 0x6f7b6a, Malik: 0x9a9060, Soren: 0x86a3a8,
};

function buildCast(layout) {
  const items = layout.characters || [];
  for (const c of items) {
    const p = ueToThree(c.location[0], c.location[1], c.location[2]);
    const fig = makeFigure(CAST_COLOUR[c.name] || 0x8a8073);
    const body = fig.userData.body;
    const head = fig.userData.head;

    if (c.pose === "seated") {
      body.scale.set(1, 0.66, 1);
      body.position.y = -0.16;
      head.position.y = 0.36;
    } else if (c.pose === "lying") {
      body.rotation.z = Math.PI / 2;
      body.position.y = -0.62;
      head.position.set(0.62, -0.62, 0);
    } else {
      body.position.y = -0.16;
      head.position.y = 0.68;
    }

    fig.position.copy(p);
    fig.rotation.y = THREE.MathUtils.degToRad(-(c.yaw || 0));
    scene.add(fig);

    // name plate, always facing the camera
    const label = makeLabel(c.name);
    label.position.copy(p).add(new THREE.Vector3(0, c.pose === "lying" ? 0.5 : 1.05, 0));
    scene.add(label);

    cast.push({ ...c, pos: p, label });
  }
  return items.length;
}

function makeLabel(text) {
  const cv = document.createElement("canvas");
  cv.width = 256; cv.height = 64;
  const ctx = cv.getContext("2d");
  ctx.fillStyle = "rgba(12,11,9,0.72)";
  ctx.fillRect(0, 0, 256, 64);
  ctx.font = "28px system-ui, sans-serif";
  ctx.fillStyle = "#e6cfa0";
  ctx.textAlign = "center";
  ctx.textBaseline = "middle";
  ctx.fillText(text, 128, 34);
  const tex = new THREE.CanvasTexture(cv);
  tex.colorSpace = THREE.SRGBColorSpace;
  const sprite = new THREE.Sprite(new THREE.SpriteMaterial({ map: tex, transparent: true, depthTest: true }));
  sprite.scale.set(1.5, 0.375, 1);
  return sprite;
}

function nearestCast() {
  let best = null;
  let bestD = 5.0;
  for (const c of cast) {
    const d = Math.hypot(c.pos.x - player.pos.x, c.pos.z - player.pos.z);
    if (d < bestD) { bestD = d; best = c; }
  }
  return best;
}

// ---------------------------------------------------------------- player
const player = {
  pos: new THREE.Vector3(0, EYE, 0),
  vel: new THREE.Vector3(),
  yaw: 0,
  pitch: 0,
  grounded: true,
  crouch: false,
};

const keys = new Set();

/**
 * Input is deliberately independent of Pointer Lock.
 *
 * The preview runs inside an iframe, and an iframe only gets Pointer Lock if the
 * embedding page grants allow="pointer-lock". When it is denied the request fails
 * silently, the pointerlockchange event never fires, and a control scheme built on
 * it is simply dead - which is exactly what happened. So: keys work whenever the
 * document has focus, looking works by dragging, and there are on-screen buttons
 * for touch. Pointer Lock, if granted, is a bonus.
 */
const gate = document.getElementById("gate");
let locked = false;
let dragging = false;

function beginPlaying() {
  gate.style.display = "none";
  canvas.focus();
  // Optional: nicer if allowed, harmless if not.
  if (canvas.requestPointerLock) {
    const req = canvas.requestPointerLock();
    if (req && typeof req.catch === "function") req.catch(() => {});
  }
}

canvas.setAttribute("tabindex", "0");
gate.addEventListener("click", beginPlaying);
canvas.addEventListener("click", () => { if (gate.style.display === "none") canvas.focus(); });

document.addEventListener("pointerlockchange", () => {
  locked = document.pointerLockElement === canvas;
});

// Drag to look when we do not have pointer lock.
canvas.addEventListener("mousedown", (e) => { dragging = true; e.preventDefault(); });
addEventListener("mouseup", () => { dragging = false; });
addEventListener("mouseleave", () => { dragging = false; });

function applyLook(dx, dy) {
  player.yaw -= dx * 0.0022;
  player.pitch -= dy * 0.0019;
  const up = THREE.MathUtils.degToRad(70);
  const down = THREE.MathUtils.degToRad(55);
  player.pitch = Math.max(-down, Math.min(up, player.pitch));
}

addEventListener("mousemove", (e) => {
  if (locked || dragging) applyLook(e.movementX || 0, e.movementY || 0);
});

// Touch: left half drags the camera, the on-screen pad drives movement.
let lastTouch = null;
canvas.addEventListener("touchstart", (e) => {
  lastTouch = e.touches[0];
  if (gate.style.display !== "none") beginPlaying();
}, { passive: true });
canvas.addEventListener("touchmove", (e) => {
  const t = e.touches[0];
  if (lastTouch) applyLook((t.clientX - lastTouch.clientX) * 1.6, (t.clientY - lastTouch.clientY) * 1.6);
  lastTouch = t;
}, { passive: true });
canvas.addEventListener("touchend", () => { lastTouch = null; }, { passive: true });

// Keys are read from e.code, so an Arabic or French keyboard layout still drives
// WASD by physical position. Arrow keys are accepted too.
const KEY_ALIAS = {
  ArrowUp: "KeyW", ArrowDown: "KeyS", ArrowLeft: "KeyA", ArrowRight: "KeyD",
};

function keyDown(code) {
  keys.add(KEY_ALIAS[code] || code);
  if (code === "KeyF") toggleTop();
  if (code === "KeyH") toggleCeilings();
  if (code === "KeyL") toggleHeadlamp();
  if (code === "BracketRight") setExposure(renderer.toneMappingExposure + 0.15);
  if (code === "BracketLeft") setExposure(renderer.toneMappingExposure - 0.15);
}

function keyUp(code) {
  keys.delete(KEY_ALIAS[code] || code);
}

addEventListener("keydown", (e) => {
  if (["Space", "ArrowUp", "ArrowDown", "ArrowLeft", "ArrowRight"].includes(e.code)) {
    e.preventDefault();
  }
  if (gate.style.display !== "none") beginPlaying();
  keyDown(e.code);
});
addEventListener("keyup", (e) => keyUp(e.code));
// If the iframe loses focus mid-stride, stop moving instead of running forever.
addEventListener("blur", () => keys.clear());

let ceilingsVisible = false;
function toggleCeilings() {
  ceilingsVisible = !ceilingsVisible;
  ceilingMeshes.forEach((m) => { m.visible = ceilingsVisible; });
  const el = document.getElementById("hCeil");
  if (el) el.textContent = ceilingsVisible ? "ظاهرة" : "مخفية";
}

let headlampOn = true;
function toggleHeadlamp() {
  headlampOn = !headlampOn;
  if (headlamp) headlamp.intensity = headlampOn ? 26 : 0;
  const el = document.getElementById("hLamp");
  if (el) el.textContent = headlampOn ? "مشغّل" : "مطفأ";
}

function setExposure(v) {
  renderer.toneMappingExposure = Math.max(0.3, Math.min(3.0, v));
  const el = document.getElementById("hExp");
  if (el) el.textContent = renderer.toneMappingExposure.toFixed(2);
}

// On-screen pad: works with mouse, finger, and pen. Held, not tapped.
function bindPad() {
  const pad = document.getElementById("pad");
  if (!pad) return;
  pad.querySelectorAll("[data-key]").forEach((btn) => {
    const code = btn.getAttribute("data-key");
    const press = (e) => { e.preventDefault(); keyDown(code); btn.classList.add("on"); };
    const release = (e) => { e.preventDefault(); keyUp(code); btn.classList.remove("on"); };
    btn.addEventListener("pointerdown", press);
    btn.addEventListener("pointerup", release);
    btn.addEventListener("pointerleave", release);
    btn.addEventListener("pointercancel", release);
  });
  pad.querySelectorAll("[data-toggle]").forEach((btn) => {
    btn.addEventListener("click", (e) => {
      e.preventDefault();
      const which = btn.getAttribute("data-toggle");
      if (which === "top") toggleTop();
      if (which === "ceil") toggleCeilings();
      if (which === "lamp") toggleHeadlamp();
    });
  });
}

// Axis-separated resolution against the box set. Crude on purpose: this is a
// layout walkthrough, and crude is predictable.
function collide(next, height) {
  const feet = next.y - height;
  const head = next.y + 0.15;
  for (const c of colliders) {
    if (next.x + RADIUS < c.min.x || next.x - RADIUS > c.max.x) continue;
    if (next.z + RADIUS < c.min.z || next.z - RADIUS > c.max.z) continue;
    if (head < c.min.y || feet > c.max.y) continue;
    return true;
  }
  return false;
}

function groundHeightAt(x, z) {
  let best = 0;
  for (const c of colliders) {
    if (x < c.min.x - RADIUS || x > c.max.x + RADIUS) continue;
    if (z < c.min.z - RADIUS || z > c.max.z + RADIUS) continue;
    if (c.max.y <= player.pos.y - (player.crouch ? EYE_CROUCH : EYE) + 0.45) {
      best = Math.max(best, c.max.y);
    }
  }
  return best;
}

let avatar = null;

function buildAvatar() {
  avatar = makeFigure(0x6f8199);
  avatar.userData.body.position.y = -0.16;
  avatar.userData.head.position.y = 0.68;
  scene.add(avatar);
}

/**
 * Third-person boom. The game is third person, so a first-person preview was
 * answering the wrong question: it could not show how much of a room the camera
 * eats, or whether the player's own body blocks a doorway. Arm length, socket
 * offset, probe size and the pitch clamp are all the pawn's own values.
 */
function updateCamera(dt) {
  const crouch = keys.has("KeyC");
  const sprint = (keys.has("ShiftLeft") || keys.has("ShiftRight")) && !crouch;
  const targetArm = crouch ? ARM_CROUCH : (sprint ? ARM_SPRINT : ARM_DEFAULT);
  updateCamera.arm = THREE.MathUtils.lerp(updateCamera.arm ?? targetArm, targetArm, 1 - Math.pow(0.001, dt));

  const pivot = new THREE.Vector3(
    player.pos.x,
    player.pos.y - (crouch ? 0.25 : 0.0),
    player.pos.z
  );

  const dir = new THREE.Vector3(
    -Math.sin(player.yaw) * Math.cos(player.pitch),
    Math.sin(player.pitch),
    -Math.cos(player.yaw) * Math.cos(player.pitch)
  );
  const right = new THREE.Vector3(Math.cos(player.yaw), 0, -Math.sin(player.yaw));

  const socket = pivot.clone()
    .add(right.clone().multiplyScalar(SOCKET_RIGHT))
    .add(new THREE.Vector3(0, SOCKET_UP, 0));

  // Collision-aware boom: march back until the probe sphere would enter geometry.
  let arm = updateCamera.arm;
  const steps = 14;
  for (let i = 1; i <= steps; i++) {
    const test = socket.clone().add(dir.clone().multiplyScalar(-arm * (i / steps)));
    if (probeHits(test, PROBE)) {
      arm = arm * ((i - 1) / steps);
      break;
    }
  }
  arm = Math.max(0.35, arm);

  camera.position.copy(socket).add(dir.clone().multiplyScalar(-arm));
  camera.lookAt(socket.clone().add(dir.clone().multiplyScalar(2.0)));

  if (avatar) {
    avatar.position.set(player.pos.x, player.pos.y - CHAR_HEIGHT + 0.88, player.pos.z);
    avatar.userData.body.scale.y = crouch ? 0.72 : 1.0;
    // The body turns toward movement, exactly like bOrientRotationToMovement.
    if (player.moveDir && player.moveDir.lengthSq() > 0.01) {
      const want = Math.atan2(-player.moveDir.x, -player.moveDir.z);
      avatar.rotation.y = THREE.MathUtils.lerp(avatar.rotation.y, want, 1 - Math.pow(0.0001, dt));
    }
  }
}

function probeHits(point, r) {
  for (const c of colliders) {
    if (point.x + r < c.min.x || point.x - r > c.max.x) continue;
    if (point.y + r < c.min.y || point.y - r > c.max.y) continue;
    if (point.z + r < c.min.z || point.z - r > c.max.z) continue;
    return true;
  }
  return false;
}

function updatePlayer(dt) {
  player.crouch = keys.has("KeyC");
  const height = player.crouch ? EYE_CROUCH : EYE;
  const speed = (keys.has("ShiftLeft") || keys.has("ShiftRight")) && !player.crouch
    ? RUN : (player.crouch ? WALK * 0.4 : WALK);

  const fwd = new THREE.Vector3(-Math.sin(player.yaw), 0, -Math.cos(player.yaw));
  const right = new THREE.Vector3(Math.cos(player.yaw), 0, -Math.sin(player.yaw));

  const wish = new THREE.Vector3();
  if (keys.has("KeyW")) wish.add(fwd);
  if (keys.has("KeyS")) wish.sub(fwd);
  if (keys.has("KeyD")) wish.add(right);
  if (keys.has("KeyA")) wish.sub(right);
  player.moveDir = wish.clone();
  if (wish.lengthSq() > 0) wish.normalize().multiplyScalar(speed);

  if (keys.has("Space") && player.grounded) {
    player.vel.y = JUMP;
    player.grounded = false;
  }
  player.vel.y -= GRAVITY * dt;

  const step = new THREE.Vector3(wish.x * dt, player.vel.y * dt, wish.z * dt);

  const tryX = player.pos.clone(); tryX.x += step.x;
  if (!collide(tryX, height)) player.pos.x = tryX.x;

  const tryZ = player.pos.clone(); tryZ.z += step.z;
  if (!collide(tryZ, height)) player.pos.z = tryZ.z;

  player.pos.y += step.y;
  const floor = groundHeightAt(player.pos.x, player.pos.z) + height;
  if (player.pos.y <= floor) {
    player.pos.y = floor;
    player.vel.y = 0;
    player.grounded = true;
  }

}

// ---------------------------------------------------------------- rooms
let roomsOfInterest = {};
const ROOM_LABELS = {
  R2_AudienceHall: "قاعة الاستقبال",
  R4_FamilyCorridor: "ممر العائلة",
  R5_EvanChamber: "غرفة إيفان",
  R7_Study: "غرفة الكاتب",
  R1_Dock: "بوابة القناة",
};

function teleport(key) {
  const p = roomsOfInterest[key];
  if (!p) return;
  const v = ueToThree(p[0], p[1], p[2]);
  player.pos.set(v.x, v.y, v.z);
  player.vel.set(0, 0, 0);
}

function currentRoom() {
  let best = "—";
  let bestD = 14;
  for (const [key, p] of Object.entries(roomsOfInterest)) {
    const v = ueToThree(p[0], p[1], p[2]);
    const d = Math.hypot(v.x - player.pos.x, v.z - player.pos.z);
    if (d < bestD) { bestD = d; best = ROOM_LABELS[key] || key; }
  }
  return best;
}

function toggleTop() {
  usingTop = !usingTop;
  document.getElementById("hMode").textContent = usingTop ? "منظر علوي" : "مشي";
}

// ---------------------------------------------------------------- loop
const hPos = document.getElementById("hPos");
const hRoom = document.getElementById("hRoom");
const hFps = document.getElementById("hFps");
let last = performance.now();
let frames = 0, fpsClock = 0;

function resize() {
  const w = innerWidth, h = innerHeight;
  renderer.setSize(w, h, false);
  camera.aspect = w / h;
  camera.updateProjectionMatrix();
  const half = 34;
  topCamera.left = -half * (w / h); topCamera.right = half * (w / h);
  topCamera.top = half; topCamera.bottom = -half;
  topCamera.updateProjectionMatrix();
}
addEventListener("resize", resize);

function frame(now) {
  const dt = Math.min((now - last) / 1000, 0.05);
  last = now;

  updatePlayer(dt);
  updateCamera(dt);

  if (usingTop) {
    topCamera.position.set(20, 90, -27);
    topCamera.lookAt(20, 0, -27);
    topCamera.up.set(0, 0, -1);
    renderer.render(scene, topCamera);
  } else {
    renderer.render(scene, camera);
  }

  frames++; fpsClock += dt;
  if (fpsClock >= 0.5) {
    hFps.textContent = Math.round(frames / fpsClock);
    frames = 0; fpsClock = 0;
    hPos.textContent = `${player.pos.x.toFixed(1)}, ${(-player.pos.z).toFixed(1)} م`;
    hRoom.textContent = currentRoom();
    const b = nearestBeat();
    const person = nearestCast();
    document.getElementById("hBeat").textContent =
      person ? `◆ ${person.name}` : (b ? `[E] ${b.prompt}` : "—");
  }
  requestAnimationFrame(frame);
}

// ---------------------------------------------------------------- boot
fetch("./data/east_wing.json")
  .then((r) => { if (!r.ok) throw new Error(`HTTP ${r.status}`); return r.json(); })
  .then((layout) => {
    buildLighting(layout);
    buildGeometry(layout);
    const beatCount = buildInteractables(layout);
    const castCount = buildCast(layout);
    buildAvatar();
    roomsOfInterest = layout.rooms_of_interest || {};

    const holder = document.getElementById("roomButtons");
    for (const key of Object.keys(roomsOfInterest)) {
      const b = document.createElement("button");
      b.textContent = ROOM_LABELS[key] || key;
      b.onclick = () => { teleport(key); beginPlaying(); };
      holder.appendChild(b);
    }

    const ps = layout.player_start.location;
    const v = ueToThree(ps[0], ps[1], ps[2] + 50);
    player.pos.set(v.x, EYE, v.z);
    player.yaw = THREE.MathUtils.degToRad(-layout.player_start.yaw);

    bindPad();
    document.getElementById("hBoxes").textContent = boxCount;
    document.getElementById("hBeats").textContent = beatCount;
    document.getElementById("hCast").textContent = castCount;

    for (const b of beats) {
      const btn = document.createElement("button");
      btn.textContent = "◆ " + (b.prompt || b.id);
      btn.title = b.examine || "";
      btn.onclick = () => {
        player.pos.set(b.pos.x, EYE, b.pos.z + 2.2);
        player.vel.set(0, 0, 0);
        beginPlaying();
      };
      document.getElementById("beatButtons").appendChild(btn);
    }
    resize();
    requestAnimationFrame(frame);
  })
  .catch((err) => {
    document.getElementById("gate").innerHTML =
      `<div><h2>تعذّر تحميل التخطيط</h2><p>${err}</p>
       <p>شغّل: <code>python tools/layout/east_wing.py</code></p></div>`;
  });
