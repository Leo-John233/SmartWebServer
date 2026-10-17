#pragma once
#include <stddef.h>
#include "ProtectionPort.h"

class OnStepProtection : public ProtectionPort {
public:
  ProtectionFeedback sample() override;
  ProtectionStopResult stop() override;
  bool latestDriverReply(char *reply, size_t capacity) const;
  static bool decodeDriver(const char *reply, ProtectionFeedback &feedback);

private:
  char driverReply_[80] = "";
  uint32_t sampledAt_ = 0;
  bool driverValid_ = false;
};

extern OnStepProtection onStepProtection;
