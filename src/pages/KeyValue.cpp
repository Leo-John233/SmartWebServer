// key/value handling
#include <Arduino.h>
#include "KeyValue.h"

String keyValueString(const char *key, const char *value1, const char *value2, const char *value3, const char *value4) {
  key = keyValueChars(key); value1 = keyValueChars(value1); value2 = keyValueChars(value2);
  value3 = keyValueChars(value3); value4 = keyValueChars(value4);
  String result;
  result.reserve(strlen(key) + strlen(value1) + strlen(value2) + strlen(value3) + strlen(value4) + 2);
  result.concat(key); result.concat("|"); result.concat(value1); result.concat(value2);
  result.concat(value3); result.concat(value4); result.concat("\n");
  return result;
}

String keyValueToggleBoolSelected(const char *keyOn, const char *keyOff, bool selectState) {
  keyOn = keyValueChars(keyOn); keyOff = keyValueChars(keyOff);
  String result;
  result.reserve(strlen(keyOn) + strlen(keyOff) + 22);
  result.concat(keyOn); result.concat("|"); result.concat(selectState ? "selected\n" : "unselected\n");
  result.concat(keyOff); result.concat("|"); result.concat(selectState ? "unselected\n" : "selected\n");
  return result;
}

String keyValueBoolSelected(const char *key, bool selectState) {
  return keyValueString(key, selectState ? "selected" : "unselected");
}

String keyValueBoolEnabled(const char *key, bool state) {
  return keyValueString(key, state ? "enabled" : "disabled");
}
