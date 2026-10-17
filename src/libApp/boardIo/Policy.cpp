#include "Policy.h"

void BoardIoPolicy::clearStop() {
  stopAttempted_ = standstillSeen_ = false;
  stopResult_ = ProtectionStopResult();
  stopAt_ = standstillAt_ = 0;
}

void BoardIoPolicy::latch(BrakeFault reason) {
  if (reason_ != BrakeFault::None) return;
  reason_ = reason;
  running_ = false;
  clearStop();
}

void BoardIoPolicy::invalidate() { healthy_ = ready_ = standstillSeen_ = false; }

void BoardIoPolicy::configure(bool activity, bool protection, uint32_t now) {
  if (manual_ && protection) { manual_ = false; running_ = false; }
  if (configured_ && protection_ && !protection) latch(BrakeFault::ProtectionOff);
  if (!configured_ || protection_ != protection) { invalidate(); healthyAt_ = now; }
  configured_ = true;
  activity_ = activity;
  protection_ = protection;
}

void BoardIoPolicy::traffic(uint32_t now) { trafficSeen_ = true; trafficAt_ = now; }

bool BoardIoPolicy::fresh(uint32_t now) const {
  return feedbackSeen_ && feedback_.driverValid && uint32_t(now - feedback_.sampledAt) <= FeedbackStaleMs;
}

void BoardIoPolicy::feedback(const ProtectionFeedback &value, uint32_t now) {
  // 相同或更早的采样不能算作另一帧确认；时间差计算兼容时钟回绕
  if (feedbackSeen_ && int32_t(value.sampledAt - feedback_.sampledAt) <= 0) return;
  if (manual_) {
    feedback_ = value;
    feedbackAt_ = now;
    feedbackSeen_ = true;
    return;
  }
  if (!fresh(now)) {
    if (released_) latch(BrakeFault::Stale);
    invalidate();
  }
  feedback_ = value;
  feedbackAt_ = now;
  feedbackSeen_ = true;
  if (!fresh(now)) {
    invalidate();
    if (released_) latch(BrakeFault::Feedback);
    return;
  }
  if (value.driverFault) latch(BrakeFault::Driver);
  const bool healthy = !value.driverFault && (reason_ == BrakeFault::None || value.standstill);
  if (!healthy_ || !healthy) { healthyAt_ = now; ready_ = false; }
  healthy_ = healthy;
  if (healthy && uint32_t(now - healthyAt_) >= ReadyMs) ready_ = true;

  // 正常运行时单独 ST 不触发抱闸停止命令完成后才统计新鲜 ST；
  // 无效或运动反馈会清除未完成的确认，不能把命令发送成功当成停稳
  if (reason_ != BrakeFault::None && released_ && stopAttempted_ && stopResult_.complete() &&
      int32_t(value.sampledAt - stopAt_) > 0 && value.standstill) {
    if (!standstillSeen_) { standstillSeen_ = true; standstillAt_ = value.sampledAt; }
    else if (uint32_t(value.sampledAt - standstillAt_) >= PollIntervalMs) {
      released_ = false;
    }
  } else standstillSeen_ = false;
}

void BoardIoPolicy::checkFeedback(bool connected, uint32_t now) {
  if (manual_) return;
  if (!connected || !fresh(now)) {
    invalidate();
    if (released_) latch(!connected ? BrakeFault::Disconnected : BrakeFault::Stale);
  }
  // 失联不修改 released_，保持当前 GPIO 电平
}

void BoardIoPolicy::advanceRelease(uint32_t now) {
  if (!protection_ || reason_ != BrakeFault::None || !fresh(now) || !ready_) return;
  released_ = running_ = true;
}

void BoardIoPolicy::unlockManually() {
  // 手动解锁立即松闸，暂停本次运行的自动逻辑并清除旧停止记录
  manual_ = true;
  protection_ = false;
  reason_ = BrakeFault::None;
  clearStop();
  invalidate();
  released_ = running_ = true;
}

bool BoardIoPolicy::stopRequired() const { return reason_ != BrakeFault::None && !stopAttempted_; }

void BoardIoPolicy::stopResult(const ProtectionStopResult &result, uint32_t now) {
  if (reason_ == BrakeFault::None) return;
  stopAttempted_ = true;
  stopResult_ = result;
  stopAt_ = now;
  standstillSeen_ = false;
}

bool BoardIoPolicy::retryStop() {
  if (reason_ == BrakeFault::None) return false;
  clearStop();
  return true;
}

bool BoardIoPolicy::resetEligible(uint32_t now) const {
  return protection_ && reason_ != BrakeFault::None && !released_ && fresh(now) && ready_ &&
         !feedback_.driverFault && feedback_.standstill;
}

bool BoardIoPolicy::resetFault(uint32_t now) {
  if (!resetEligible(now)) return false;
  reason_ = BrakeFault::None;
  clearStop();
  released_ = running_ = true;
  return true;
}

bool BoardIoPolicy::ledLit(bool connected, bool originalIdleLit, uint32_t now) const {
  if (!connected) return (now / 400) % 2 == 0;
  if (!activity_) return originalIdleLit;
  return trafficSeen_ && uint32_t(now - trafficAt_) < 180 && (now / 120) % 2 == 0;
}

BrakeState BoardIoPolicy::brakeState(bool connected, uint32_t now) const {
  if (!configured_) return BrakeState::Starting;
  if (manual_) return BrakeState::Manual;
  if (reason_ != BrakeFault::None && !released_) return BrakeState::Fault;
  if (!connected || (feedbackSeen_ && !fresh(now))) return BrakeState::Stale;
  if (reason_ != BrakeFault::None) return stopAttempted_ && !stopResult_.complete() ? BrakeState::StopFailed : BrakeState::Stopping;
  if (!protection_) return BrakeState::Disabled;
  if (!ready_) return BrakeState::Waiting;
  return running_ ? BrakeState::Released : BrakeState::Waiting;
}

bool BoardIoPolicy::feedbackDue(uint32_t now) const {
  return !feedbackSeen_ || uint32_t(now - feedbackAt_) >= PollIntervalMs;
}
