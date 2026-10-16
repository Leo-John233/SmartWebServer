// 使用 Arduino 生成的真实编译命令，在配置副本中检查所有网页的语言分支。
// node legacy/tests/firmware-languages.cjs <compile_commands.json> <输出目录>
const fs = require('node:fs');
const path = require('node:path');
const cp = require('node:child_process');
const assert = require('node:assert/strict');

const [databasePath, outputPath] = process.argv.slice(2);
assert(databasePath && outputPath, '需要 compile_commands.json 和输出目录');
const root = path.resolve(__dirname, '../SmartWebServer');
const database = JSON.parse(fs.readFileSync(databasePath, 'utf8'));
const entries = database.filter((entry) => /[/\\]src[/\\]pages[/\\].*\.cpp$/.test(entry.file));
assert(entries.length > 0, '编译清单中必须包含真实网页文件');
const originalSketch = entries[0].file.split(/[/\\]src[/\\]pages[/\\]/)[0];
const output = path.resolve(outputPath);
fs.mkdirSync(output, { recursive: true });
const sketch = fs.mkdtempSync(path.join(output, 'language-sketch-'));
fs.cpSync(path.join(root, 'src'), path.join(sketch, 'src'), { recursive: true });
fs.copyFileSync(path.join(root, 'Extended.config.h'), path.join(sketch, 'Extended.config.h'));
const config = fs.readFileSync(path.join(root, 'Config.h'), 'utf8');
assert(/^#define\s+DISPLAY_LANGUAGE\s+\w+/m.test(config));

function checkFile(entry) {
  const args = [];
  for (let i = 1; i < entry.arguments.length; i++) {
    const arg = entry.arguments[i];
    if (arg === '-o') { i++; continue; }
    if (arg === '-c' || arg === '-MMD') continue;
    args.push(arg.replaceAll(originalSketch, sketch));
  }
  args.push('-fsyntax-only');
  return new Promise((resolve, reject) => {
    const child = cp.spawn(entry.arguments[0], args, { cwd: root, windowsHide: true });
    let errors = '';
    child.stderr.on('data', (data) => { errors += data; });
    child.on('error', reject);
    child.on('close', (code) => code === 0 ? resolve() : reject(Error(entry.file + '\n' + errors)));
  });
}

(async () => {
  const report = [];
  for (const language of ['cn', 'en', 'us', 'ca', 'de', 'es', 'fr', 'it', 'jp', 'ro']) {
    fs.writeFileSync(path.join(sketch, 'Config.h'),
      config.replace(/^(#define\s+DISPLAY_LANGUAGE\s+)\w+/m, '$1L_' + language));
    let next = 0;
    const worker = async () => {
      while (next < entries.length) await checkFile(entries[next++]);
    };
    // 保持有限并发；本语言全部完成后才更改副本中的语言配置。
    await Promise.all([worker(), worker(), worker()]);
    report.push({ language, files: entries.length });
    console.log('PASS firmware DISPLAY_LANGUAGE=L_' + language + ': ' + entries.length + ' page files');
  }
  fs.writeFileSync(path.join(output, 'firmware-languages-report.json'), JSON.stringify(report, null, 2));
})().catch((error) => { console.error(error); process.exitCode = 1; });
