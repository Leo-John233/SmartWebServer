#pragma once

#include <stdint.h>

// 已保存网络的后台恢复策略，独立于单次连接尝试和网页操作
namespace swsWifi {
class StationReconnect {
  public:
    void init(bool saved, uint32_t now) { knownNetwork = saved; blocked = false; next = now + 30000; }
    void online(uint32_t now) { knownNetwork = true; blocked = false; next = now + 30000; }
    void lost(uint32_t now) { next = now + 30000; }
    void defer(uint32_t now) { next = now + 30000; }
    // 已验证网络后台超时后稍后再试，手动配网失败仍等待用户处理
    void retryLater(uint32_t now) { blocked = false; next = now + 30000; }
    void failed() { blocked = true; }
    bool due(uint32_t now) const { return knownNetwork && !blocked && int32_t(now - next) >= 0; }
    bool known() const { return knownNetwork; }

  private:
    bool knownNetwork = false, blocked = false;
    uint32_t next = 0;
};
}
