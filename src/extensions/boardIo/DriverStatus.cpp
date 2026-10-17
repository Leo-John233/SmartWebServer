#include "BoardIo.h"
#include "OnStepProtection.h"
#include "../status/DriverStatus.h"

// 将保护采样同步给原版状态对象，页面读取不触发额外串口查询
void State::updateBrakeDriverStatus() {
  if (!boardIo.brakeAvailable()) return;
  char reply[80];
  // 页面只读同一次采样，不额外查询，也不因连续读取失败而停止刷新
  const bool received = status.onStepFound && status.mountFound == SD_TRUE &&
                        onStepProtection.latestDriverReply(reply, sizeof(reply));
  if (received) decodeControllerDriver(reply, driver[0]);
  else driver[0].valid = false;
  formatControllerDriver(driver[0], driverStatusStr[0], sizeof(driverStatusStr[0]));
}
