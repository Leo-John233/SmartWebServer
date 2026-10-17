#line 1 "E:\\GitHub\\SmartWebServer\\legacy\\SmartWebServer\\src\\pages\\network\\StationConnection.h"
#pragma once

#include <stdint.h>

// 只决定连接时序，不接触网页、密码存储或无线接口，主循环执行返回的动作。
namespace swsWifi {
class StationConnection {
  public:
    enum Action { Wait, Start, Retry, Success, Failed };

    void begin(uint32_t now) {
      started = phaseAt = now;
      attempts = 0;
      settling = true;
      bad = false;
    }

    Action poll(uint32_t now, bool connected, bool error, bool hasAddress) {
      if (!settling && connected && hasAddress) return Success;
      if (uint32_t(now - started) >= 30000) return Failed;
      if (settling) {
        if (uint32_t(now - phaseAt) < 350) return Wait;
        settling = false;
        phaseAt = now;
        bad = false;
        attempts++;
        return Start;
      }
      if (error) {
        if (!bad) { bad = true; badAt = now; }
      } else bad = false;
      // 忽略启动初期及短暂的失败状态，持续失败或首次等待超时才重试一次。
      const bool rejected = bad && uint32_t(now - phaseAt) >= 3000 && uint32_t(now - badAt) >= 1500;
      const bool expired = uint32_t(now - phaseAt) >= 15000;
      if (!rejected && !expired) return Wait;
      if (attempts >= 2) return Failed;
      settling = true;
      phaseAt = now;
      bad = false;
      return Retry;
    }

  private:
    uint32_t started = 0, phaseAt = 0, badAt = 0;
    uint8_t attempts = 0;
    bool settling = false, bad = false;
};
}
