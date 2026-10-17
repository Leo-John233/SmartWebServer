#pragma once
#include <stdint.h>
#include "ProtectionPort.h"

enum class BrakeState { Starting, StartFailed, Disabled, Waiting, Stopping, StopFailed, Fault, Stale, Released, Manual };
enum class BrakeFault : uint8_t { None, Driver, Feedback, Disconnected, Stale, ProtectionOff };

// 独立保护策略，不依赖串口协议、GPIO、网页或存储
class BoardIoPolicy {
public:
  enum : uint32_t { PollIntervalMs = 250, FeedbackStaleMs = 1000, ReadyMs = 500 };
  void configure(bool activity, bool protection, uint32_t now);
  void traffic(uint32_t now);
  void feedback(const ProtectionFeedback &feedback, uint32_t now);
  void checkFeedback(bool connected, uint32_t now);
  void advanceRelease(uint32_t now);
  void unlockManually();
  bool stopRequired() const;
  void stopResult(const ProtectionStopResult &result, uint32_t now);
  bool retryStop();
  bool resetEligible(uint32_t now) const;
  bool resetFault(uint32_t now);
  bool ledLit(bool connected, bool originalIdleLit, uint32_t now) const;
  BrakeState brakeState(bool connected, uint32_t now) const;
  bool releaseBrake() const { return released_; }
  bool motionAllowed() const { return running_ && (protection_ || manual_) && reason_ == BrakeFault::None; }
  bool manuallyUnlocked() const { return manual_; }
  bool activityEnabled() const { return activity_; }
  bool protectionEnabled() const { return protection_; }
  BrakeFault faultReason() const { return reason_; }
  const ProtectionStopResult &lastStopResult() const { return stopResult_; }
  bool feedbackDue(uint32_t now) const;

private:
  void latch(BrakeFault reason);
  void invalidate();
  void clearStop();
  bool fresh(uint32_t now) const;
  bool configured_ = false, activity_ = true, protection_ = true;
  bool trafficSeen_ = false, feedbackSeen_ = false, healthy_ = false, ready_ = false;
  bool released_ = false, running_ = false;
  bool manual_ = false;
  bool stopAttempted_ = false, standstillSeen_ = false;
  BrakeFault reason_ = BrakeFault::None;
  ProtectionFeedback feedback_;
  ProtectionStopResult stopResult_;
  uint32_t trafficAt_ = 0, feedbackAt_ = 0, healthyAt_ = 0, standstillAt_ = 0, stopAt_ = 0;
};
