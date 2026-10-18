#pragma once

#include <Arduino.h>
#include <stdarg.h>

// 按格式项追加，字符串直接借用，数字只使用小缓冲
inline void appendPageFormat(String &data, const char *format, ...) {
  va_list values;
  va_start(values, format);
  char part[96];
  size_t used = 0;
  for (size_t offset = 0;; ++offset) {
    char c = pgm_read_byte(format + offset);
    if (!c) break;
    if (c != '%') {
      part[used++] = c;
      if (used == sizeof(part) - 1) { part[used] = 0; data.concat(part); used = 0; }
      continue;
    }
    if (used) { part[used] = 0; data.concat(part); used = 0; }
    char spec[24] = "%";
    size_t size = 1;
    bool longValue = false;
    do {
      c = pgm_read_byte(format + ++offset);
      if (!c || size >= sizeof(spec) - 1) { va_end(values); return; }
      spec[size++] = c;
      if (c == 'l') longValue = true;
    } while (!strchr("%sdiuoxXcfFeEgG", c));
    spec[size] = 0;
    if (c == '%') { data.concat("%"); continue; }
    if (c == 's' && size == 2) { data.concat(va_arg(values, const char *)); continue; }
    switch (c) {
      case 'd': case 'i':
        if (longValue) snprintf(part, sizeof(part), spec, va_arg(values, long));
        else snprintf(part, sizeof(part), spec, va_arg(values, int));
        break;
      case 'u': case 'o': case 'x': case 'X':
        if (longValue) snprintf(part, sizeof(part), spec, va_arg(values, unsigned long));
        else snprintf(part, sizeof(part), spec, va_arg(values, unsigned int));
        break;
      case 'c': snprintf(part, sizeof(part), spec, va_arg(values, int)); break;
      case 's': snprintf(part, sizeof(part), spec, va_arg(values, const char *)); break;
      default: snprintf(part, sizeof(part), spec, va_arg(values, double)); break;
    }
    data.concat(part);
  }
  if (used) { part[used] = 0; data.concat(part); }
  va_end(values);
}

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
