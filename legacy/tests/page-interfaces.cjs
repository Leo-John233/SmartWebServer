// 对照原始路由和后台入口，并检查实际生成控件的参数能够被固件读取
const fs = require('node:fs'),
  path = require('node:path'),
  cp = require('node:child_process'),
  assert = require('node:assert/strict');
const root = path.join(__dirname, '../SmartWebServer'),
  ref = '193a81809f5b19b3afa2a8c58f18291c2f0b0475',
  read = (p) => fs.readFileSync(path.join(root, p), 'utf8');
const git = (p) =>
  cp.execFileSync('git', ['show', ref + ':' + p], { encoding: 'utf8', maxBuffer: 8 * 1024 * 1024 });
const folders = {
  index: 'index',
  mount: 'mount',
  focuser: 'focuser',
  rotator: 'rotator',
  auxiliary: 'auxiliary',
  enc: 'encoders',
  net: 'network',
};
const patterns = {};
for (const [page, folder] of Object.entries(folders)) {
  const source = fs
    .readdirSync(path.join(root, 'src/pages', folder))
    .filter((p) => p.endsWith('.cpp'))
    .map((p) => read('src/pages/' + folder + '/' + p))
    .join('\n');
  const keys = new Set([...source.matchAll(/www\.(?:arg|hasArg)\(\s*"(\w+)"/g)].map((m) => m[1]));
  if (page === 'mount')
    for (const key of ['ra_h', 'ra_m', 'ra_s', 'dec_sign', 'dec_d', 'dec_m', 'dec_s'])
      keys.add(key);
  const dynamic = [...source.matchAll(/snprintf\([^;]*?,\s*"([a-z]\w*(?:%[cdsu]\w*)+)"/g)].map(
    (m) => new RegExp('^' + m[1].replace(/%[cdu]/g, '[0-9]+').replace(/%s/g, '[a-z0-9_]*') + '$'),
  );
  patterns[page] = { keys, dynamic };
}
module.exports = (page, controls) => {
  const { keys, dynamic } = patterns[page];
  let checked = 0;
  for (const control of controls) {
    const parameters = [];
    if (
      control.name &&
      control.inForm &&
      !control.disabled &&
      !control.readonly &&
      control.name !== 'view'
    )
      parameters.push(control.name);
    for (const handler of control.handlers || [])
      for (const m of handler.matchAll(/\b(s|sf|sz|g|gf|t)\(\s*'([^']+)'/g))
        parameters.push(['g', 'gf'].includes(m[1]) ? 'dr' : m[2]);
    for (const name of parameters) {
      assert(
        keys.has(name) || dynamic.some((p) => p.test(name)),
        page + ': ' + name + ' 没有对应的后台参数读取',
      );
      checked++;
    }
  }
  return checked;
};
function functions(source) {
  const mask = source.replace(
      /\/\*[\s\S]*?\*\/|\/\/[^\n]*|"(?:\\.|[^"\\])*"|'(?:\\.|[^'\\])*'/g,
      (m) => m.replace(/[^\n]/g, ' '),
    ),
    out = {};
  for (const match of mask.matchAll(/\b(?:void|bool|String)\s+(\w+)\s*\([^;{}]*\)\s*\{/g)) {
    let end = match.index + match[0].length,
      depth = 1;
    for (; end < mask.length && depth; end++) {
      if (mask[end] === '{') depth++;
      if (mask[end] === '}') depth--;
    }
    out[match[1]] = source.slice(match.index, end);
  }
  return out;
}
if (require.main === module) {
  const routes = (s) =>
    [...s.matchAll(/(?:www\.on|registerPageHandler)\("([^"]+)",\s*(\w+)\)/g)].map((m) => m[1] + '=' + m[2]);
  const original = routes(git('SmartWebServer.ino')),
    current = routes(read('SmartWebServer.ino'));
  for (const route of original) assert(current.includes(route), route + ' 原路由必须保留');
  const files = cp
    .execFileSync('git', ['ls-tree', '-r', '--name-only', ref, '--', 'src/pages'], {
      encoding: 'utf8',
    })
    .trim()
    .split('\n')
    .filter((p) => p.endsWith('.cpp'));
  const reviewed = new Set([
    'axisTileGet',
    'focuserSlewingTileGet',
    'rotatorSlewingTileGet',
    'mountAjaxGet',
    'encAxisTileGet',
    'siteTileGet', // 修正声明允许 -14 至 +12、实际却拒绝 -13/-14 的时区边界。
  ]);
  let unchanged = 0;
  const normalize = (s) => s.replace(/\/\*[\s\S]*?\*\/|\/\/[^\n]*/g, '').replace(/\s/g, '');
  for (const file of files) {
    assert(fs.existsSync(path.join(root, file)), file + ' 原页面必须保留');
    const before = functions(git(file)),
      after = functions(read(file));
    for (const [name, body] of Object.entries(before)) {
      if (!/Get$/.test(name)) continue;
      assert(after[name], name + ' 后台入口必须保留');
      if (reviewed.has(name)) continue;
      assert.equal(
        normalize(after[name]),
        normalize(body),
        file + ':' + name + ' 发生未核对的接口变化',
      );
      unchanged++;
    }
  }
  for (const file of [
    'src/libApp/cmd/Cmd.cpp',
    'src/libApp/cmd/Cmd.h',
    'src/pages/KeyValue.cpp',
    'src/pages/KeyValue.h',
  ])
    assert.equal(
      read(file).replace(/\r/g, ''),
      git(file).replace(/\r/g, ''),
      file + ' 设备通信和编码保持原始实现',
    );
  console.log(
    'PASS: ' +
      original.length +
      ' original routes, ' +
      unchanged +
      ' unchanged request handlers, six reviewed fixes/extensions, device transport and key/value encoding',
  );
}
