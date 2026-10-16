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
  void driverFeedback(bool received, const char *reply);
  bool save(bool activity, bool protection);
  bool ownsPin(int pin) const;
  bool activityAvailable() const { return SWS_ACTIVITY_LED_PIN != OFF; }
  bool brakeAvailable() const { return RA_BRAKE_PROTECTION == ON && SWS_RA_BRAKE_PIN != OFF; }
  const BoardIoPolicy &policy() const { return policy_; }
  BrakeState brakeState() const;

private:
  BoardIoPolicy policy_;
  bool started_ = false;
  int ledLevel_ = -1, brakeLevel_ = -1;
};

extern BoardIo boardIo;
