#pragma once
#include "../../Common.h"
#include "Config.h"
#include "Policy.h"
#include "Activity.h"

class BoardIo {
public:
  void initPins();
  void begin();
  void tick();
  void networkActivity();
  void pollDriver();
  bool setProtectionPort(ProtectionPort &port);
  bool resetFault();
  bool unlockManually();
  bool retryStop();
  bool save(bool activity, bool protection);
  bool ownsPin(int pin) const;
  bool activityAvailable() const { return SWS_ACTIVITY_LED_PIN != OFF; }
  bool brakeAvailable() const { return RA_BRAKE_PROTECTION == ON && SWS_RA_BRAKE_PIN != OFF; }
  const BoardIoPolicy &policy() const { return policy_; }
  BrakeState brakeState() const;

private:
  void updateDriverTaskInterval();
  BoardIoPolicy policy_;
  ProtectionPort *port_ = nullptr;
  uint8_t driverTask_ = 0;
  uint32_t driverTaskIntervalMs_ = 0;
  bool started_ = false;
  bool startFailed_ = false;
  bool servicing_ = false;
  bool driverTaskKicked_ = false;
  int ledLevel_ = -1, brakeLevel_ = -1;
};

extern BoardIo boardIo;
