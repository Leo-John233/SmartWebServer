// 在测试中只替换配置选择，仍执行真实 Locale.h 的语言和单位分支
const fs = require('node:fs'),
  path = require('node:path');
module.exports = (language = process.env.SWS_TEST_LANGUAGE || 'cn') => {
  if (!['ca', 'cn', 'de', 'en', 'es', 'fr', 'it', 'jp', 'us', 'ro'].includes(language))
    throw Error('Unknown DISPLAY_LANGUAGE: ' + language);
  const root = path.join(__dirname, '../SmartWebServer/src');
  const read = (p) => fs.readFileSync(path.join(root, p), 'utf8');
  let locale = read('locales/Locale.h').replace(
    /^\s*#include\s+"(?:\.\.\/Constants.h|\.\.\/\.\.\/Config.h|Locales.h)".*$/gm,
    '',
  );
  locale = locale.replace(
    /#include\s+"(Strings_\w+\.h)"/g,
    (_, file) => '#include "' + path.join(root, 'locales', file).replaceAll('\\', '/') + '"',
  );
  return (
    '#define DISPLAY_LANGUAGE L_' +
    language +
    '\n' +
    read('Constants.h') +
    '\n' +
    read('locales/Locales.h') +
    '\n' +
    locale +
    '\n'
  );
};
