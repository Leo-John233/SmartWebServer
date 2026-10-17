#line 1 "E:\\GitHub\\SmartWebServer\\legacy\\SmartWebServer\\src\\pages\\network\\StationReconnect.h"
#pragma once

#include <stdint.h>

// 已保存网络的后台恢复策略，独立于单次连接尝试和网页操作。
namespace swsWifi {
class StationReconnect {
  public:
    void init(bool saved, uint32_t now) { knownNetwork = saved; blocked = false; failures = 0; next = now + 2000; }
    void online(uint32_t now) { knownNetwork = true; blocked = false; failures = 0; next = now + 2000; }
    void lost(uint32_t now) { next = now + 2000; }
    void defer(uint32_t now) { next = now + 5000; }
    void failed(uint32_t now, bool permanent) {
      blocked = blocked || permanent;
      const uint32_t delay = failures >= 4 ? 60000 : 5000UL << failures;
      if (failures < 4) failures++;
      next = now + delay;
    }
    bool due(uint32_t now) const { return knownNetwork && !blocked && int32_t(now - next) >= 0; }
    bool known() const { return knownNetwork; }

  private:
    bool knownNetwork = false, blocked = false;
    uint8_t failures = 0;
    uint32_t next = 0;
};
}
