#include "BoardIo.h"
#include "View.h"
#include "../../libApp/boardIo/BoardIo.h"

static const char *brakeMessage() {
  switch (boardIo.brakeState()) {
    case BrakeState::Starting: return L_IO_BRAKE_STARTING;
    case BrakeState::Disabled: return L_IO_BRAKE_DISABLED;
    case BrakeState::Waiting: return L_IO_BRAKE_WAITING;
    case BrakeState::Fault: return L_IO_BRAKE_FAULT;
    case BrakeState::Stale: return L_IO_BRAKE_STALE;
    case BrakeState::Released: return L_IO_BRAKE_RELEASED;
  }
  return L_UNKNOWN;
}

void boardIoTile(String &data) {
  if (!boardIo.activityAvailable() && !boardIo.brakeAvailable()) return;
  char buffer[1200];
  data.concat(FPSTR(html_board_io_begin));
  if (boardIo.activityAvailable()) {
    const char *enabled = boardIo.policy().activityEnabled() ? "true" : "false";
    snprintf_P(buffer, sizeof(buffer), html_board_io_activity, enabled, enabled);
    data.concat(buffer);
  }
  if (boardIo.brakeAvailable()) {
    const char *enabled = boardIo.policy().protectionEnabled() ? "true" : "false";
    snprintf_P(buffer, sizeof(buffer), html_board_io_brake, enabled, enabled, brakeMessage());
    data.concat(buffer);
  }
  data.concat(F("<p class='sws-io-error' role='alert'></p></section>"));
}

void boardIoTileAjax(String &data) {
  if (boardIo.activityAvailable()) {
    data.concat(F("sws-io-activity|"));
    data.concat(boardIo.policy().activityEnabled() ? "selected\n" : "unselected\n");
  }
  if (boardIo.brakeAvailable()) {
    data.concat(F("sws-io-protection|"));
    data.concat(boardIo.policy().protectionEnabled() ? "selected\n" : "unselected\n");
    data.concat(F("sws-io-brake-state|"));
    data.concat(brakeMessage());
    data.concat("\n");
  }
}

// 独立的 SWS 设置接口，不把本机 IO 设置传给 OnStep，也不复用运动命令。
void handleBoardIo() {
  www.sendHeader("Cache-Control", "no-store");
  if (www.method() == HTTP_POST) {
    const String setting = www.arg("setting"), value = www.arg("enabled");
    if ((setting != "activity" && setting != "protection") || (value != "0" && value != "1")) {
      www.send(400, "application/json", "{}"); return;
    }
    if ((setting == "activity" && !boardIo.activityAvailable()) ||
        (setting == "protection" && !boardIo.brakeAvailable())) {
      www.send(409, "application/json", "{}"); return;
    }
    const bool activity = setting == "activity" ? value == "1" : boardIo.policy().activityEnabled();
    const bool protection = setting == "protection" ? value == "1" : boardIo.policy().protectionEnabled();
    if (!boardIo.save(activity, protection)) { www.send(500, "application/json", "{}"); return; }
  } else if (www.method() != HTTP_GET) {
    www.send(405, "application/json", "{}"); return;
  }
  // 消息先作为 JSON 字符串转义，避免译文中的引号破坏响应。
  String message = brakeMessage();
  message.replace("\\", "\\\\"); message.replace("\"", "\\\"");
  String result = "{\"activity\":";
  result.concat(boardIo.policy().activityEnabled() ? "true" : "false");
  result.concat(",\"protection\":");
  result.concat(boardIo.policy().protectionEnabled() ? "true" : "false");
  result.concat(",\"message\":\""); result.concat(message); result.concat("\"}");
  www.send(200, "application/json; charset=utf-8", result);
}
