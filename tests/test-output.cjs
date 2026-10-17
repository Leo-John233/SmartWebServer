// 测试产物集中放在仓库内，避免在系统临时目录执行生成的程序。
const fs = require('node:fs');
const path = require('node:path');
const output = path.join(__dirname, 'output');
const temporary = path.join(output, 'temp');
fs.mkdirSync(temporary, { recursive: true });

// 编译器中间文件及浏览器临时配置也使用此目录。
process.env.TEMP = temporary;
process.env.TMP = temporary;
process.env.TMPDIR = temporary;
module.exports = output;
