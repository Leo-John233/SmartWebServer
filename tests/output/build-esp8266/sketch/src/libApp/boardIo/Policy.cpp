#line 1 "E:\\GitHub\\SmartWebServer\\legacy\\SmartWebServer\\src\\libApp\\boardIo\\Policy.cpp"
#include "Policy.h"
#include <cstring>

// GXU1 的七个逗号分隔字段及末尾故障码；不把超时、0 或任意非空文本当作正常。
static bool readDriverReply(const char *reply, bool &fault, bool &standstill) {
  const char *tokens[] = {"ST", "OA", "OB", "GA", "GB", "OT", "PW", "GF"};
  bool flags[8] = {};
  const char *field = reply;
  for (int i = 0; i < 8; i++) {
    const char *end = std::strchr(field, ',');
    if ((i < 7 && !end) || (i == 7 && end)) return false;
    const size_t length = end ? size_t(end - field) : std::strlen(field);
    if (length && (length != 2 || std::strncmp(field, tokens[i], 2))) return false;
    flags[i] = length != 0;
    if (end) field = end + 1;
  }
  // 原 SWS 把 ST 到 PW 全置位视为驱动通信失败，不可作为有效静止反馈。
  if (flags[0] && flags[1] && flags[2] && flags[3] && flags[4] && flags[5] && flags[6]) return false;
  // ST 只表示静止；其余任一故障位均触发停止流程，包括静止时的开路。
  fault = false;
  for (int i = 1; i < 8; i++) fault = fault || flags[i];
  standstill = flags[0];
  return true;
}

void BoardIoPolicy::clearFaultCycle() {
  faultLatched_ = false;
  stopAttempted_ = false;
  stopSent_ = false;
  standstillSeen_ = false;
  stopped_ = false;
}

void BoardIoPolicy::invalidateFeedback() {
  validFeedback_ = false;
  healthy_ = false;
  ready_ = false;
  fault_ = false;
  clearFaultCycle();
}

void BoardIoPolicy::configure(bool activity, bool protection, uint32_t now) {
  if (!configured_ || protection_ != protection) {
    feedbackSeen_ = false;
    invalidateFeedback();
    healthyAt_ = now;
  }
  configured_ = true;
  activity_ = activity;
  protection_ = protection;
}

void BoardIoPolicy::traffic(uint32_t now) {
  trafficSeen_ = true;
  trafficAt_ = now;
}

void BoardIoPolicy::driverFeedback(bool received, const char *reply, uint32_t now) {
  bool fault = false, standstill = false;
  const bool valid = received && reply && readDriverReply(reply, fault, standstill);
  if (!validFeedback_ || uint32_t(now - feedbackAt_) > FeedbackStaleMs) invalidateFeedback();
  feedbackAt_ = now;
  feedbackSeen_ = true;
  if (!valid) { invalidateFeedback(); return; }
  validFeedback_ = true;
  fault_ = fault;
  const bool healthy = !fault;
  if (!healthy_ || !healthy) {
    healthyAt_ = now;
    ready_ = false;
  }
  healthy_ = healthy;
  if (healthy && uint32_t(now - healthyAt_) >= ReadyMs) ready_ = true;
  if (fault) faultLatched_ = true;
  if (!faultLatched_) return;

  // 只统计 STOP 发送之后的新反馈，至少两帧 ST 持续一个采样周期才允许抱闸。
  const bool wasStopped = stopped_;
  if (stopSent_ && standstill) {
    if (!standstillSeen_) { standstillSeen_ = true; standstillAt_ = now; }
    else if (uint32_t(now - standstillAt_) >= PollIntervalMs) stopped_ = true;
  } else {
    standstillSeen_ = false;
    stopped_ = false;
  }
  if (!wasStopped && stopped_) { healthyAt_ = now; ready_ = false; }
  // 完成停稳抱闸后，再由持续正常反馈恢复；停止发送失败则只允许正常恢复。
  if (healthy && ready_ && (stopped_ || !stopSent_)) clearFaultCycle();
}

void BoardIoPolicy::checkFeedback(bool connected, uint32_t now) {
  if (!connected || (feedbackSeen_ && uint32_t(now - feedbackAt_) > FeedbackStaleMs)) invalidateFeedback();
}

bool BoardIoPolicy::stopRequired() const {
  return configured_ && protection_ && validFeedback_ && fault_ && !stopAttempted_;
}

void BoardIoPolicy::stopResult(bool sent) {
  if (!protection_ || !validFeedback_ || !faultLatched_) return;
  stopAttempted_ = true;
  stopSent_ = sent;
  standstillSeen_ = false;
  stopped_ = false;
}

bool BoardIoPolicy::ledLit(bool connected, bool originalIdleLit, uint32_t now) const {
  if (!connected) return (now / 400) % 2 == 0;
  if (!activity_) return originalIdleLit;
  // 连续数据也保留熄灭间隙，避免频繁刷新时变成常亮。
  return trafficSeen_ && uint32_t(now - trafficAt_) < 180 && (now / 120) % 2 == 0;
}

BrakeState BoardIoPolicy::brakeState(bool connected, uint32_t now) const {
  if (!configured_) return BrakeState::Starting;
  if (!protection_) return BrakeState::Disabled;
  if (!connected || !feedbackSeen_) return BrakeState::Waiting;
  if (uint32_t(now - feedbackAt_) > FeedbackStaleMs) return BrakeState::Stale;
  if (!validFeedback_) return BrakeState::Waiting;
  if (faultLatched_) {
    if (stopAttempted_ && !stopSent_) return BrakeState::StopFailed;
    return stopped_ ? BrakeState::Fault : BrakeState::Stopping;
  }
  if (!ready_) return BrakeState::Waiting;
  return BrakeState::Released;
}

bool BoardIoPolicy::releaseBrake(bool connected, uint32_t now) const {
  const BrakeState state = brakeState(connected, now);
  // 自动保护运行时无有效反馈保持松闸，仅故障流程确认静止后才抱闸。
  return state != BrakeState::Starting && state != BrakeState::Disabled && state != BrakeState::Fault;
}

bool BoardIoPolicy::feedbackDue(uint32_t now) const {
  // 无驱动/不支持反馈时降低探测频率，避免串口超时占满命令服务时间。
  const uint32_t interval = validFeedback_ ? PollIntervalMs : FeedbackStaleMs;
  return protection_ && (!feedbackSeen_ || uint32_t(now - feedbackAt_) >= interval);
}
