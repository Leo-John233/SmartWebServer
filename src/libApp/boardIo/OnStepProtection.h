#pragma once
#include "ProtectionPort.h"

class OnStepProtection : public ProtectionPort {
public:
  ProtectionFeedback sample() override;
  ProtectionStopResult stop() override;
  static bool decodeDriver(const char *reply, ProtectionFeedback &feedback);
};

extern OnStepProtection onStepProtection;
