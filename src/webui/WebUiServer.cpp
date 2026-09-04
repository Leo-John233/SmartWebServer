#include "WebUiServer.h"

#if defined(ESP8266) && OPERATIONAL_MODE == WIFI && WEB_SERVER == ON

#include "WebAssets.generated.h"
#include "../pages/Pages.common.h"

namespace {

const size_t WEB_UI_CHUNK_SIZE = 1024;

bool recognizedController() {
  return status.onStepFound && status.getVersionMajor() >= 0 && status.getVersionPatch() != 0;
}

bool versionAtLeast(int major, int minor) {
  return status.getVersionMajor() > major || (status.getVersionMajor() == major && status.getVersionMinor() >= minor);
}

void appendJsonString(String& json, const char* value) {
  json.concat('"');
  while (*value) {
    if (*value == '"' || *value == '\\') json.concat('\\');
    if (static_cast<uint8_t>(*value) >= 0x20) json.concat(*value);
    value++;
  }
  json.concat('"');
}

void appendJsonBool(String& json, bool value) {
  json.concat(value ? F("true") : F("false"));
}

void readAsset(size_t index, webui_assets::WebAsset& asset) {
  memcpy_P(&asset, &webui_assets::assets[index], sizeof(asset));
}

void sendAsset(const webui_assets::WebAsset& asset) {
  String mimeType = FPSTR(asset.mimeType);
  www.setContentLength(asset.compressedLength);
  www.sendHeader("Content-Encoding", "gzip");
  www.sendHeader("Cache-Control", asset.immutable ? "public, max-age=31536000, immutable" : "no-cache");
  www.send(200, mimeType.c_str(), String());

  for (size_t offset = 0; offset < asset.compressedLength; offset += WEB_UI_CHUNK_SIZE) {
    size_t count = min(WEB_UI_CHUNK_SIZE, asset.compressedLength - offset);
    www.sendContent_P(reinterpret_cast<PGM_P>(asset.data + offset), count);
    delay(0);
  }
}

bool sendAssetAtPath(const String& requestedPath) {
  String path = requestedPath;
  if (path == F("/ui") || path == F("/ui/index.html")) path = F(WEB_UI_INDEX_PATH);

  for (size_t index = 0; index < webui_assets::assetCount; index++) {
    webui_assets::WebAsset asset;
    readAsset(index, asset);
    if (strcmp_P(path.c_str(), asset.path) == 0) {
      sendAsset(asset);
      return true;
    }
  }
  return false;
}

}

void handleWebUi() {
  if (!sendAssetAtPath(F(WEB_UI_INDEX_PATH))) www.send(404, "text/plain", "Web UI entry not found");
}

void handleWebUiAsset() {
  if (!sendAssetAtPath(www.uri())) www.send(404, "text/plain", "Web UI asset not found");
}

void handleUiConfig() {
  const bool recognized = recognizedController();
  const int major = status.getVersionMajor();
  const bool mount = recognized && status.mountFound == SD_TRUE;
  char controllerVersion[16] = "";
  char controllerName[16] = "";

  if (recognized) {
    status.getVersionStr(controllerVersion);
    status.getProduct(controllerName);
  }

  String json;
  json.reserve(640);
  json.concat(F("{\"schemaVersion\":1,\"firmwareVersion\":"));
  appendJsonString(json, firmwareVersion.str);
  json.concat(F(",\"controller\":{\"protocolFamily\":"));
  appendJsonString(json, recognized ? (major >= 10 ? "onstepx" : "onstep4") : "unknown");
  json.concat(F(",\"version\":"));
  appendJsonString(json, controllerVersion);
  json.concat(F(",\"name\":"));
  appendJsonString(json, controllerName);
  json.concat(F("},\"locale\":"));
  #if DISPLAY_LANGUAGE == L_cn
    appendJsonString(json, "zh");
  #else
    appendJsonString(json, "en");
  #endif
  json.concat(F(",\"pollIntervalMs\":"));
  json.concat(STATE_POLLING_RATE_MS);
  json.concat(F(",\"capabilities\":{\"mount\":"));
  appendJsonBool(json, mount);
  json.concat(F(",\"rotator\":"));
  appendJsonBool(json, recognized && status.rotatorFound == SD_TRUE);
  json.concat(F(",\"focuserCount\":"));
  json.concat(recognized ? status.focuserCount : 0);
  json.concat(F(",\"auxiliary\":"));
  appendJsonBool(json, recognized && status.auxiliaryFound == SD_TRUE);
  json.concat(F(",\"encoders\":"));
  #if ENCODERS == ON
    appendJsonBool(json, mount);
  #else
    appendJsonBool(json, false);
  #endif
  json.concat(F(",\"axisCount\":"));
  json.concat(recognized ? (major >= 10 ? 9 : 2) : 0);
  json.concat(F(",\"highPrecisionCoordinates\":"));
  appendJsonBool(json, recognized && major > 3);
  json.concat(F(",\"axisConfiguration\":"));
  #if DRIVE_CONFIGURATION == ON
    appendJsonBool(json, mount && versionAtLeast(10, 26));
  #else
    appendJsonBool(json, false);
  #endif
  json.concat(F(",\"autoHome\":"));
  appendJsonBool(json, mount && versionAtLeast(10, 20));
  json.concat(F(",\"meridianFlipHomeModes\":"));
  appendJsonBool(json, mount && status.hasMeridianFlipHomeModes() && (status.mountType == MT_GEM || status.mountType == MT_FORK));
  json.concat(F(",\"workingSlewRate\":"));
  appendJsonBool(json, mount && major >= 10);
  json.concat(F("}}"));

  www.sendHeader("Cache-Control", "no-store");
  www.send(200, "application/json", json);
}

#endif
