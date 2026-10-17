#include "BoardIo.h"
#include "../../lib/nv/Nv.h"
#include "../../lib/tasks/OnTask.h"
#include "../cmd/Cmd.h"
#include "../status/Status.h"
#include "OnStepProtection.h"

BoardIo boardIo;
static void pollBoardOutputs() { boardIo.tick(); }
static void pollBoardDriver() { boardIo.pollDriver(); }
static bool boardMotionAllowed() { return boardIo.motionAllowed(); }

static bool readBoardSetting(const char *key, uint8_t &value) {
  const KvPartition::Status result = nv().kv().get(key, value);
  if (result == KvPartition::Status::Ok) return true;
  if (result != KvPartition::Status::NotFound) return false;
  return nv().kv().put(key, value) == KvPartition::Status::Ok;
}

void BoardIo::initPins() {
  SwsCommandGate::permit() = boardMotionAllowed;
  if (brakeAvailable()) SwsCommandGate::setLocked(true);
  #if SWS_RA_BRAKE_PIN != OFF
    // 先设置输出锁存值再切换方向；编译关闭保护时从初始化开始松闸
    const int initialLevel = brakeAvailable() ? LOW : HIGH;
    digitalWrite(SWS_RA_BRAKE_PIN, initialLevel);
    pinMode(SWS_RA_BRAKE_PIN, OUTPUT);
    brakeLevel_ = initialLevel;
  #endif
}

void BoardIo::begin() {
  if (!activityAvailable() && !brakeAvailable()) return;
  if (!port_) port_ = &onStepProtection;
  uint8_t settings = 3; // 默认开启活动指示和自动保护，不持久保存“已就绪”
  const bool settingsOk = readBoardSetting("BOARD_IO_SETTINGS", settings) && settings <= 3;
  if (!settingsOk) settings = 3;
  policy_.configure(settings & 1, brakeAvailable() && (settings & 2), millis());
  if (brakeAvailable()) {
    uint8_t fault = 0;
    const bool faultOk = readBoardSetting("RA_BRAKE_FAULT", fault) && fault <= uint8_t(BrakeFault::Storage);
    savedFault_ = fault;
    policy_.restoreFault(settingsOk && faultOk ? BrakeFault(fault) : BrakeFault::Storage);
    faultSaved_ = settingsOk && faultOk;
  }
  #if SWS_ACTIVITY_LED_PIN != OFF
    digitalWrite(SWS_ACTIVITY_LED_PIN, HIGH);
    pinMode(SWS_ACTIVITY_LED_PIN, OUTPUT);
    ledLevel_ = HIGH;
  #endif
  // 无法建立输出监控任务时维持初始化电平；启用保护时仍保持启动抱闸
  if (!tasks.add(20, 0, true, 1, pollBoardOutputs, "BoardIO")) { startFailed_ = true; return; }
  // 低于命令通道及网页，缺失驱动的查询不能抢占用户操作
  if (brakeAvailable() && !tasks.add(BoardIoPolicy::PollIntervalMs, 0, true, 4, pollBoardDriver, "RaBrake")) {
    startFailed_ = true;
    return;
  }
  started_ = true;
  tick();
}

void BoardIo::tick() {
  if (!started_ || servicing_) return;
  servicing_ = true;
  const uint32_t now = millis();
  const bool connected = status.onStepFound && status.mountFound == SD_TRUE;
  if (brakeAvailable()) {
    policy_.checkFeedback(connected, now);
    if (!policy_.motionAllowed()) SwsCommandGate::setLocked(true);
    const uint8_t fault = uint8_t(policy_.faultReason());
    if (fault != savedFault_ || !faultSaved_) {
      // 每次事件只保存一次；存储失败最多每秒重试，不重复发送停止命令
      if (faultSaved_ || uint32_t(now - saveAttemptAt_) >= 1000) {
        saveAttemptAt_ = now;
        faultSaved_ = nv().kv().put("RA_BRAKE_FAULT", fault) == KvPartition::Status::Ok;
        if (faultSaved_) savedFault_ = fault;
      }
    }
    if (policy_.stopRequired()) {
      const ProtectionStopResult result = port_->stop();
      policy_.stopResult(result, millis());
    }
    policy_.advanceRelease(millis());
  }
  #if SWS_ACTIVITY_LED_PIN != OFF
    const bool lit = policy_.ledLit(status.onStepFound, LED_STATUS_ON_STATE == LOW, now);
    const int level = lit ? LOW : HIGH;
    if (level != ledLevel_) { digitalWrite(SWS_ACTIVITY_LED_PIN, level); ledLevel_ = level; }
  #endif
  #if SWS_RA_BRAKE_PIN != OFF
    const bool released = policy_.releaseBrake();
    const int brake = !brakeAvailable() || released ? HIGH : LOW;
    if (brake != brakeLevel_) { digitalWrite(SWS_RA_BRAKE_PIN, brake); brakeLevel_ = brake; }
  #endif
  if (brakeAvailable()) SwsCommandGate::setLocked(!policy_.motionAllowed());
  servicing_ = false;
}

void BoardIo::networkActivity() {
  if (!started_ || !activityAvailable()) return;
  // 原始上电连接流程自行闪灯；网络启动后交由此模块合并活动脉冲
  policy_.traffic(millis());
  tick();
}

void swsNetworkActivity() { boardIo.networkActivity(); }

void BoardIo::pollDriver() {
  if (!started_ || servicing_ || !brakeAvailable() || !policy_.feedbackDue(millis())) return;
  if (!status.onStepFound || status.mountFound != SD_TRUE) {
    tick();
    return;
  }
  // 串口等待可能让出任务，此时禁止嵌套采样、复位或修改设置
  servicing_ = true;
  const ProtectionFeedback feedback = port_->sample();
  policy_.feedback(feedback, millis());
  servicing_ = false;
  tick();
}

bool BoardIo::setProtectionPort(ProtectionPort &port) {
  if (started_) return false;
  port_ = &port;
  return true;
}

bool BoardIo::motionAllowed() {
  if (!brakeAvailable()) return true;
  policy_.checkFeedback(status.onStepFound && status.mountFound == SD_TRUE, millis());
  const bool allowed = started_ && brakeLevel_ == HIGH && policy_.motionAllowed();
  SwsCommandGate::setLocked(!allowed);
  return allowed;
}

bool BoardIo::resetFault() {
  if (!started_ || servicing_ || !brakeAvailable()) return false;
  tick();
  if (!policy_.resetEligible(millis())) return false;
  servicing_ = true;
  const bool saved = nv().kv().put("RA_BRAKE_FAULT", uint8_t(0)) == KvPartition::Status::Ok;
  const bool reset = saved && status.onStepFound && status.mountFound == SD_TRUE && policy_.resetFault(millis());
  if (reset) { savedFault_ = 0; faultSaved_ = true; }
  else if (saved) { faultSaved_ = false; } // 写入期间条件失效，需要重新保存故障
  servicing_ = false;
  tick();
  return reset;
}

bool BoardIo::retryStop() {
  if (!started_ || servicing_ || !brakeAvailable() || !policy_.retryStop()) return false;
  tick();
  return true;
}

bool BoardIo::save(bool activity, bool protection) {
  if (!started_ || servicing_) return false;
  protection = brakeAvailable() && protection;
  if (brakeAvailable() && policy_.protectionEnabled() && !protection) {
    // 先锁定、保存事件并停止，再保存开关，避免断电后只剩关闭设置却丢失故障
    policy_.configure(policy_.activityEnabled(), false, millis());
    tick();
  }
  const uint8_t settings = (activity ? 1 : 0) | (protection ? 2 : 0);
  if (nv().kv().put("BOARD_IO_SETTINGS", settings) != KvPartition::Status::Ok) return false;
  policy_.configure(activity, protection, millis());
  tick();
  return true;
}

bool BoardIo::ownsPin(int pin) const {
  return pin >= 0 && (pin == SWS_ACTIVITY_LED_PIN || pin == SWS_RA_BRAKE_PIN);
}

BrakeState BoardIo::brakeState() const {
  if (!started_) return startFailed_ ? BrakeState::StartFailed : BrakeState::Starting;
  return policy_.brakeState(status.onStepFound && status.mountFound == SD_TRUE, millis());
}
