require('./test-output.cjs');
// 检查中文基准的宏覆盖、条件分支、格式标记及实际坐标表单
const fs = require('node:fs');
const path = require('node:path');
const cp = require('node:child_process');
const assert = require('node:assert/strict');
const {chromium} = require('../../webui/node_modules/playwright');

const root = path.join(__dirname, '../SmartWebServer/src');
const folder = path.join(root, 'locales');
const files = fs.readdirSync(folder).filter(file => /^Strings_\w+\.h$/.test(file));
const source = file => fs.readFileSync(path.join(folder, file), 'utf8');
const keys = text => [...new Set([...text.matchAll(/^\s*#define\s+(\w+)/gm)].map(m => m[1]))].sort();
const baseline = keys(source('Strings_cn.h'));
const coordinateHeader = fs.readFileSync(path.join(root, 'pages/mount/GotoTile.h'), 'utf8');
const coordinate = coordinateHeader.slice(coordinateHeader.indexOf('const char html_coordinateInput[]'), coordinateHeader.indexOf('extern void gotoCoordinatesGet'));

function preprocess(file, special) {
  const code = '#define ON 1\n#define OFF 0\n#define PROGMEM\n' +
    '#define DISPLAY_SPECIAL_CHARS ' + special + '\n' +
    '#define HOME_OFFSET_RANGE_AXIS1 3600\n#define HOME_OFFSET_RANGE_AXIS2 3600\n' +
    '#define STR_INNER(x) #x\n#define STR(x) STR_INNER(x)\n' +
    '#include "' + path.join(folder, file).replaceAll('\\', '/') + '"\n' +
    baseline.map(key => 'const char* locale_' + key + ' = ' + key + ';').join('\n') + '\n' + coordinate;
  const args = ['-std=c++11', '-x', 'c++', '-'];
  // 编译真实宏展开，确保本地化字符串仍是合法的 C++ 常量
  cp.execFileSync(process.env.CXX || 'g++', ['-Werror', '-fsyntax-only', ...args], {input: code, stdio: ['pipe', 'pipe', 'pipe']});
  const output = cp.execFileSync(process.env.CXX || 'g++', ['-E', '-P', ...args], {input: code, encoding: 'utf8'});
  const values = new Map();
  for (const m of output.matchAll(/const char\* locale_(\w+)\s*=\s*((?:"(?:\\.|[^"\\])*"\s*)+);/g)) {
    values.set(m[1], [...m[2].matchAll(/"(?:\\.|[^"\\])*"/g)].map(t => JSON.parse(t[0])).join(''));
  }
  assert.equal(values.size, baseline.length, file + ' 宏必须全部展开为文字');
  const match = output.match(/const char html_coordinateInput\[\]\s*=\s*((?:"(?:\\.|[^"\\])*"\s*)+);/);
  assert(match, file + ' 坐标表单可展开');
  const html = [...match[1].matchAll(/"(?:\\.|[^"\\])*"/g)].map(t => JSON.parse(t[0])).join('');
  return {values, html};
}

const tokens = value => ({
  format: value.match(/%(?:\d+\$)?[-+ #0]*(?:\d+|\*)?(?:\.(?:\d+|\*))?(?:hh|ll|[hljztL])?[diuoxXfFeEgGaAcsp]/g) || [],
  tags: value.match(/<\/?[a-z][^>]*>/gi) || [],
});
const results = [];
for (const special of [0, 1]) {
  const chinese = preprocess('Strings_cn.h', special);
  for (const file of files) {
    assert.deepEqual(keys(source(file)), baseline, file + ' 的条目必须与中文基准一致');
    const translated = file === 'Strings_cn.h' ? chinese : preprocess(file, special);
    for (const key of baseline) {
      const value = translated.values.get(key);
      assert(value.trim(), file + ':' + key + ' 不能为空');
      assert(!value.includes('\uFFFD'), file + ':' + key + ' 不含损坏字符');
      assert.deepEqual(tokens(value), tokens(chinese.values.get(key)), file + ':' + key + ' 保留格式标记和 HTML');
    }
    if (special) results.push({file, ...translated});
  }
}

(async () => {
  const browser = await chromium.launch({channel: 'msedge', headless: true});
  try {
    const page = await browser.newPage();
    for (const {file, values, html} of results) {
      await page.setContent(html);
      const decoded = await page.evaluate(value => {
        const area = document.createElement('textarea');
        area.innerHTML = value;
        return area.value;
      }, Object.fromEntries(values).L_COORD_NETWORK);
      // 撇号和重音文字不能截断单引号包裹的 data 属性
      assert.equal(await page.locator('form').getAttribute('data-network'), decoded, file + ' 提示属性完整');
      for (const [attribute, key] of Object.entries({send:'L_COORD_SEND', ok:'L_COORD_OK', invalid:'L_COORD_INVALID', reject:'L_COORD_REJECT'})) {
        const expected = await page.evaluate(value => {const el=document.createElement('textarea');el.innerHTML=value;return el.value;}, values.get(key));
        assert.equal(await page.locator('form').getAttribute('data-' + attribute), expected, file + ':' + key);
      }
      assert.equal(await page.locator('input[required]').count(), 6, file + ' 坐标字段完整');
      assert.equal(await page.locator('button[value=sync]').innerText(), values.get('L_COORD_SYNC'), file + ' 使用本地同步文字');
    }
    console.log('PASS: ' + files.length + ' locales, ' + baseline.length + ' macros each, both symbol modes, C++ syntax, HTML and coordinate attributes');
  } finally {
    await browser.close();
  }
})().catch(error => {console.error(error); process.exitCode = 1;});
