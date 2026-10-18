// key/value handling
#pragma once

String keyValueString(const char *key, const char *value1, const char *value2 = "", const char *value3 = "", const char *value4 = "");
String keyValueToggleBoolSelected(const char *keyOn, const char *keyOff, bool selectState);
String keyValueBoolSelected(const char *key, bool selectState);
String keyValueBoolEnabled(const char *key, bool state);

// 兼容现有 String 调用，参数只借用文本，组装完成后不保留指针
inline const char *keyValueChars(const char *value) { return value ? value : ""; }
inline const char *keyValueChars(const String &value) { return value.c_str(); }
template<class Key, class Value, class... Values>
inline String keyValueString(const Key &key, const Value &value, const Values &...values) {
  return keyValueString(keyValueChars(key), keyValueChars(value), keyValueChars(values)...);
}
template<class On, class Off>
inline String keyValueToggleBoolSelected(const On &keyOn, const Off &keyOff, bool state) {
  return keyValueToggleBoolSelected(keyValueChars(keyOn), keyValueChars(keyOff), state);
}
template<class Key> inline String keyValueBoolSelected(const Key &key, bool state) {
  return keyValueBoolSelected(keyValueChars(key), state);
}
template<class Key> inline String keyValueBoolEnabled(const Key &key, bool state) {
  return keyValueBoolEnabled(keyValueChars(key), state);
}
