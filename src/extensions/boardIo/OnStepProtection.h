#pragma once
#include <stddef.h>
#include "ProtectionPort.h"

class OnStepProtection : public ProtectionPort {
public:
  enum : uint32_t { ReplyCacheMs = 1500 };
  ProtectionFeedback sample() override;
  ProtectionStopResult stop() override;
  bool latestDriverReply(char *reply, size_t capacity) const;
  void recordDriverQuery(const char *reply, bool received, uint32_t sampledAt);
  static bool decodeDriver(const char *reply, ProtectionFeedback &feedback);

private:
  char driverReply_[80] = "";
  uint32_t sampledAt_ = 0;
  uint32_t consumedAt_ = 0;
  bool driverReceived_ = false, driverValid_ = false, consumedSeen_ = false, queryRecorded_ = false;
};

extern OnStepProtection onStepProtection;
