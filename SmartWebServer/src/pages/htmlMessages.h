#pragma once

#include "Pages.common.h"

const char html_bad_comms_message[] PROGMEM =
// 通信异常使用独立提示区域，避免错误内容破坏卡片布局
"<section class='sws-notice'><h2>" L_DOWN_TITLE "</h2><p>"
L_DOWN_MESSAGE1 L_DOWN_MESSAGE2 "</p><ul>"
" <li>" L_DOWN_MESSAGE3 "</li><br />"
" <li>" L_DOWN_MESSAGE4 "</li><br />"
" <li>" L_DOWN_MESSAGE5 "</li><br />"
" <li>" L_DOWN_MESSAGE6 "</li><br />"
" <li>" L_DOWN_MESSAGE7 "</li><br />"
#if DISPLAY_RESET_CONTROLS != OFF
  "<br /><form method='get' action='/configuration.htm'>"
  "<button name='advanced' type='submit' value='reset'" SWS_CONFIRM_ATTRIBUTE " onclick=\"return confirm(this.dataset.confirm);\" >" L_RESET "</button></form>\n"
#endif
"</ul></section>\n";
