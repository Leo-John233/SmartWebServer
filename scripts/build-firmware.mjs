import {
  cpSync,
  existsSync,
  lstatSync,
  mkdirSync,
  readFileSync,
  realpathSync,
  rmSync,
  writeFileSync,
} from "node:fs";
import { dirname, join, resolve, sep } from "node:path";
import { fileURLToPath } from "node:url";
import { spawnSync } from "node:child_process";

const root = realpathSync(
  resolve(dirname(fileURLToPath(import.meta.url)), ".."),
);
const [variant, ...args] = process.argv.slice(2);
if (!["legacy", "vue"].includes(variant))
  throw new Error(
    "Usage: node scripts/build-firmware.mjs legacy|vue [--compile --fqbn BOARD]",
  );
const compiling = args.includes("--compile");
const fqbnIndex = args.indexOf("--fqbn");
const fqbn = fqbnIndex >= 0 ? args[fqbnIndex + 1] : process.env.SWS_FQBN;
if (compiling && !fqbn)
  throw new Error("Specify --fqbn or SWS_FQBN for the target board");
const source = join(root, "legacy", "SmartWebServer");
const overlay = join(root, "webui", "firmware");
const buildRoot = join(root, "build");
const output = join(buildRoot, variant);
const marker = ".sws-generated";

// 只替换本脚本生成的构建目录，拒绝链接路径及手动创建的目录
for (const path of [buildRoot, output]) {
  if (
    existsSync(path) &&
    (lstatSync(path).isSymbolicLink() || realpathSync(path) !== path)
  ) {
    throw new Error("Build path must not be redirected: " + path);
  }
}
if (!output.startsWith(root + sep + "build" + sep))
  throw new Error("Output is outside the build directory");
if (existsSync(output)) {
  if (
    !existsSync(join(output, marker)) ||
    readFileSync(join(output, marker), "utf8") !== variant
  ) {
    throw new Error("Refusing to overwrite an unmanaged directory: " + output);
  }
  rmSync(output, { recursive: true });
}
mkdirSync(output, { recursive: true });
writeFileSync(join(output, marker), variant);
const sketch = join(output, "SmartWebServer");
cpSync(source, sketch, { recursive: true });
if (variant === "vue") {
  for (const name of ["api", "webui"])
    cpSync(join(overlay, name), join(sketch, "src", name), { recursive: true });
  for (const name of ["Config.h", "Extended.config.h"])
    cpSync(join(overlay, name), join(sketch, name));
  const entryPath = join(sketch, "SmartWebServer.ino");
  let entry = readFileSync(entryPath, "utf8");
  // 精确检查接入位置，原版启动流程变化时停止构建，防止静默遗漏路由
  for (const anchor of [
    '#include "src/Common.h"',
    'www.on("/", handleRoot);',
  ]) {
    if (entry.split(anchor).length !== 2)
      throw new Error("Expected one integration anchor: " + anchor);
  }
  entry = entry
    .replace(
      '#include "src/Common.h"',
      '#include "src/Common.h"\n#include "src/webui/VueRoutes.h"',
    )
    .replace('www.on("/", handleRoot);', "registerVueRoutes();");
  writeFileSync(entryPath, entry);
}
console.log("Prepared " + variant + ": " + sketch);
if (compiling) {
  const result = spawnSync(
    "arduino-cli",
    [
      "compile",
      "--fqbn",
      fqbn,
      "--output-dir",
      join(output, "artifacts"),
      sketch,
    ],
    { stdio: "inherit", shell: false },
  );
  if (result.error) throw result.error;
  process.exitCode = result.status ?? 1;
}
