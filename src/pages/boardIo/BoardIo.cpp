#include "BoardIo.h"
#include "View.h"
#include "../../libApp/boardIo/BoardIo.h"

static const char *brakeMessage() {
  switch (boardIo.brakeState()) {
    case BrakeState::Starting: return L_IO_BRAKE_STARTING;
    case BrakeState::StartFailed: return L_IO_BRAKE_START_FAILED;
    case BrakeState::Disabled: return L_IO_BRAKE_DISABLED;
    case BrakeState::Waiting: return L_IO_BRAKE_WAITING;
    case BrakeState::Stopping: return L_IO_BRAKE_STOPPING;
    case BrakeState::StopFailed: return L_IO_BRAKE_STOP_FAILED;
    case BrakeState::Fault: return L_IO_BRAKE_FAULT;
    case BrakeState::Stale: return L_IO_BRAKE_STALE;
    case BrakeState::Released: return L_IO_BRAKE_RELEASED;
  }
  return L_UNKNOWN;
}

static const char *faultMessage() {
  switch (boardIo.policy().faultReason()) {
    case BrakeFault::None: return "";
    case BrakeFault::Driver: return L_IO_FAULT_DRIVER;
    case BrakeFault::Feedback: return L_IO_FAULT_FEEDBACK;
    case BrakeFault::Disconnected: return L_IO_FAULT_DISCONNECTED;
    case BrakeFault::Stale: return L_IO_FAULT_STALE;
    case BrakeFault::ProtectionOff: return L_IO_FAULT_OFF;
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
    const char *locked = boardIo.policy().faultReason() != BrakeFault::None ? "true" : "false";
    const bool released = boardIo.policy().releaseBrake();
    snprintf_P(buffer, sizeof(buffer), html_board_io_brake, enabled, enabled, brakeMessage(), faultMessage(), locked,
               released ? "false" : "true", released ? "true" : "false");
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
    data.concat(F("sws-io-fault|")); data.concat(faultMessage()); data.concat("\n");
    data.concat(F("sws-io-actions|"));
    data.concat(boardIo.policy().faultReason() != BrakeFault::None ? "selected\n" : "unselected\n");
    data.concat(F("sws-io-reset|"));
    data.concat(boardIo.policy().releaseBrake() ? "unselected\n" : "selected\n");
    data.concat(F("sws-io-retry-stop|"));
    data.concat(boardIo.policy().releaseBrake() ? "selected\n" : "unselected\n");
  }
}

// 独立的 SWS 设置接口，不把本机 IO 设置传给 OnStep，也不复用运动命令
void handleBoardIo() {
  www.sendHeader("Cache-Control", "no-store");
  if (www.method() == HTTP_POST) {
    const String action = www.arg("action");
    if (action != "") {
      if (!boardIo.brakeAvailable() || (action != "reset" && action != "retry-stop")) {
        www.send(400, "application/json", "{}"); return;
      }
      const bool accepted = action == "reset" ? boardIo.resetFault() : boardIo.retryStop();
      if (!accepted) { www.send(409, "application/json", "{}"); return; }
    } else {
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
    }
  } else if (www.method() != HTTP_GET) {
    www.send(405, "application/json", "{}"); return;
  }
  // 消息先作为 JSON 字符串转义，避免译文中的引号破坏响应
  String message = brakeMessage();
  message.replace("\\", "\\\\"); message.replace("\"", "\\\"");
  String fault = faultMessage();
  fault.replace("\\", "\\\\"); fault.replace("\"", "\\\"");
  String result = "{\"activity\":";
  result.concat(boardIo.policy().activityEnabled() ? "true" : "false");
  result.concat(",\"protection\":");
  result.concat(boardIo.policy().protectionEnabled() ? "true" : "false");
  result.concat(",\"locked\":");
  result.concat(boardIo.policy().faultReason() != BrakeFault::None ? "true" : "false");
  result.concat(",\"showUnlock\":"); result.concat(boardIo.policy().releaseBrake() ? "false" : "true");
  result.concat(",\"showStopRetry\":"); result.concat(boardIo.policy().releaseBrake() ? "true" : "false");
  result.concat(",\"message\":\""); result.concat(message);
  result.concat("\",\"fault\":\""); result.concat(fault);
  result.concat("\",\"haltSent\":"); result.concat(boardIo.policy().lastStopResult().haltSent ? "true" : "false");
  result.concat(",\"trackingOffAccepted\":"); result.concat(boardIo.policy().lastStopResult().trackingOffAccepted ? "true" : "false");
  result.concat("}");
  www.send(200, "application/json; charset=utf-8", result);
}
