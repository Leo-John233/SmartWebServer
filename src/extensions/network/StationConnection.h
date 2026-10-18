#pragma once

#include <stdint.h>

// 只决定连接时序，不接触网页、密码存储或无线接口
namespace swsWifi {
class StationConnection {
  public:
    enum Action { Wait, Start, Success, Failed };
    enum : uint32_t { PrepareMs = 250, TimeoutMs = 25000 };

    void begin(uint32_t now) {
      started = now;
      preparing_ = addressSeen = false;
    }
    void prepare(uint32_t now) { begin(now); preparing_ = true; }
    bool preparing() const { return preparing_; }

    Action poll(uint32_t now, bool connected, bool error, bool hasAddress, bool associated = false) {
      // 模式切换和旧连接断开先交给 SDK 处理，之后只发起一次新连接
      if (preparing_) return uint32_t(now - started) >= PrepareMs ? Start : Wait;
      if (connected && hasAddress) return Success;
      if (error) return Failed;
      // 已关联路由器后单独留出获取地址的时间，总时长仍有明确上限
      if (associated && !addressSeen) { addressSeen = true; addressStarted = now; }
      if (uint32_t(now - started) > 2 * TimeoutMs) return Failed;
      if (addressSeen) return uint32_t(now - addressStarted) > TimeoutMs ? Failed : Wait;
      return uint32_t(now - started) > TimeoutMs ? Failed : Wait;
    }

  private:
    bool preparing_ = false, addressSeen = false;
    uint32_t started = 0, addressStarted = 0;
};
}
