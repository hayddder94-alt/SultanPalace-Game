// Headless self-test for the East Wing preview.
//
// A browser cannot be installed in the build sandbox, so this exercises
// everything except the pixels: the vendored three.js loads, the shared layout
// parses, every instance matrix is finite, the spawn point is not buried inside
// geometry, and the lighting rig cannot leave a surface at zero.
//
//   node web/preview/selftest.mjs

import * as THREE from "./vendor/three.module.js";
import { readFileSync } from "node:fs";
import { fileURLToPath } from "node:url";
import { dirname, join } from "node:path";

const here = dirname(fileURLToPath(import.meta.url));
const layout = JSON.parse(readFileSync(join(here, "data", "east_wing.json"), "utf8"));

const CM = 0.01;
const EYE = 1.7;
const RADIUS = 0.38;

let failures = 0;
function check(name, ok, detail = "") {
  console.log(`${ok ? "PASS" : "FAIL"}  ${name}${detail ? "  — " + detail : ""}`);
  if (!ok) failures++;
}

// ---------------------------------------------------------------- library
check("three.js vendored and importable", typeof THREE.REVISION === "string", `r${THREE.REVISION}`);
check("WebGL-free classes available", !!(THREE.InstancedMesh && THREE.HemisphereLight));

// ---------------------------------------------------------------- layout
check("layout parses", Array.isArray(layout.boxes) && layout.boxes.length > 0, `${layout.boxes.length} boxes`);
check("units are centimetres", layout.units === "cm");
check("footprint matches the spec", JSON.stringify(layout.footprint_m) === "[40,55]");

const kinds = new Set(layout.boxes.map((b) => b.kind));
check("every kind has a material mapping",
  [...kinds].every((k) => ["wall", "floor", "hero", "ground", "water"].includes(k)),
  [...kinds].join(", "));

// ---------------------------------------------------------------- geometry
function ueToThree(x, y, z) {
  return new THREE.Vector3(x * CM, z * CM, -y * CM);
}

const colliders = [];
let bad = 0;
const m = new THREE.Matrix4();
for (const b of layout.boxes) {
  const [cx, cy, cz] = b.center;
  const [sx, sy, sz] = b.size;
  const p = ueToThree(cx, cy, cz);
  m.compose(p, new THREE.Quaternion(), new THREE.Vector3(sx * CM, sz * CM, sy * CM));
  if (m.elements.some((v) => !Number.isFinite(v))) bad++;
  if (sx <= 0 || sy <= 0 || sz <= 0) bad++;
  if (b.kind !== "water") {
    colliders.push({
      min: new THREE.Vector3(p.x - sx * CM / 2, p.y - sz * CM / 2, p.z - sy * CM / 2),
      max: new THREE.Vector3(p.x + sx * CM / 2, p.y + sz * CM / 2, p.z + sy * CM / 2),
      name: b.name,
    });
  }
}
check("all instance matrices finite and sized", bad === 0, `${bad} bad`);

// ---------------------------------------------------------------- spawn
const ps = layout.player_start.location;
const spawn = ueToThree(ps[0], ps[1], ps[2]);
spawn.y = EYE;

const inside = colliders.filter((c) =>
  spawn.x + RADIUS > c.min.x && spawn.x - RADIUS < c.max.x &&
  spawn.z + RADIUS > c.min.z && spawn.z - RADIUS < c.max.z &&
  spawn.y + 0.15 > c.min.y && spawn.y - EYE < c.max.y
);
check("spawn is not inside geometry", inside.length === 0, inside.map((c) => c.name).join(", "));

// something solid under the player, or they fall forever
const floorBelow = colliders.some((c) =>
  spawn.x > c.min.x - 0.5 && spawn.x < c.max.x + 0.5 &&
  spawn.z > c.min.z - 0.5 && spawn.z < c.max.z + 0.5 &&
  c.max.y <= 0.4 && c.max.y >= -0.5
);
check("floor exists under the spawn", floorBelow);

// ---------------------------------------------------------------- lighting
// The black-screen bug: the spawn sits in a ceilinged corridor, so the sun is
// blocked. Ambient plus hemisphere must carry the interior on their own.
const src = readFileSync(join(here, "main.js"), "utf8");
const ambient = Number((src.match(/AmbientLight\(0xffffff,\s*([\d.]+)\)/) || [])[1] || 0);
const hemi = Number((src.match(/HemisphereLight\(0x[0-9a-f]+,\s*0x[0-9a-f]+,\s*([\d.]+)\)/) || [])[1] || 0);
const lampOn = /headlampOn = true/.test(src);
const ceilingsHidden = /mesh\.visible = false;\s*\/\/ hidden by default/.test(src);

check("ambient light present", ambient >= 0.4, `intensity ${ambient}`);
check("hemisphere light strong enough for interiors", hemi >= 1.5, `intensity ${hemi}`);
check("camera headlamp on by default", lampOn);
check("ceilings hidden by default so sun reaches rooms", ceilingsHidden);

const worstCaseSurface = ambient + hemi * 0.5;   // wall normal, half sky visibility
check("unlit-surface floor above black", worstCaseSurface > 0.8, `~${worstCaseSurface.toFixed(2)} before albedo`);

// ---------------------------------------------------------------- no CDN
const html = readFileSync(join(here, "index.html"), "utf8");
check("no external CDN dependency", !/unpkg|jsdelivr|cdnjs/.test(html));
check("importmap points at the vendored copy", /\.\/vendor\/three\.module\.js/.test(html));

console.log(failures === 0 ? "\nRESULT: PASS" : `\nRESULT: FAIL (${failures})`);
process.exit(failures === 0 ? 0 : 1);
