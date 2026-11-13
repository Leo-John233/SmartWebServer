#include "OnStepProtection.h"
#include "../../libApp/cmd/Cmd.h"

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
  const uint32_t now = millis();
  const bool cacheFresh = driverReceived_ && uint32_t(now - sampledAt_) <= ReplyCacheMs;
  const bool cacheUnconsumed = !consumedSeen_ || int32_t(sampledAt_ - consumedAt_) > 0;
  if (cacheFresh && cacheUnconsumed) {
    feedback.sampledAt = sampledAt_;
    decodeDriver(driverReply_, feedback);
    driverValid_ = feedback.driverValid;
    consumedAt_ = sampledAt_;
    consumedSeen_ = true;
    return feedback;
  }

  driverValid_ = driverReceived_ = false;
  driverReply_[0] = 0;
  sampledAt_ = now;
  queryRecorded_ = false;
  const bool received = onStep.processCommand(":GXU1#", driverReply_, 200);
  if (!queryRecorded_) recordDriverQuery(driverReply_, received, sampledAt_);
  feedback.sampledAt = sampledAt_;
  if (received && driverReceived_) decodeDriver(driverReply_, feedback);
  driverValid_ = feedback.driverValid;
  consumedAt_ = sampledAt_;
  consumedSeen_ = true;
  return feedback;
}

void OnStepProtection::recordDriverQuery(const char *reply, bool received, uint32_t sampledAt) {
  queryRecorded_ = true;
  sampledAt_ = sampledAt;
  driverReceived_ = received && reply;
  driverValid_ = false;
  if (!driverReceived_) { driverReply_[0] = 0; return; }
  if (reply != driverReply_) {
    const size_t length = strlen(reply);
    if (length >= sizeof(driverReply_)) { driverReceived_ = false; driverReply_[0] = 0; return; }
    memcpy(driverReply_, reply, length + 1);
  }
  const size_t length = strlen(driverReply_);
  if (length && driverReply_[length - 1] == '#') driverReply_[length - 1] = 0;
  ProtectionFeedback parsed;
  parsed.sampledAt = sampledAt_;
  driverValid_ = decodeDriver(driverReply_, parsed);
}

bool OnStepProtection::latestDriverReply(char *reply, size_t capacity) const {
  if (!reply || !capacity) return false;
  reply[0] = 0;
  // 页面只使用保护任务的同一份新鲜反馈，采样失败后不能继续显示旧状态
  if (!driverReceived_ || !driverValid_ || uint32_t(millis() - sampledAt_) > ReplyCacheMs || strlen(driverReply_) >= capacity) return false;
  strcpy(reply, driverReply_);
  return true;
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
