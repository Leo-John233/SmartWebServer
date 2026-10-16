#include "BoardIo.h"
#include "../../lib/nv/Nv.h"
#include "../../lib/tasks/OnTask.h"
#include "../cmd/Cmd.h"
#include "../status/Status.h"

BoardIo boardIo;
static void pollBoardOutputs() { boardIo.tick(); }
static void pollBoardDriver() { boardIo.pollDriver(); }

void BoardIo::initPins() {
  #if SWS_RA_BRAKE_PIN != OFF
    // 先设置输出锁存值，再切换方向；启动及等待主控期间保持线圈断电。
    digitalWrite(SWS_RA_BRAKE_PIN, LOW);
    pinMode(SWS_RA_BRAKE_PIN, OUTPUT);
    brakeLevel_ = LOW;
  #endif
}

void BoardIo::begin() {
  if (!activityAvailable() && !brakeAvailable()) return;
  uint8_t settings = 3; // 默认开启活动指示和自动保护，不持久保存“已就绪”。
  if (!nv().kv().getOrInit("BOARD_IO_SETTINGS", settings) || settings > 3) settings = 3;
  policy_.configure(settings & 1, brakeAvailable() && (settings & 2), millis());
  #if SWS_ACTIVITY_LED_PIN != OFF
    digitalWrite(SWS_ACTIVITY_LED_PIN, HIGH);
    pinMode(SWS_ACTIVITY_LED_PIN, OUTPUT);
    ledLevel_ = HIGH;
  #endif
  // 无法建立输出监控任务时维持启动锁定，不允许网页声称已经释放。
  if (!tasks.add(20, 0, true, 1, pollBoardOutputs, "BoardIO")) return;
  started_ = true;
  // 与命令通道同优先级，允许在网页让出执行权时采样，避免长页面使反馈过期。
  if (brakeAvailable()) tasks.add(BoardIoPolicy::PollIntervalMs, 0, true, 2, pollBoardDriver, "RaBrake");
  tick();
}

void BoardIo::tick() {
  if (!started_) return;
  const uint32_t now = millis();
  const bool connected = status.onStepFound && status.mountFound == SD_TRUE;
  policy_.checkFeedback(connected, now);
  const bool released = policy_.releaseBrake(connected, now);
  #if SWS_ACTIVITY_LED_PIN != OFF
    const bool lit = policy_.ledLit(status.onStepFound, LED_STATUS_ON_STATE == LOW, now);
    const int level = lit ? LOW : HIGH;
    if (level != ledLevel_) { digitalWrite(SWS_ACTIVITY_LED_PIN, level); ledLevel_ = level; }
  #endif
  #if SWS_RA_BRAKE_PIN != OFF
    // 发送 STOP 期间即使其他任务调用 tick，也不能抢先改变抱闸输出。
    if (!stopping_) {
      const int brake = brakeAvailable() && released ? HIGH : LOW;
      if (brake != brakeLevel_) { digitalWrite(SWS_RA_BRAKE_PIN, brake); brakeLevel_ = brake; }
    }
  #endif
}

void BoardIo::networkActivity() {
  if (!started_ || !activityAvailable()) return;
  // 原始上电连接流程自行闪灯；网络启动后交由此模块合并活动脉冲。
  policy_.traffic(millis());
  tick();
}

void swsNetworkActivity() { boardIo.networkActivity(); }

void BoardIo::pollDriver() {
  if (!started_ || !brakeAvailable() || !policy_.feedbackDue(millis())) return;
  if (!status.onStepFound || status.mountFound != SD_TRUE) {
    driverFeedback(false, "");
    return;
  }
  char reply[80] = "";
  // 同一原始查询，不新增 OnStep 命令；不依赖浏览器是否打开控制器页面。
  const bool received = onStep.processCommand(":GXU1#", reply, 200);
  const size_t length = strlen(reply);
  if (length && reply[length - 1] == '#') reply[length - 1] = 0;
  driverFeedback(received, reply);
}

void BoardIo::driverFeedback(bool received, const char *reply) {
  if (!started_ || !brakeAvailable()) return;
  policy_.driverFeedback(received, reply, millis());
  if (!stopping_ && policy_.stopRequired()) {
    stopping_ = true;
    char response[80] = "";
    // 这里只发送 STOP；后续有效静止反馈由策略确认，不能发完立即抱闸。
    const bool sent = onStep.processCommand(":Q#", response, 200);
    policy_.stopResult(sent);
    stopping_ = false;
  }
  tick();
}

bool BoardIo::save(bool activity, bool protection) {
  if (!started_) return false;
  protection = brakeAvailable() && protection;
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
  if (!started_) return BrakeState::Starting;
  return policy_.brakeState(status.onStepFound && status.mountFound == SD_TRUE, millis());
}
