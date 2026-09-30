import { strict as assert } from "node:assert";
import { createHash } from "node:crypto";
import { existsSync, readFileSync, readdirSync } from "node:fs";
import { dirname, join, resolve } from "node:path";
import { fileURLToPath } from "node:url";
import { spawnSync } from "node:child_process";
import test from "node:test";

const root = resolve(dirname(fileURLToPath(import.meta.url)), "..");
function fingerprint(directory) {
  const hash = createHash("sha256");
  function visit(path) {
    for (const entry of readdirSync(path, { withFileTypes: true }).sort(
      (a, b) => a.name.localeCompare(b.name),
    )) {
      const file = join(path, entry.name);
      hash.update(entry.name);
      if (entry.isDirectory()) visit(file);
      else hash.update(readFileSync(file));
    }
  }
  visit(directory);
  return hash.digest("hex");
}
function prepare(variant) {
  const result = spawnSync(
    process.execPath,
    [join(root, "scripts/build-firmware.mjs"), variant],
    { encoding: "utf8" },
  );
  assert.equal(result.status, 0, result.stderr);
}

test("two firmware builds remain isolated and leave sources unchanged", () => {
  const legacySource = join(root, "legacy", "SmartWebServer");
  const vueSource = join(root, "webui", "firmware");
  const beforeLegacy = fingerprint(legacySource);
  const beforeVue = fingerprint(vueSource);
  prepare("legacy");
  const legacy = join(root, "build", "legacy", "SmartWebServer");
  const beforeLegacyBuild = fingerprint(legacy);
  prepare("vue");
  const vue = join(root, "build", "vue", "SmartWebServer");
  assert.equal(fingerprint(legacy), beforeLegacyBuild);
  assert.equal(existsSync(join(legacy, "src/webui")), false);
  assert.equal(existsSync(join(legacy, "src/api/ApiV1.cpp")), false);
  assert.match(
    readFileSync(join(legacy, "SmartWebServer.ino"), "utf8"),
    /www.on\("\/", handleRoot\)/,
  );
  assert.match(
    readFileSync(join(vue, "SmartWebServer.ino"), "utf8"),
    /registerVueRoutes\(\)/,
  );
  assert.ok(existsSync(join(vue, "src/webui/WebAssets.generated.h")));
  assert.equal(
    readFileSync(join(vue, "Config.h"), "utf8"),
    readFileSync(join(vueSource, "Config.h"), "utf8"),
  );
  assert.equal(fingerprint(legacySource), beforeLegacy);
  assert.equal(fingerprint(vueSource), beforeVue);
  const beforeVueBuild = fingerprint(vue);
  prepare("legacy");
  assert.equal(fingerprint(vue), beforeVueBuild);
});
