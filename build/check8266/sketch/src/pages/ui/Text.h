#line 1 "E:\\GitHub\\SmartWebServer\\legacy\\SmartWebServer\\src\\pages\\ui\\Text.h"
#pragma once

// 语言仍由 DISPLAY_LANGUAGE / Locale.h 在编译时选择。
// 将 C++ 字符串转为已转义的 JavaScript 字符串，避免引号破坏脚本。
#define SWS_JS_TEXT_INNER(text) #text
#define SWS_JS_TEXT(text) SWS_JS_TEXT_INNER(text)

// 文案放进 HTML 属性，由事件读取；语言里的单引号不会进入 JavaScript。
#define SWS_CONFIRM_ATTRIBUTE " data-confirm=\"" L_ARE_YOU_SURE "?\" "
