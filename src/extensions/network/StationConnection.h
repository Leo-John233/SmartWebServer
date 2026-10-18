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

    Action poll(uint32_t now, bool connected, bool hasAddress) {
      if (connected && hasAddress) return Success;
      // 连接只发起一次，不在等待路由器响应时再次断开
      if (uint32_t(now - started) > 25000) return Failed;
      return Wait;
    }

  private:
    uint32_t started = 0;
};
}
