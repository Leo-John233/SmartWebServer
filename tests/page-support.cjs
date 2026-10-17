const fs = require('node:fs');
const path = require('node:path');
module.exports = () =>
  ['Palette.h', 'Text.h', 'Layouts.h']
    .map((file) =>
      fs
        .readFileSync(path.join(__dirname, '../SmartWebServer/src/pages/ui', file), 'utf8')
        .replace(/^#pragma once.*$/gm, ''),
    )
    .join('\n');
