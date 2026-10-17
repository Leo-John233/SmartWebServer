#pragma once
#include <cstdio>
#include <cstring>

// 严格检查整数坐标，禁止空值、尾随字符和越界值进入设备协议
inline bool coordinateInteger(const char *value, int maximum, int &number) {
  if (!value || !*value) return false;
  number = 0;
  for (const char *p = value; *p; ++p) {
    if (*p < '0' || *p > '9') return false;
    number = number * 10 + (*p - '0');
    if (number > maximum) return false;
  }
  return true;
}
inline bool coordinateCommands(const char *const values[7], char *ra, char *dec) {
  int h, m, s, d, dm, ds;
  if (!coordinateInteger(values[0], 23, h) || !coordinateInteger(values[1], 59, m) ||
      !coordinateInteger(values[2], 59, s) || !coordinateInteger(values[4], 90, d) ||
      !coordinateInteger(values[5], 59, dm) || !coordinateInteger(values[6], 59, ds)) return false;
  if ((strcmp(values[3], "+") && strcmp(values[3], "-")) || (d == 90 && (dm || ds))) return false;
  snprintf(ra, 24, ":Sr%02d:%02d:%02d#", h, m, s);
  snprintf(dec, 24, ":Sd%c%02d*%02d:%02d#", values[3][0], d, dm, ds);
  return true;
}
