#include "ApiV1.h"

#if defined(ESP8266) && OPERATIONAL_MODE == WIFI && WEB_SERVER == ON

#include "SemanticActions.h"
#include "../pages/Pages.common.h"
#include "../webui/WebUiServer.h"
#include "../lib/wifi/WifiManager.h"

extern bool loginRequired;
extern bool restartRequired;

namespace {

void appendJsonString(String& json, const char* value) {
  json.concat('"');
  while (*value) {
    if (*value == '"' || *value == '\\') json.concat('\\');
    if (static_cast<uint8_t>(*value) >= 0x20) json.concat(*value);
    value++;
  }
  json.concat('"');
}

void addString(String& json, bool& first, const char* key, const char* value) {
  if (!first) json.concat(',');
  first = false;
  appendJsonString(json, key);
  json.concat(':');
  appendJsonString(json, value);
}

void addBool(String& json, bool& first, const char* key, bool value) {
  if (!first) json.concat(',');
  first = false;
  appendJsonString(json, key);
  json.concat(value ? F(":true") : F(":false"));
}

void addNumberString(String& json, bool& first, const char* key, long value) {
  char text[20];
  snprintf(text, sizeof(text), "%ld", value);
  addString(json, first, key, text);
}

void addNumber(String& json, bool& first, const char* key, long value) {
  if (!first) json.concat(',');
  first = false;
  appendJsonString(json, key);
  json.concat(':');
  json.concat(value);
}

void sendResult(int statusCode, bool ok, const char* error = "") {
  String json = F("{\"ok\":");
  json.concat(ok ? F("true") : F("false"));
  if (error[0]) {
    json.concat(F(",\"error\":"));
    appendJsonString(json, error);
  }
  json.concat('}');
  www.send(statusCode, "application/json", json);
}

String internalDirection(const String& direction, bool start) {
  char prefix = direction == "north" ? 'n' : direction == "south" ? 's' : direction == "east" ? 'e' : direction == "west" ? 'w' : 0;
  if (!prefix) return EmptyStr;
  String value(prefix);
  value.concat(start ? '1' : '0');
  return value;
}

String internalSlewRate(const String& rate) {
  if (rate == "verySlow") return "vs";
  if (rate == "slow") return "s";
  if (rate == "normal") return "n";
  if (rate == "fast") return "f";
  if (rate == "veryFast") return "vf";
  return EmptyStr;
}

bool dispatchAction(const String& action) {
  if (action == "mount.park") return performMountHomeAction("pk", "p");
  if (action == "mount.unpark") return performMountHomeAction("pk", "u");
  if (action == "mount.setPark") return performMountHomeAction("pk", "s");
  if (action == "mount.home") return performMountHomeAction("qb", www.arg("mode") == "find" ? "hc" : "hf");
  if (action == "mount.autoHome") return performMountHomeAction("ha", www.arg("enabled") == "true" ? "1" : "0");
  if (action == "mount.align.start") return performMountAlignAction("al", www.arg("stars"));
  if (action == "mount.align.accept") return performMountAlignAction("al", "n");
  if (action == "mount.align.cancel") return performMountAlignAction("al", "q");
  if (action == "mount.align.refine") return performMountAlignAction("rp", "a");
  if (action == "mount.goto.start") return performMountGotoAction("go");
  if (action == "mount.goto.stop") return performMountGotoAction("stop");
  if (action == "mount.goto.rate") return performMountGotoAction(internalSlewRate(www.arg("rate")));
  if (action == "mount.goto.flip") return performMountGotoAction("mh_" + www.arg("mode"));
  if (action == "mount.guide.start") return performMountGuideAction(internalDirection(www.arg("direction"), true));
  if (action == "mount.guide.stop") return performMountGuideAction(internalDirection(www.arg("direction"), false));
  if (action == "mount.guide.stopAll") return performMountGuideAction("q");
  if (action == "mount.guide.rate") return performMountGuideAction("R" + www.arg("rate"));
  if (action == "mount.sync") return performMountGuideAction("sy");
  if (action == "mount.tracking") return performMountTrackingAction("dr", www.arg("enabled") == "true" ? "T1" : "T0");
  if (action == "mount.tracking.mode") {
    String mode = www.arg("mode");
    return performMountTrackingAction("dr", mode == "sidereal" ? "Ts" : mode == "lunar" ? "Tl" : mode == "solar" ? "Th" : mode == "king" ? "Tk" : "");
  }
  if (action == "mount.tracking.adjust") {
    String adjustment = www.arg("adjustment");
    return performMountTrackingAction("tk", adjustment == "faster" ? "f" : adjustment == "slower" ? "-" : adjustment == "reset" ? "r" : "");
  }
  if (action == "mount.pec") {
    String command = www.arg("command");
    return performMountPecAction("pec", command == "record" ? "rec" : command == "clear" ? "clr" : command == "write" ? "wrt" : command);
  }
  if (action == "mount.coordinateOrigin") return performMountPecAction("ob", "set");
  if (action == "rotator.move") {
    String value = www.arg("direction") == "counterclockwise" ? "ccw" : www.arg("direction") == "clockwise" ? "cw" : "";
    if (www.arg("fast") == "true") value.concat('f');
    return performRotatorAction("rot", value);
  }
  if (action == "rotator.stop") return performRotatorAction("rot", "stop");
  if (action == "rotator.home") return performRotatorAction("rot", www.arg("mode") == "find" ? "home" : "reset");
  if (action == "rotator.position") return performRotatorAction("rs", www.arg("degrees"));
  if (action == "rotator.rate") return performRotatorAction("rot", internalSlewRate(www.arg("rate")));
  if (action == "rotator.derotate") {
    String command = www.arg("command");
    return performRotatorAction("rot", command == "reverse" ? "rev" : command == "parallactic" ? "par" : command);
  }
  if (action == "rotator.backlash") return performRotatorAction("b3", www.arg("steps"));
  if (action == "focuser.select") return performFocuserAction("dr", "F" + www.arg("index"));
  if (action == "focuser.move") {
    String value = www.arg("direction");
    if (www.arg("fast") == "true") value.concat('f');
    return performFocuserAction("foc", value);
  }
  if (action == "focuser.stop") return performFocuserAction("foc", "stop");
  if (action == "focuser.home") return performFocuserAction("dr", www.arg("mode") == "find" ? "Fh" : "FH");
  if (action == "focuser.position") return performFocuserAction("fs", www.arg("steps"));
  if (action == "focuser.rate") return performFocuserAction("foc", internalSlewRate(www.arg("rate")));
  if (action == "encoders.sync") {
    String direction = www.arg("direction");
    return performEncoderAction(direction == "controllerToEncoder" ? "o2e" : direction == "encoderToController" ? "e2o" : "zro");
  }
  if (action == "encoders.autoSync") return performEncoderAction(www.arg("enabled") == "true" ? "on" : "off");
  return false;
}

void beginState(String& json, const String& scope) {
  json = F("{\"schemaVersion\":1,\"scope\":");
  appendJsonString(json, scope.c_str());
  json.concat(F(",\"values\":{"));
}

void appendControllerState(String& values, bool& first) {
  state.updateController(true);
  char product[16] = "";
  char version[16] = "";
  status.getProduct(product);
  status.getVersionStr(version);
  addString(values, first, "fw_name", product);
  addString(values, first, "version", version);
  addString(values, first, "work_load", state.workLoadStr);
  addString(values, first, "last_err", state.lastErrorStr);
  addString(values, first, "signal", state.signalStrengthStr);
  addString(values, first, "tphd_m", state.controllerTemperatureStr);
  addString(values, first, "tphd_t", state.siteTemperatureStr);
  addString(values, first, "tphd_p", state.sitePressureStr);
  addString(values, first, "tphd_h", state.siteHumidityStr);
  addString(values, first, "tphd_d", state.siteDewPointStr);
  for (int axis = 0; axis < (status.getVersionMajor() >= 10 ? 9 : 2); axis++) {
    char key[24];
    snprintf(key, sizeof(key), "dvr_stat%d", axis + 1);
    addString(values, first, key, state.driverStatusStr[axis]);
  }
}

void appendMountState(String& values, bool& first, String& selected, bool& firstSelected, String& enabled, bool& firstEnabled) {
  state.updateMount(true);
  addString(values, first, "hp_sta", status.parked ? "Parked" : status.parking ? "Parking" : status.atHome ? "At home" : "Ready");
  addString(values, first, "gto_status", status.inGoto ? "Slewing" : "Inactive");
  addString(values, first, "gto_t1", state.targetRaStr);
  addString(values, first, "gto_t2", state.targetDecStr);
  addString(values, first, "gto_i1", state.indexRaStr);
  addString(values, first, "gto_i2", state.indexDecStr);
  addString(values, first, "gto_az1", state.indexAzmStr);
  addString(values, first, "gto_az2", state.indexAltStr);
  addString(values, first, "gto_rate", state.slewSpeedStr);
  addString(values, first, "track", state.trackStr);
  addString(values, first, "guide_sta", status.guiding ? "Slewing" : "Inactive");
  addString(values, first, "guide_rate", GuideRatesStr[status.guideRate]);
  addString(values, first, "date_ut", state.dateStr);
  addString(values, first, "time_ut", state.timeStr);
  addString(values, first, "time_lst", state.lastStr);
  addString(values, first, "site_long", state.longitudeStr);
  addString(values, first, "site_lat", state.latitudeStr);
  addString(values, first, "align_progress", state.alignProgress);
  addBool(selected, firstSelected, "trk_on", status.tracking);
  addBool(selected, firstSelected, "trk_off", !status.tracking);
  addBool(selected, firstSelected, "auto_on", status.autoHome);
  addBool(selected, firstSelected, "auto_off", !status.autoHome);
  addBool(enabled, firstEnabled, "park", !(status.atHome || status.parked || status.parkFail || status.parking));
  addBool(enabled, firstEnabled, "unpark", status.atHome || status.parked);
}

void appendRotatorState(String& values, bool& first, String& selected, bool& firstSelected) {
  state.updateRotator(true);
  addString(values, first, "rot_sta", state.rotatorSlewing ? "Active" : "Inactive");
  addString(values, first, "rotatorpos", state.rotatorPositionStr);
  addString(values, first, "rot_rate", state.rotateSlewSpeedStr);
  addBool(selected, firstSelected, "rot_on", state.rotatorDerotate);
  addBool(selected, firstSelected, "rot_off", !state.rotatorDerotate);
  addBool(selected, firstSelected, "rot_rev", state.rotatorDerotateReverse);
}

void appendFocuserState(String& values, bool& first, String& selected, bool& firstSelected) {
  state.updateFocuser(true);
  addNumberString(values, first, "focuser_selected", state.focuserSelected);
  addString(values, first, "focuserpos", state.focuserPositionStr);
  addString(values, first, "foc_sta", state.focuserSlewing ? "Active" : "Inactive");
  addString(values, first, "foc_rate", state.focuserSlewSpeedStr);
  addString(values, first, "f_temp", state.focuserTemperatureStr);
  addString(values, first, "f_bl", state.focuserBacklashStr);
  addString(values, first, "f_tcf_db", state.focuserDeadbandStr);
  addString(values, first, "f_tcf_coef", state.focuserTcfCoefStr);
  addString(values, first, "f_tcf_en", state.focuserTcfEnable ? "true" : "false");
  for (int index = 1; index <= status.focuserCount; index++) {
    char key[24];
    snprintf(key, sizeof(key), "foc%d_sel", index);
    addBool(selected, firstSelected, key, state.focuserSelected == index);
  }
}

void appendAuxiliaryState(String& values, bool& first, String& selected, bool& firstSelected) {
  state.updateAuxiliary(true, true);
  for (int index = 0; index < 8; index++) {
    state.selectFeature(index);
    if (!state.featurePurpose()) continue;
    char key[16];
    snprintf(key, sizeof(key), "x%dv1", index + 1);
    addNumberString(values, first, key, state.featureValue1());
    snprintf(key, sizeof(key), "sw%d_on", index + 1);
    addBool(selected, firstSelected, key, state.featureValue1() != 0);
    snprintf(key, sizeof(key), "sw%d_off", index + 1);
    addBool(selected, firstSelected, key, state.featureValue1() == 0);
  }
}

void appendEncoderState(String& values, bool& first, String& selected, bool& firstSelected) {
  #if ENCODERS == ON
    state.updateEncoders(true);
    addString(values, first, "diff_a1", state.diffAngleAxis1Str);
    addString(values, first, "enc_a1", state.encAngleAxis1Str);
    addString(values, first, "raw_a1", state.angleAxis1Str);
    addString(values, first, "diff_a2", state.diffAngleAxis2Str);
    addString(values, first, "enc_a2", state.encAngleAxis2Str);
    addString(values, first, "raw_a2", state.angleAxis2Str);
    addBool(selected, firstSelected, "enc_as_on", encoders.settings.autoSync);
    addBool(selected, firstSelected, "enc_as_off", !encoders.settings.autoSync);
  #else
    UNUSED(values);
    UNUSED(first);
    UNUSED(selected);
    UNUSED(firstSelected);
  #endif
}

String ipString(const uint8_t ip[4]) {
  char text[20];
  snprintf(text, sizeof(text), "%u.%u.%u.%u", ip[0], ip[1], ip[2], ip[3]);
  return String(text);
}

}

void handleApiV1Capabilities() {
  handleUiConfig();
}

void handleApiV1State() {
  String scope = www.arg("scope");
  String json;
  json.reserve(scope == "auxiliary" ? 1600 : 1200);
  beginState(json, scope);
  bool firstValue = true;
  String selected = F("\"selected\":{");
  String enabled = F("\"enabled\":{");
  bool firstSelected = true;
  bool firstEnabled = true;

  if (scope == "controller") appendControllerState(json, firstValue); else
  if (scope == "mount") appendMountState(json, firstValue, selected, firstSelected, enabled, firstEnabled); else
  if (scope == "rotator") appendRotatorState(json, firstValue, selected, firstSelected); else
  if (scope == "focuser") appendFocuserState(json, firstValue, selected, firstSelected); else
  if (scope == "auxiliary") appendAuxiliaryState(json, firstValue, selected, firstSelected); else
  if (scope == "encoders") appendEncoderState(json, firstValue, selected, firstSelected); else {
    sendResult(400, false, "Unknown state scope");
    return;
  }

  json.concat(F("},"));
  selected.concat(F("},"));
  enabled.concat(F("}}"));
  json.concat(selected);
  json.concat(enabled);
  www.sendHeader("Cache-Control", "no-store");
  www.send(200, "application/json", json);
}

void handleApiV1Actions() {
  if (!status.onStepFound || status.getVersionPatch() == 0) {
    sendResult(503, false, "Controller unavailable");
    return;
  }
  String action = www.arg("action");
  if (!dispatchAction(action)) {
    sendResult(501, false, "Action requires legacy compatibility path");
    return;
  }
  sendResult(200, true);
}

void handleApiV1NetworkGet() {
  String json;
  json.reserve(480);
  json.concat(F("{\"schemaVersion\":1,\"authenticated\":"));
  json.concat(loginRequired ? F("false") : F("true"));
  json.concat(F(",\"restartRequired\":"));
  json.concat(restartRequired ? F("true") : F("false"));
  json.concat(F(",\"station\":{"));
  bool first = true;
  addBool(json, first, "enabled", wifiManager.settings.stationEnabled);
  addString(json, first, "ssid", wifiManager.sta->ssid);
  addBool(json, first, "dhcp", wifiManager.sta->dhcpEnabled);
  String stationIp = ipString(wifiManager.sta->ip);
  addString(json, first, "ip", stationIp.c_str());
  json.concat(F("},\"accessPoint\":{"));
  first = true;
  addBool(json, first, "enabled", wifiManager.settings.accessPointEnabled);
  addString(json, first, "ssid", wifiManager.settings.ap.ssid);
  addNumber(json, first, "channel", wifiManager.settings.ap.channel);
  String accessPointIp = ipString(wifiManager.settings.ap.ip);
  addString(json, first, "ip", accessPointIp.c_str());
  json.concat(F("}}"));
  www.sendHeader("Cache-Control", "no-store");
  www.send(200, "application/json", json);
}

void handleApiV1NetworkPost() {
  String action = www.arg("action");
  if (action == "login") {
    loginRequired = strcmp(wifiManager.settings.masterPassword, www.arg("password").c_str()) != 0;
    sendResult(loginRequired ? 401 : 200, !loginRequired, loginRequired ? "Invalid password" : "");
    return;
  }
  if (action == "logout") {
    loginRequired = true;
    sendResult(200, true);
    return;
  }
  if (loginRequired) {
    sendResult(401, false, "Login required");
    return;
  }
  if (action == "save") {
    if (www.hasArg("stationEnabled")) wifiManager.settings.stationEnabled = www.arg("stationEnabled") == "true";
    if (www.hasArg("stationSsid")) sstrcpy(wifiManager.sta->ssid, www.arg("stationSsid").c_str());
    if (www.hasArg("stationPassword") && www.arg("stationPassword").length()) sstrcpy(wifiManager.staPwd->password, www.arg("stationPassword").c_str());
    if (www.hasArg("stationDhcp")) wifiManager.sta->dhcpEnabled = www.arg("stationDhcp") == "true";
    if (www.hasArg("accessPointEnabled")) wifiManager.settings.accessPointEnabled = www.arg("accessPointEnabled") == "true";
    if (www.hasArg("accessPointSsid")) sstrcpy(wifiManager.settings.ap.ssid, www.arg("accessPointSsid").c_str());
    if (www.hasArg("accessPointPassword") && www.arg("accessPointPassword").length()) sstrcpy(wifiManager.settings.ap.pwd, www.arg("accessPointPassword").c_str());
    if (www.hasArg("accessPointChannel")) wifiManager.settings.ap.channel = constrain(www.arg("accessPointChannel").toInt(), 1, 14);
    wifiManager.writeSettings();
    restartRequired = true;
    sendResult(200, true);
    return;
  }
  if (action == "reboot") {
    sendResult(200, true);
    delay(1000);
    HAL_RESET();
    return;
  }
  sendResult(400, false, "Unknown network action");
}

#endif
