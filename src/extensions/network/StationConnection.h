#pragma once

#include <stdint.h>

// 只决定连接时序，不接触网页、密码存储或无线接口，主循环执行返回的动作
namespace swsWifi {
class StationConnection {
  public:
    enum Action { Wait, Success, Failed };

    void begin(uint32_t now) {
      started = now;
    }

    Action poll(uint32_t now, bool connected, bool error, bool hasAddress) {
      if (connected && hasAddress) return Success;
      // 沿用 7bfba6ed 的单次连接，不在等待路由器响应时再次断开并发起连接
      if (error || uint32_t(now - started) > 25000) return Failed;
      return Wait;
    }

  private:
    uint32_t started = 0;
};
}
