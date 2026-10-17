// 读取预处理后的相邻普通字符串和原始字符串，保留真实编译后的内容
module.exports = (source) => {
  const out = {};
  for (const match of source.matchAll(/const char (\w+)\[\]\s*=\s*/g)) {
    let cursor = match.index + match[0].length,
      value = '',
      valid = false;
    while (cursor < source.length) {
      while (/\s/.test(source[cursor] || '') && cursor < source.length) cursor++;
      if (source.startsWith('R"', cursor)) {
        const begin = source.indexOf('(', cursor),
          delimiter = source.slice(cursor + 2, begin),
          marker = ')' + delimiter + '"',
          end = source.indexOf(marker, begin);
        if (begin < 0 || end < 0) break;
        // C++ 翻译阶段会把源文件 CRLF 归一为 LF，原始字符串也一样。
        value += source.slice(begin + 1, end).replace(/\r\n/g, '\n');
        cursor = end + marker.length;
      } else if (source[cursor] === '"') {
        const token = /^"(?:\\.|[^"\\])*"/.exec(source.slice(cursor));
        if (!token) break;
        value += JSON.parse(token[0]);
        cursor += token[0].length;
      } else {
        valid = source[cursor] === ';';
        break;
      }
    }
    if (valid) out[match[1]] = value;
  }
  return out;
};
