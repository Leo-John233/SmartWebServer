#pragma once

#include "Pages.common.h"
#include "../extensions/ui/Palette.h"
#include "../extensions/ui/Text.h"

const char html_head_begin[] PROGMEM = "<!DOCTYPE HTML>\n"
                                  "<html class='sws-page'>\n"
                                  "<head>\n"
                                  "<meta charset='utf-8'/>\n"
                                  "<meta name='color-scheme' content='dark'>\n"
                                  "<link rel='expect' blocking='render' href='#sws-nav-ready'>\n"
                                  "<meta name=\"viewport\" content=\"width=device-width,initial-scale=1\">\n"
                                  "<style>html,body{background:" COLOR_BODY_BACKGROUND ";color:" SWS_COLOR_TEXT ";color-scheme:dark}</style>\n";
const char html_headerPec[] PROGMEM = "<meta http-equiv=\"refresh\" content=\"5; URL=/pec.htm\">\n";
const char html_headerIdx[] PROGMEM = "<meta http-equiv=\"refresh\" content=\"5; URL=/index.htm\">\n";
const char html_head_end[] PROGMEM = "</head>\n";
const char html_body_begin[] PROGMEM = "<body bgcolor='" COLOR_BODY_BACKGROUND "'>\n";
const char html_page_and_body_end[] PROGMEM = "</div><span id='sws-page-ready' hidden></span></body></html>\n";

const char html_main_css_begin[] PROGMEM = 
"<STYLE>";

const char html_main_css_core[] PROGMEM =
".clear{ clear: both}.a{ background-color: " COLOR_BACKGROUND "}input{ width:5em; background-color: " COLOR_CONTROL "}.b{ border-left: 5px solid " COLOR_BORDER "; border-right: 5px solid " COLOR_BORDER "; border-bottom: 5px solid " COLOR_BORDER "}select{ width:4em; background-color: " COLOR_CONTROL "}.c{ font-weight: bold}h1{ text-align: right}a:hover, a:active{ background-color: " COLOR_LINK_BORDER "}.y{ font-weight: bold}a:link, a:visited{ border:1px solid " COLOR_LINK_BORDER "; padding: 5px 10px; margin: none; text-align: center; text-decoration: none; display: inline-block}button{ background-color: " COLOR_CONTROL "}";

const char html_main_css_no_select[] PROGMEM =
"body { -webkit-user-select: none; -moz-user-select: none; -ms-user-select: none; user-select: none; }";

const char html_main_css_buttons[] PROGMEM = 
".btns_left{ margin-left: -1px; border-top-left-radius: 0px; border-bottom-left-radius: 0px}.btns_mid{ margin-left: -1px; margin-right: -1px; border-top-left-radius: 0px; border-bottom-left-radius: 0px; border-top-right-radius: 0px; border-bottom-right-radius: 0px}.btns_right{ margin-right: -1px; border-top-right-radius: 0px; border-bottom-right-radius: 0px}";

const char html_main_css_control[] PROGMEM = 
".b1{ text-align: center}.gb{  font-weight: bold; font-size: 150%; width: 60px; height: 50px; padding: 0px; touch-action: none; -webkit-touch-callout: none}.bb{  font-weight: bold; font-size: 105%}.bbh{  font-weight: bold; font-size: 100%; height: 2.1em; touch-action: none; -webkit-touch-callout: none}";

const char html_main_css_collapse[] PROGMEM =
".collapsible{ cursor: pointer; padding: 7px; outline: none; font-size: 14px}.content{ display: none; overflow: hidden}";

const char html_main_css_end[] PROGMEM = 
"</STYLE>";

// tile: begin takes title and width
const char html_tile_beg[] PROGMEM = "<div class='b1' style='width: %s; min-height: %s;'><div style='float: left;'>%s:</div>\n";
// tile: begin for text takes title and width
const char html_tile_text_beg[] PROGMEM = "<div class='b1' style='text-align: left; width: %s; min-height: %s;'><div style='float: left;'>%s:</div>\n";
// tile: adds status to first line
const char html_tile_status[] PROGMEM = "<div style='float: right; text-align: right;' id='%s' class='c'>%s</div><br />\n";
// tile: end
const char html_tile_end[] PROGMEM = "</div>\n";

// collapsable: begin takes title
const char html_collapsable_beg[] PROGMEM = 
"<div style='text-align: left;'> <button type='button' class='collapsible' style='vertical-align: bottom'>%s</button>"
"<div class='content' style='text-align: left;'><br />\n";
// collapsable: end
const char html_collapsable_end[] PROGMEM = "<br /></div></div>\n";

// form: begin takes webpage for get
const char html_form_begin[] PROGMEM = "<form method='get' action='/%s'>\n";
// form: end
const char html_form_end[] PROGMEM = "</form>\n";

// input: medium width numeric takes name (for get,) value, min, max, unit
const char html_ninput_wide[] PROGMEM =
"<input style='width: 4em;' type='number' name='%s' value='%s' min='%s' max='%s'>&nbsp;%s";
// input: narrow width numeric takes name (for get,) value, min, max, unit
const char html_ninput[] PROGMEM =
"<input style='width: 3.5em;' type='number' name='%s' value='%s' min='%s' max='%s'>&nbsp;%s";
