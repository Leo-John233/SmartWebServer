// -----------------------------------------------------------------------------------
// Setup Network
#pragma once

#include "../htmlHeaders.h"
#include "../htmlMessages.h"
#include "../htmlScripts.h"

const char htmL_NETWORKSerial[] PROGMEM =
"<br/><hr><form method='post' action='/net.htm'>"
L_NETWORK_CMD_TO ": <input style='width:4em' name='ccto' value='%d' type='number' min='100' max='300'> ms<br/>"
L_NETWORK_WWW_TO ": <input style='width:4em' name='wcto' value='%d' type='number' min='100' max='300'> ms<br/>"
"<br/><button type='submit'>" L_UPLOAD "</button></form>\n";

const char htmL_NET_MAC[] PROGMEM =
"MAC: <input style='width:10em' name='%s' type='text' value='%s' maxlength='17' disabled><br/>";

const char htmL_NET_MAC_AP[] PROGMEM =
"MAC: <input style='width:10em' name='apmac' type='text' value='%s' maxlength='17' disabled>&nbsp;&nbsp;&nbsp;"
L_NETWORK_CHA ": <input style='width:3em' name='apch' value='%d' type='number' min='1' max='11'><br/>";

const char htmL_NET_IP[] PROGMEM =
"<table><tr><td>" L_IP_ADDRESS ": </td><td>"
"<input name='%sip1' value='%d' type='number' min='0' max='255'>&nbsp;.&nbsp;"
"<input name='%sip2' value='%d' type='number' min='0' max='255'>&nbsp;.&nbsp;"
"<input name='%sip3' value='%d' type='number' min='0' max='255'>&nbsp;.&nbsp;"
"<input name='%sip4' value='%d' type='number' min='0' max='255'></td>";

const char htmL_NET_GW[] PROGMEM =
"<tr><td>" L_GATEWAY ": </td><td>"
"<input name='%sgw1' value='%d' type='number' min='0' max='255'>&nbsp;.&nbsp;"
"<input name='%sgw2' value='%d' type='number' min='0' max='255'>&nbsp;.&nbsp;"
"<input name='%sgw3' value='%d' type='number' min='0' max='255'>&nbsp;.&nbsp;"
"<input name='%sgw4' value='%d' type='number' min='0' max='255'></td>";

const char htmL_NET_SN[] PROGMEM =
"<tr><td>" L_SUBNET ": </td><td>"
"<input name='%ssn1' value='%d' type='number' min='0' max='255'>&nbsp;.&nbsp;"
"<input name='%ssn2' value='%d' type='number' min='0' max='255'>&nbsp;.&nbsp;"
"<input name='%ssn3' value='%d' type='number' min='0' max='255'>&nbsp;.&nbsp;"
"<input name='%ssn4' value='%d' type='number' min='0' max='255'></td></tr></table>";

#if OPERATIONAL_MODE == WIFI
  const char htmL_NETWORKSSID1[] PROGMEM =
  "<br/><hr><form method='post' action='/net.htm'>"
  "SSID: <input style='width:13em; text-align:left;' name='stssid' type='text' value='%s' maxlength='32'>&nbsp;&nbsp;&nbsp;"
  L_NETWORK_PWD ": <input style='width:8em; text-align:left;' name='stpwd' type='password' minlength='8' maxlength='63'> (" L_NETWORK_PWD_MSG ")<br/>";

  const char htmL_NETWORKSSID2[] PROGMEM =
  L_NETWORK_EN_DHCP ": <input type='checkbox' name='stadhcp' value='1' %s> (" L_NETWORK_EN_DHCP_MSG ")<br/>"
  L_NETWORK_EN_STA ": <input type='checkbox' name='staen' value='1' %s><br/>"
  "<br/><button type='submit'>" L_UPLOAD "</button></form>\n";

  const char htmL_NETWORKSSID3A[] PROGMEM =
  "<br/><hr><form method='post' action='/net.htm'>"
  "SSID: <input style='width:13em; text-align:left;' name='apssid' type='text' ";

  const char htmL_NETWORKSSID3B[] PROGMEM =
  "value='%s' maxlength='31'>&nbsp;&nbsp;&nbsp;"
  L_NETWORK_PWD ": <input style='width:8em; text-align:left;' name='appwd' type='password' minlength='8' maxlength='31'> (" L_NETWORK_PWD_MSG ")<br/>";

  const char htmL_NETWORKSSID7[] PROGMEM =
  L_NETWORK_EN_AP_MODE ": <input type='checkbox' name='apen' value='1' %s><br/>"
  "<br/><button type='submit'>" L_UPLOAD "</button></form>\n";
#else
  const char htmL_NETWORK_ETH_DHCP[] PROGMEM =
  L_NETWORK_EN_DHCP ": <input type='checkbox' name='ethdhcp' value='1' %s> (" L_NETWORK_EN_DHCP_MSG ")<br/>";

  const char htmL_NETWORK_ETH_BEG[] PROGMEM =
  "<br/><hr><form method='post' action='/net.htm'>\n";

  const char htmL_NETWORK_ETH_END[] PROGMEM =
  "<button type='submit'>" L_UPLOAD "</button></form>\n";
#endif

#ifdef OTA_PRESENT
  const char html_network_ota_begin[] PROGMEM =
  "<div class='sws-ota'><p class='sws-ota-intro'>" L_NETWORK_OTA_HELP "</p>"
  "<form method='post' action='/net.htm' class='sws-ota-form'>"
  "<label for='sws-ota-password'>" L_NETWORK_OTA_PASSWORD "</label>"
  "<input id='sws-ota-password' name='otapwd' type='password' minlength='8' maxlength='63' autocomplete='new-password' aria-describedby='sws-ota-password-help'>"
  "<p id='sws-ota-password-help' class='sws-ota-help'>" L_NETWORK_OTA_PASSWORD_HELP "</p>";

  #if DISPLAY_RESET_CONTROLS != OFF
    const char html_network_ota_action[] PROGMEM =
    "<br/><button " SWS_CONFIRM_ATTRIBUTE " onclick=\"return confirm(this.dataset.confirm)\" type='submit' name='otareboot' value='1'>" L_REBOOT_OTA "</button></form>\n"
    ;
  #else
    const char html_network_ota_mode[] PROGMEM =
    "<div class='sws-ota-mode'><input type='checkbox' name='otaen' value='1' aria-label=\"" L_NETWORK_EN_OTA "\" %s><span>" L_NETWORK_EN_OTA "</span></div>";
    const char html_network_ota_action[] PROGMEM =
    "<button type='submit'>" L_SAVE_SETTINGS "</button></form>\n";
  #endif
  const char html_network_ota_end[] PROGMEM =
  "<div class='sws-ota-guide'><h3>" L_NETWORK_OTA_STEPS "</h3><ol><li>"
  #if DISPLAY_RESET_CONTROLS != OFF
    L_REBOOT_OTA
  #else
    L_NETWORK_OTA_STEP_RESTART
  #endif
  "</li><li>" L_NETWORK_OTA_STEP_PORT "</li><li>" L_NETWORK_OTA_STEP_UPLOAD "</li></ol></div>"
  "<p class='sws-ota-help'>" L_NETWORK_OTA_NOTE "</p></div>";
#endif

const char html_logout[] PROGMEM =
"<h2>" L_WEB_PASSWORD_TITLE "</h2><p class='sws-account-note'>" L_WEB_PASSWORD_HELP "</p>"
"<div class='sws-account-controls'><form method='post' action='/net.htm'>"
"<label for='sws-web-password'>" L_WEB_PASSWORD_NEW "</label><div class='sws-password-actions'>"
"<input id='sws-web-password' name='webpwd' type='password' minlength='3' maxlength='31' autocomplete='new-password' aria-describedby='sws-web-password-rule'>"
"<button type='submit'>" L_WEB_PASSWORD_SAVE "</button></div></form>"
"<form class='sws-logout-form' method='post' action='/net.htm'>"
"<button class='sws-secondary' type='submit' name='logout' value='1'>" L_WEB_LOGOUT "</button></form></div>"
"<p id='sws-web-password-rule' class='sws-account-note'>" L_WEB_PASSWORD_RULE "</p>\n";

const char html_reboot[] PROGMEM =
"<br/><br/><br/><br/><br/><form method='post' action='/net.htm'>"
"<b>" L_NETWORK_RESTART_MSG "</b><br/><br/>"
"<button type='submit'>" L_CONTINUE "</button>"
"</form><br/><br/><br/><br/>"
"\n";

const char html_login[] PROGMEM =
"<div class='nw-card sws-network-entry'><section class='sws-login-card'>"
"<h2>" L_WEB_PASSWORD_TITLE "</h2><p id='sws-login-help' class='sws-entry-note'>" L_WEB_PASSWORD_HELP "</p>"
"<form method='post' action='/net.htm'><label for='sws-login-password'>" L_NETWORK_PWD "</label>"
"<div class='sws-login-actions'><input id='sws-login-password' name='login' type='password' minlength='3' maxlength='31' required autocomplete='current-password' aria-describedby='sws-login-help'>"
"<button type='submit'>" L_OK "</button></div></form></section>"
#if OPERATIONAL_MODE == WIFI
"<section class='sws-network-help'><h2>" L_NETWORK_ADVICE1 "</h2>"
"<p>" L_NETWORK_ADVICE2 "</p><p>" L_NETWORK_ADVICE3 L_NETWORK_ADVICE4 L_NETWORK_ADVICE5 L_NETWORK_ADVICE6 "</p>"
"<p>" L_NETWORK_ADVICE7 L_NETWORK_ADVICE8 L_NETWORK_ADVICE9 "</p></section>"
#endif
"</div>\n";
