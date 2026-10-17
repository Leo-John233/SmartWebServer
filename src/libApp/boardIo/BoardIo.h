#pragma once
#include "../../Common.h"
#include "Config.h"
#include "Policy.h"
#include "Activity.h"
#include "CommandGate.h"

class BoardIo {
public:
  void initPins();
  void begin();
  void tick();
  void networkActivity();
  void pollDriver();
  bool setProtectionPort(ProtectionPort &port);
  bool resetFault();
  bool retryStop();
  bool motionAllowed();
  bool faultSaved() const { return faultSaved_; }
  bool save(bool activity, bool protection);
  bool ownsPin(int pin) const;
  bool activityAvailable() const { return SWS_ACTIVITY_LED_PIN != OFF; }
  bool brakeAvailable() const { return RA_BRAKE_PROTECTION == ON && SWS_RA_BRAKE_PIN != OFF; }
  const BoardIoPolicy &policy() const { return policy_; }
  BrakeState brakeState() const;

private:
  BoardIoPolicy policy_;
  ProtectionPort *port_ = nullptr;
  bool started_ = false;
  bool startFailed_ = false;
  bool servicing_ = false, faultSaved_ = true;
  uint8_t savedFault_ = 0;
  uint32_t saveAttemptAt_ = 0;
  int ledLevel_ = -1, brakeLevel_ = -1;
};

extern BoardIo boardIo;
