#include "Policy.h"
#include <cstring>

// GXU1 的七个逗号分隔字段及末尾故障码；不把超时、0 或任意非空文本当作正常。
static bool healthyDriverReply(const char *reply) {
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
  // 静止时的开路位可能是驱动芯片的无效诊断，沿用 SWS 原有显示判据。
  return !flags[3] && !flags[4] && !flags[5] && !flags[6] && !flags[7] &&
         (flags[0] || (!flags[1] && !flags[2]));
}

void BoardIoPolicy::configure(bool activity, bool protection, uint32_t now) {
  if (!configured_ || (!protection_ && protection)) {
    feedbackSeen_ = false;
    healthy_ = false;
    ready_ = false;
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
  const bool healthy = received && reply && healthyDriverReply(reply);
  if (!healthy_ || !feedbackSeen_ || uint32_t(now - feedbackAt_) > FeedbackStaleMs) {
    healthyAt_ = now;
    ready_ = false;
  }
  feedbackAt_ = now;
  feedbackSeen_ = true;
  healthy_ = healthy;
  // 必须由后续正常反馈确认稳定，不能收到一帧后仅靠等待就释放。
  if (!healthy) ready_ = false;
  else if (uint32_t(now - healthyAt_) >= ReadyMs) ready_ = true;
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
  if (!healthy_) return BrakeState::Fault;
  if (!ready_) return BrakeState::Waiting;
  return BrakeState::Released;
}

bool BoardIoPolicy::releaseBrake(bool connected, uint32_t now) const {
  const BrakeState state = brakeState(connected, now);
  return state == BrakeState::Disabled || state == BrakeState::Released;
}

bool BoardIoPolicy::feedbackDue(uint32_t now) const {
  return protection_ && (!feedbackSeen_ || uint32_t(now - feedbackAt_) >= PollIntervalMs);
}
