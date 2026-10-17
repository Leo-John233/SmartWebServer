#pragma once
#include <stdint.h>

// 只传递现有驱动反馈；ST 来自步进脉冲状态，不代表机械轴速测量
struct ProtectionFeedback {
  bool driverValid = false, driverFault = false, standstill = false;
  uint32_t sampledAt = 0;
};

struct ProtectionStopResult {
  bool haltSent = false, trackingOffAccepted = false;
  bool complete() const { return haltSent && trackingOffAccepted; }
};

// 适配器负责采样和发送停止命令，保护决策留在 SWS 策略中
// sampledAt 必须标记本次采样开始时间，不能复用停止前的缓存
class ProtectionPort {
public:
  virtual ~ProtectionPort() {}
  virtual ProtectionFeedback sample() = 0;
  virtual ProtectionStopResult stop() = 0;
};
