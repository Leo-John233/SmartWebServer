#pragma once
#include <stdint.h>

enum class BrakeState { Starting, Disabled, Waiting, Fault, Stale, Released };

// 无串口、网页、存储或 GPIO 依赖，可独立验证输出判据。
class BoardIoPolicy {
public:
  enum : uint32_t { PollIntervalMs = 250, FeedbackStaleMs = 1000, ReadyMs = 500 };
  void configure(bool activity, bool protection, uint32_t now);
  void traffic(uint32_t now);
  void driverFeedback(bool received, const char *reply, uint32_t now);
  bool ledLit(bool connected, bool originalIdleLit, uint32_t now) const;
  BrakeState brakeState(bool connected, uint32_t now) const;
  bool releaseBrake(bool connected, uint32_t now) const;
  bool activityEnabled() const { return activity_; }
  bool protectionEnabled() const { return protection_; }
  bool feedbackDue(uint32_t now) const;

private:
  bool configured_ = false, activity_ = true, protection_ = true;
  bool trafficSeen_ = false, feedbackSeen_ = false, healthy_ = false, ready_ = false;
  uint32_t trafficAt_ = 0, feedbackAt_ = 0, healthyAt_ = 0;
};
