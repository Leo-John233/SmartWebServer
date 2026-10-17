#include "OnStepProtection.h"
#include "BoardIo.h"
#include "../cmd/Cmd.h"

OnStepProtection onStepProtection;

bool OnStepProtection::decodeDriver(const char *reply, ProtectionFeedback &feedback) {
  feedback.driverValid = feedback.driverFault = feedback.standstill = false;
  if (!reply) return false;
  const char *tokens[] = {"ST", "OA", "OB", "GA", "GB", "OT", "PW", "GF"};
  bool flags[8] = {};
  const char *field = reply;
  for (int i = 0; i < 8; i++) {
    const char *end = strchr(field, ',');
    if ((i < 7 && !end) || (i == 7 && end)) return false;
    const size_t length = end ? size_t(end - field) : strlen(field);
    if (length && (length != 2 || strncmp(field, tokens[i], 2))) return false;
    flags[i] = length != 0;
    if (end) field = end + 1;
  }
  if (flags[0] && flags[1] && flags[2] && flags[3] && flags[4] && flags[5] && flags[6]) return false;
  feedback.driverValid = true;
  feedback.standstill = flags[0];
  for (int i = 1; i < 8; i++) feedback.driverFault |= flags[i];
  return true;
}

ProtectionFeedback OnStepProtection::sample() {
  ProtectionFeedback feedback;
  feedback.sampledAt = millis();
  char reply[80] = "";
  const bool received = onStep.processCommand(":GXU1#", reply, 200);
  const size_t length = strlen(reply);
  if (length && reply[length - 1] == '#') reply[length - 1] = 0;
  if (received) decodeDriver(reply, feedback);
  return feedback;
}

ProtectionStopResult OnStepProtection::stop() {
  ProtectionStopResult result;
  char reply[80] = "";
  result.haltSent = onStep.processCommand(":Q#", reply, 200);
  // 即使 Q 发送失败也尝试关闭跟踪；发送成功不等于实际停止
  const bool received = onStep.processCommand(":Td#", reply, 200);
  result.trackingOffAccepted = received && !strcmp(reply, "1");
  return result;
}
