#pragma once

#include <Arduino.h>

// 仅替换界面模板的 %s 和 %%，直接追加到原 String，避免整卡格式化缓冲占用小栈
inline void appendPageTemplate(String &data, const char *format, const char *const *values, size_t count) {
  // 先预留完整结果所需容量，逐片追加时不反复扩容
  size_t length = 0, argument = 0;
  for (size_t offset = 0;; ++offset) {
    const char c = pgm_read_byte(format + offset);
    if (!c) break;
    if (c == '%' && pgm_read_byte(format + offset + 1) == 's' && argument < count) {
      length += strlen(values[argument++]);
      ++offset;
    } else {
      ++length;
      if (c == '%' && pgm_read_byte(format + offset + 1) == '%') ++offset;
    }
  }
  data.reserve(data.length() + length);
  char part[65];
  size_t used = 0, value = 0;
  for (size_t offset = 0;; ++offset) {
    char c = pgm_read_byte(format + offset);
    if (!c) break;
    if (c == '%' && pgm_read_byte(format + offset + 1) == 's' && value < count) {
      if (used) { part[used] = 0; data.concat(part); used = 0; }
      data.concat(values[value++]);
      ++offset;
      continue;
    }
    if (c == '%' && pgm_read_byte(format + offset + 1) == '%') ++offset;
    part[used++] = c;
    if (used == sizeof(part) - 1) { part[used] = 0; data.concat(part); used = 0; }
  }
  if (used) { part[used] = 0; data.concat(part); }
}
